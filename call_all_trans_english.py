import json
from http import HTTPStatus
from dashscope import Application

prompt1 = '请把以下内容翻译为英文。\n'
def call_agent_app(prompt):
    response = Application.call(app_id='890843a0ea0b452aafed80fb54574e98',
                                prompt=prompt,
                                api_key='sk-9b70a2c344c14b8aabf1509515a31fb6')

    if response.status_code != HTTPStatus.OK:
        print('request_id=%s, code=%s, message=%s' % (response.request_id, response.status_code, response.message))
        return None
    else:
        return response.output.text

def process_jsonl_file(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as infile, open(output_file, 'w', encoding='utf-8') as outfile:
        for line in infile:
            data = json.loads(line)
            prompt = data.get('input_field')
            output = call_agent_app(prompt1+prompt)

            data['input_field'] = output
            outfile.write(json.dumps(data, ensure_ascii=False) + '\n')
            print(output)

if __name__ == '__main__':
    process_jsonl_file('test1.jsonl', 'test_en.jsonl')
