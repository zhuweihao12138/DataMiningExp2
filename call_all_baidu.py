# 安装说明：
# 执行如下命令，快速安装Python语言的最新版本AppBuilder-SDK（要求Python >= 3.9)：
# pip install --upgrade appbuilder-sdk
import appbuilder
import os
import json
from http import HTTPStatus
from dashscope import Application
# 设置环境中的TOKEN，以下TOKEN请替换为您的个人TOKEN，个人TOKEN可通过该页面【获取鉴权参数】或控制台页【密钥管理】处获取
os.environ["APPBUILDER_TOKEN"] = "bce-v3/ALTAK-UO8hFLbo6Obb52OWUWReo/ff41a6653113822d5f0fe7428942ed8ae44e2d35"

# 从AppBuilder控制台【个人空间】-【应用】网页获取已发布应用的ID
app_id = "e1e259be-92ad-4748-a528-1d652f5af0f9"

def call_agent_app(prompt):
    app_builder_client = appbuilder.AppBuilderClient(app_id)
    conversation_id = app_builder_client.create_conversation()
    resp = app_builder_client.run(conversation_id, prompt)
    return resp.content.answer
app_builder_client = appbuilder.AppBuilderClient(app_id)

def process_jsonl_file(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as infile, open(output_file, 'w', encoding='utf-8') as outfile:
        for line in infile:
            data = json.loads(line)
            prompt = data.get('input_field')
            output = call_agent_app(prompt)

            # Add output_field to the data
            data['output_field'] = output
            outfile.write(json.dumps(data, ensure_ascii=False) + '\n')
            print(output)

if __name__ == '__main__':
    process_jsonl_file('test1 copy.jsonl', 'answer_baidu_v1_rest.jsonl')