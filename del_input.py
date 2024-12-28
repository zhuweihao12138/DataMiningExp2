import json

def process_jsonl_file(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as infile, open(output_file, 'w', encoding='utf-8') as outfile:
        for line in infile:
            data = json.loads(line)
            if 'input_field' in data:
                del data['input_field']  # 删除 input_field 字段
            json.dump(data, outfile,ensure_ascii=False)
            outfile.write('\n')  # 添加换行符分隔每个 JSON 对象

# 指定输入和输出文件路径
input_file_path = 'answer_v3_modified.jsonl'
output_file_path = 'answer_v3_modified1.jsonl'

process_jsonl_file(input_file_path, output_file_path)
