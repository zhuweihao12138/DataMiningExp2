/*
 * Modifications Copyright 2022 "Ant Group"
 * Copyright (c) 2002-2022 "Neo4j,"
 * Neo4j Sweden AB [http://neo4j.com]
 *
 * This file is part of Neo4j.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.antgroup.tugraph.ogm.drivers.rpc.driver;

import java.net.URI;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;
import java.util.function.BiFunction;
import java.util.function.Function;

import com.antgroup.tugraph.ogm.drivers.rpc.request.RpcRequest;
import com.antgroup.tugraph.ogm.drivers.rpc.transaction.RpcTransaction;
import com.antgroup.tugraph.TuGraphDbRpcClient;

import com.antgroup.tugraph.ogm.drivers.rpc.exception.ServiceUnavailableException;
import com.antgroup.tugraph.ogm.config.Configuration;
import com.antgroup.tugraph.ogm.config.Credentials;
import com.antgroup.tugraph.ogm.config.UsernamePasswordCredentials;
import com.antgroup.tugraph.ogm.driver.AbstractConfigurableDriver;
import com.antgroup.tugraph.ogm.driver.ExceptionTranslator;
import com.antgroup.tugraph.ogm.exception.ConnectionException;
import com.antgroup.tugraph.ogm.request.Request;
import com.antgroup.tugraph.ogm.transaction.Transaction;
import com.antgroup.tugraph.ogm.transaction.TransactionManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * @author Vince Bickers
 * @author Luanne Misquitta
 * @author Mark Angrish
 * @author Michael J. Simons
 */
public class RpcDriver extends AbstractConfigurableDriver {

    private static final Logger LOGGER = LoggerFactory.getLogger(RpcDriver.class);
    public static final String CONFIG_PARAMETER_RPC_LOGGING = "Rpc_Logging";

    private final ExceptionTranslator exceptionTranslator = new RpcDriverExceptionTranslator();

    private TuGraphDbRpcClient rpcClient;
    private Credentials credentials;
    /**
     * The database to use (Use Tugraph default).
     */
    private String database = "default";

    // required for service loader mechanism
    public RpcDriver() {
    }

    public RpcDriver(TuGraphDbRpcClient rpcClient) {
        this.rpcClient = rpcClient;
    }

    @Override
    public void configure(Configuration newConfiguration) {

        close();

        super.configure(newConfiguration);

        this.credentials = this.configuration.getCredentials();
        this.database = this.configuration.getDatabase();

        if (this.configuration.getVerifyConnection()) {
            checkClientInitialized();
        }
    }

    @Override
    protected String getTypeSystemName() {
        return "org.neo4j.ogm.drivers.rpc.types.RpcNativeTypes";
    }

    public Function<TransactionManager, BiFunction<Transaction.Type, Iterable<String>, Transaction>> getTransactionFactorySupplier() {
        return transactionManager -> (type, bookmarks) -> {
            checkClientInitialized();
            return new RpcTransaction(transactionManager, rpcClient, type);
        };
    }

    @Override
    public void close() {
        try {
            LOGGER.info("Shutting down rpc client {} ", this);
            if (rpcClient != null) {
                rpcClient.logout();
            }
        } catch (Exception e) {
            LOGGER.warn("Unexpected Exception when closing tugraph client rpcClient: ", e);
        }
    }

    private void checkClientInitialized() {
        TuGraphDbRpcClient client = rpcClient;
        if (client == null) {
            synchronized (this) {
                client = rpcClient;
                if (client == null) {
                    createRpcClient();
                }
            }
        }
    }

    static boolean isCorrectScheme(String scheme) {
        String lowerCaseScheme = scheme.toLowerCase(Locale.ENGLISH);
        return  "list".equals(lowerCaseScheme);
    }

    private void createRpcClient() {

        final String serviceUnavailableMessage = "Could not create Rpc client instance";

        try {
            if (credentials != null) {
                UsernamePasswordCredentials usernameAndPassword = (UsernamePasswordCredentials) this.credentials;
                String[] uris = configuration.getURIS();
                String uri = configuration.getURI();
                boolean useURIs = uris != null && Arrays.stream(uris).anyMatch(u -> u != null && !u.isEmpty());
                TuGraphDbRpcClient client;
                if (useURIs) {
                    List<String> urisList = Arrays.asList(uris);
                    client = new TuGraphDbRpcClient(urisList, usernameAndPassword.getUsername(), usernameAndPassword.getPassword());
                    rpcClient = client;
                } else if (uri != null && !uri.isEmpty()) {
                    client = new TuGraphDbRpcClient(uri, usernameAndPassword.getUsername(), usernameAndPassword.getPassword());
                    rpcClient = client;
                } else {
                    LOGGER.debug("Failed to get correct url");
                }
            } else {
                LOGGER.debug("Rpc Driver credentials not supplied");
            }
        } catch (ServiceUnavailableException e) {
            throw new ConnectionException(serviceUnavailableMessage, e);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private URI getSingleURI(String singleUri) {
        return URI.create(singleUri);
    }

    @Override
    public ExceptionTranslator getExceptionTranslator() {
        return this.exceptionTranslator;
    }

    @Override
    public Request request(Transaction transaction) {
        return new RpcRequest(rpcClient, this.parameterConversion, getCypherModification(), database);
    }

}
