﻿/**
 * Copyright 2022 AntGroup CO., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "lgraph/olap_on_db.h"
#include "tools/json.hpp"
#include "./algo.h"

using namespace lgraph_api;
using namespace lgraph_api::olap;
using json = nlohmann::json;

void CountComp(OlapBase<Empty>& graph, ParallelVector<size_t>& label, size_t& max, size_t& num) {
    ParallelVector<size_t> cnt = graph.AllocVertexArray<size_t>();
    cnt.Fill(0);
    graph.ProcessVertexInRange<size_t>(
        [&](size_t v) {
            if (graph.OutDegree(v) == 0) return 0;
            size_t v_label = label[v];
            write_add(&cnt[v_label], (size_t)1);
            return 0;
        },
        0, label.Size());
    max = 0;
    num = graph.ProcessVertexInRange<size_t>(
        [&](size_t v) {
            if (max < cnt[v]) write_max(&max, cnt[v]);
            return (graph.OutDegree(v) > 0 && cnt[v] > 0) ? 1 : 0;
        },
        0, label.Size());
}

/**
 * Processes a request on a GraphDB.
 *
 * @param db The reference to the GraphDB object on which the request will be processed.
 * @param request The input request in JSON format.
 *        The request should contain the following parameters:
 *        - "vertex_label_filter": Filter vertex sets based on vertex labels
 *        - "edge_label_filter": Filter edge sets based on edge labels
 *        - "wcc_value": Vertex field name to be written back into the database.
 *        - "output_file": Wcc value to be written to the file.
 * @param response The output response in JSON format.
 *        The response will contain the following parameters:
 *        - "num_components": The number of components in the graph.
 *        - "max_component": The size of the largest component.
 *        - "num_vertices": The number of vertices in the graph.
 *        - "num_edges": The number of edges in the graph.
 *        - "prepare_cost": The time cost of preparing the graph data.
 *        - "core_cost": The time cost of the core algorithm.
 *        - "output_cost": The time cost of writing the result to a file.
 *        - "total_cost": The total time cost.
 * @return True if the request is processed successfully, false otherwise.
 */

extern "C" bool Process(GraphDB& db, const std::string& request, std::string& response) {
    double start_time;

    // prepare
    start_time = get_time();
    std::string output_file = "";
    std::cout << "Input: " << request << std::endl;
    try {
        json input = json::parse(request);
        parse_from_json(output_file, "output_file", input);
    } catch (std::exception& e) {
        response = "json parse error: " + std::string(e.what());
        std::cout << response << std::endl;
        return false;
    }

    auto txn = db.CreateReadTxn();
    OlapOnDB<Empty> olapondb(db, txn, SNAPSHOT_PARALLEL | SNAPSHOT_UNDIRECTED);
    auto prepare_cost = get_time() - start_time;

    // core
    start_time = get_time();
    ParallelVector<size_t> label = olapondb.AllocVertexArray<size_t>();
    WCCCore(olapondb, label);
    auto core_cost = get_time() - start_time;

    // output
    start_time = get_time();
    if (output_file != "") {
        olapondb.WriteToFile<size_t>(label, output_file);
    }

    size_t num_components, max_component;
    CountComp(olapondb, label, max_component, num_components);
    printf("max_component = %ld\n", max_component);
    printf("num_components = %ld\n", num_components);
    auto output_cost = get_time() - start_time;

    // return
    {
        json output;
        output["num_components"] = num_components;
        output["max_component"] = max_component;
        output["num_vertices"] = olapondb.NumVertices();
        output["num_edges"] = olapondb.NumEdges();
        output["prepare_cost"] = prepare_cost;
        output["core_cost"] = core_cost;
        output["output_cost"] = output_cost;
        output["total_cost"] = prepare_cost + core_cost + output_cost;
        response = output.dump();
    }
    return true;
}
