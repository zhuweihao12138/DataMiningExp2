import json

# Path to your input and output files
input_file_path = 'val.jsonl'
output_file_path = 'val.txt'

# Read the JSONL file and write to the TXT file
with open(input_file_path, 'r', encoding='utf-8') as infile, open(output_file_path, 'w', encoding='utf-8') as outfile:
    for line in infile:
        data = json.loads(line)
        input_field = data.get("input_field", "")
        output_field = data.get("output_field", "")
        formatted_line = f"询问：\"{input_field}\" 回答：\"{output_field}\"\n"
        outfile.write(formatted_line)

print(f"Data has been successfully written to {output_file_path}")
