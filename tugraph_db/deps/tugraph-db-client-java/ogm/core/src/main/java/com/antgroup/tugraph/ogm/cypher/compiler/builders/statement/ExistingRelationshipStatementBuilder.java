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
package com.antgroup.tugraph.ogm.cypher.compiler.builders.statement;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.antgroup.tugraph.ogm.model.Edge;
import com.antgroup.tugraph.ogm.model.Property;
import com.antgroup.tugraph.ogm.request.OptimisticLockingConfig;
import com.antgroup.tugraph.ogm.request.Statement;
import com.antgroup.tugraph.ogm.request.StatementFactory;
import com.antgroup.tugraph.ogm.cypher.compiler.CypherStatementBuilder;

/**
 * @author Luanne Misquitta
 * @author Mark Angrish
 */
public class ExistingRelationshipStatementBuilder implements CypherStatementBuilder {

    private final StatementFactory statementFactory;

    private final Set<Edge> edges;

    /**
     * Set to {@literal true} if all the edges above are dirty (changed) edges.
     */
    private final boolean dirtyEdges;

    public ExistingRelationshipStatementBuilder(Set<Edge> edges, StatementFactory statementFactory, boolean dirtyOnes) {
        this.edges = edges;
        this.statementFactory = statementFactory;
        this.dirtyEdges = dirtyOnes;
    }

    @Override
    public Statement build() {
        final Map<String, Object> parameters = new HashMap<>();
        final StringBuilder queryBuilder = new StringBuilder();

        Edge firstEdge = edges.iterator().next();
        if (edges.size() > 0) {
            queryBuilder.append("UNWIND $rows AS row MATCH ()-[r]->() WHERE ID(r) = row.relId ");

            if (firstEdge.hasVersionProperty()) {
                queryBuilder.append(OptimisticLockingUtils.getFragmentForExistingNodesAndRelationships(firstEdge, "r", dirtyEdges ? 1 : 0));
            }

            queryBuilder.append(firstEdge.createPropertyRemovalFragment("r"));

            queryBuilder.append("SET r += row.props ");
            queryBuilder.append("RETURN ID(r) as ref, ID(r) as id, $type as type");
            List<Map> rows = new ArrayList<>();
            for (Edge edge : edges) {
                Map<String, Object> rowMap = new HashMap<>();
                rowMap.put("relId", edge.getId());
                Map<String, Object> props = new HashMap<>();
                for (Property property : edge.getPropertyList()) {
                    if (!property.equals(edge.getVersion())) {
                        props.put((String) property.getKey(), property.getValue());
                    }
                }
                rowMap.put("props", props);
                if (edge.hasVersionProperty()) {
                    Property version = edge.getVersion();
                    rowMap.put((String) version.getKey(), version.getValue());
                }
                rows.add(rowMap);
            }
            parameters.put("rows", rows);
            parameters.put("type", "rel");
            if (firstEdge.hasVersionProperty()) {
                OptimisticLockingConfig olConfig = new OptimisticLockingConfig(rows.size(),
                    new String[] { firstEdge.getType() }, firstEdge.getVersion().getKey());
                return statementFactory.statement(queryBuilder.toString(), parameters, olConfig);
            }

        }

        return statementFactory.statement(queryBuilder.toString(), parameters);
    }
}
