import { getSceneData } from '@/service/get-public-data/get-public-data'
const Demos = [
    {
        name: 'MovieDemo',
        key: 'movie',
        description: '电影示例图谱。',
        data: async () => {
            let fileNames = [
                'person_node',
                'genre_node',
                'keyword_node',
                'movie_node',
                'user_node',
                'acted_in_rels',
                'directed_rels',
                'friends_rel',
                'has_genre_rels',
                'produced_rels',
                'ratings',
                'has_keyword_rels',
                'writer_of_rels'
            ]
            let dirName = 'movie'
            let dataPromiseList = []
            fileNames.forEach((name) => {
                dataPromiseList.push(getSceneData({ dirName: dirName, fileName: name, last: 'json' }))
            })
            let res = await Promise.all(dataPromiseList)
            let data = res.map((item) => {
                return item.data
            })
            return data
        },
        createLabelScript: [
            `CALL db.createVertexLabel('genre', 'id' , 'id' ,INT32, false, 'name' ,STRING, true)`,
            `CALL db.createVertexLabel('keyword', 'id' , 'id' ,INT32, false, 'name' ,STRING, true)`,
            `CALL db.createVertexLabel('movie', 'id' , 'id' ,INT32, false, 'title' ,STRING, true, 'tagline' ,STRING, true, 'summary' ,STRING, true, 'poster_image' ,STRING, true, 'duration' ,INT32, true, 'rated' ,STRING, true)`,
            `CALL db.createVertexLabel('person', 'id' , 'id' ,INT32, false, 'name' ,STRING, true, 'born' ,INT32, true, 'poster_image' ,STRING, true)`,
            `CALL db.createVertexLabel('user', 'id' , 'id' ,INT32, false, 'login' ,STRING, true)`,
            `CALL db.createEdgeLabel('acted_in','[["person","movie"]]', 'role', STRING, false)`,
            `CALL db.createEdgeLabel('directed','[["person","movie"]]')`,
            `CALL db.createEdgeLabel('is_friend','[["user","user"]]')`,
            `CALL db.createEdgeLabel('has_genre','[["movie","genre"]]')`,
            `CALL db.createEdgeLabel('has_keyword','[["movie","keyword"]]')`,
            `CALL db.createEdgeLabel('produce','[["person","movie"]]')`,
            `CALL db.createEdgeLabel('rate','[["user","movie"]]', 'stars', INT8, false)`,
            `CALL db.createEdgeLabel('write','[["person","movie"]]')`
        ]
    },
    {
        name: 'CovidDemo',
        key: 'covid',
        description: '疫情追踪图谱。',
        data: async () => {
            let fileNames = [
                'person',
                'address',
                'city',
                'district',
                'street_0',
                'street_1',
                'street_2',
                'street_3',
                'street_4',
                'street_5',
                'town',
                'province',
                'person_live_with_person',
                'person_visit_address',
                'province_to_city',
                'city_to_district',
                'district_to_town',
                'town_to_street_0',
                'town_to_street_1',
                'street_to_address'
            ]
            let dirName = 'covid-track'
            let dataPromiseList = []
            fileNames.forEach((name) => {
                dataPromiseList.push(getSceneData({ dirName: dirName, fileName: name, last: 'json' }))
            })
            let res = await Promise.all(dataPromiseList)
            let data = res.map((item) => {
                return item.data
            })
            return data
        },
        createLabelScript: [
            `CALL db.createVertexLabel('person', 'id' , 'id' , STRING, false, 'name' ,STRING, true, 'is_confirmed', BOOL, true, 'confirmed_at' , STRING, true)`,
            `CALL db.createVertexLabel('address', 'id' , 'id' , STRING, false, 'name' ,STRING, true, 'street_id' , STRING, true)`,
            `CALL db.createVertexLabel('street', 'id' , 'id' , STRING, false,'incremental_id',STRING, true, 'name' ,STRING, true, 'city_id' , STRING, true)`,
            `CALL db.createVertexLabel('city', 'id' , 'id' , STRING, false, 'name' ,STRING, true, 'province_id' , STRING, true)`,
            `CALL db.createVertexLabel('province', 'id' , 'id' , STRING, false, 'name' ,STRING, true)`,
            `CALL db.createVertexLabel('district', 'id' , 'id' , STRING, false, 'name' ,STRING, true, 'city_id' , STRING, true)`,
            `CALL db.createVertexLabel('town', 'id' , 'id' , STRING, false, 'name' ,STRING, true, 'district_id' , STRING, true)`,
            `CALL db.createEdgeLabel('person_live_with_person','[["person","person"]]', 'start_time', STRING, false, 'end_time', STRING, false)`,
            `CALL db.createEdgeLabel('person_visit_address','[["person","address"]]',  'start_time', STRING, false, 'end_time', STRING, false)`,
            `CALL db.createEdgeLabel('province_to_city','[["province","city"]]')`,
            `CALL db.createEdgeLabel('city_to_district','[["city","district"]]')`,
            `CALL db.createEdgeLabel('district_to_town','[["district","town"]]')`,
            `CALL db.createEdgeLabel('town_to_street','[["town","street"]]')`,
            `CALL db.createEdgeLabel('street_to_address','[["street","address"]]')`
        ],
        createIndexScript: []
    },
    {
        name: '流浪地球',
        key: 'WE',
        description: '流浪地球示例',
        data: async () => {
            let fileNames = [
                '天体与设施',
                '组织',
                '角色',
                '事件_天体与设施_天体与设施',
                '事件_组织_天体与设施',
                '事件_组织_角色',
                '事件_角色_天体与设施',
                '事件_角色_组织',
                '事件_角色_角色',
                '关系_角色_天体与设施',
                '关系_角色_组织',
                '关系_角色_角色'
            ]
            let dirName = 'wandering-earth'
            let dataPromiseList = []
            fileNames.forEach((name) => {
                dataPromiseList.push(getSceneData({ dirName: dirName, fileName: name, last: 'json' }))
            })
            let res = await Promise.all(dataPromiseList)
            let data = res.map((item) => {
                return item.data
            })
            return data
        },
        createLabelScript: [
            `CALL db.createVertexLabel('天体与设施', 'name' , 'name' ,STRING, false, 'description' ,STRING, true)`,
            `CALL db.createVertexLabel('组织', 'name' , 'name' ,STRING, false, 'description' ,STRING, true)`,
            `CALL db.createVertexLabel('角色', 'name' , 'name' ,STRING, false, 'description' ,STRING, true, 'birthYear', INT32, true)`,
            `CALL db.createEdgeLabel('关系','[["角色","角色"],["角色","天体与设施"],["角色","组织"]]', 'name', STRING, true)`,
            `CALL db.createEdgeLabel('事件关系','[["角色","角色"],["角色","天体与设施"],["角色","组织"],["天体与设施","天体与设施"],["组织","角色"],["组织","天体与设施"]]', 'title', STRING, true, 'year', INT32, true, 'no', INT32, false, 'name', STRING, true)`
        ]
    },
    {
        name: '三体',
        key: 'TB',
        description: '三体示例。',
        data: async () => {
            let fileNames = ['三体人物表', '三体时间线', '三体组织合集', '三体计划合集', '人物-人物-关系', '人物-组织-关系', '人物-计划-关系', '组织-组织-关系', '组织-计划-关系', '三体时间关系表']
            let dirName = 'three-body'
            let dataPromiseList = []
            fileNames.forEach((name) => {
                dataPromiseList.push(getSceneData({ dirName: dirName, fileName: name, last: 'json' }))
            })
            let res = await Promise.all(dataPromiseList)
            let data = res.map((item) => {
                return item.data
            })
            return data
        },
        createLabelScript: [
            `CALL db.createVertexLabel('person', 'name' , 'name' ,STRING, false, 'introduce' ,STRING, true)`,
            `CALL db.createVertexLabel('organization', 'name' , 'name' ,STRING, false, 'introduce' ,STRING, true)`,
            `CALL db.createVertexLabel('plan', 'name' , 'name' ,STRING, false, 'content' ,STRING, true)`,
            `CALL db.createVertexLabel('time', 'name' , 'name' ,STRING, false, 'time' ,STRING, true, 'important_events' ,STRING, true)`,
            `CALL db.createEdgeLabel('person_person','[["person","person"]]', 'relation', STRING, true)`,
            `CALL db.createEdgeLabel('person_plan','[["person","plan"]]', 'relation', STRING, true)`,
            `CALL db.createEdgeLabel('person_organization','[["person","organization"]]', 'relation', STRING, true)`,
            `CALL db.createEdgeLabel('organization_plan','[["organization","plan"]]', 'relation', STRING, true)`,
            `CALL db.createEdgeLabel('organization_organization','[["organization","organization"]]', 'relation', STRING, true)`,
            `CALL db.createEdgeLabel('time_time','[["time","time"]]', 'relation', STRING, true)`
        ]
    }
]

export default Demos
