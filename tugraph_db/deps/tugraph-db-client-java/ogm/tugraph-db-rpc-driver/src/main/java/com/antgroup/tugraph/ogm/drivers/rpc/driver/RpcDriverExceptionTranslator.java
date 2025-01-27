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

import com.antgroup.tugraph.ogm.drivers.rpc.exception.ServiceUnavailableException;
import com.antgroup.tugraph.ogm.driver.ExceptionTranslator;
import com.antgroup.tugraph.ogm.exception.ConnectionException;

/**
 * Translates exceptions from the Java driver into unchecked exceptions.
 *
 * @author Michael J. Simons
 * @soundtrack Die Toten Hosen - Ein kleines bisschen Horrorschau
 * @since 3.2
 */
class RpcDriverExceptionTranslator implements ExceptionTranslator {

    @Override
    public RuntimeException translateExceptionIfPossible(Throwable e) {

        if (e instanceof ServiceUnavailableException) {
            return new ConnectionException(e.getMessage(), e);
        }

        if (e instanceof RuntimeException) {
            return (RuntimeException) e;
        }

        return new RuntimeException(e);
    }
}
