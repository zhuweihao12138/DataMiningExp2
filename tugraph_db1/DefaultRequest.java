/*
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
package com.antgroup.tugraph.ogm.session.request;

import java.util.Collections;
import java.util.List;

import com.antgroup.tugraph.ogm.request.Statement;

/**
 * @author Luanne Misquitta
 */
public class DefaultRequest implements com.antgroup.tugraph.ogm.request.DefaultRequest {

    List<Statement> statements;

    public DefaultRequest() {
    }

    public DefaultRequest(Statement statement) {
        this.statements = Collections.singletonList(statement);
    }

    public List<Statement> getStatements() {
        return statements;
    }

    public void setStatements(List<Statement> statements) {
        this.statements = statements;
    }
}
