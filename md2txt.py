import os

# 指定源文件夹和目标文件夹
source_folder = 'source1'  # 替换为你的源文件夹路径
target_folder = 'source1_txt'  # 替换为你想要保存的目标文件夹路径

# 创建目标文件夹（如果不存在）
os.makedirs(target_folder, exist_ok=True)

# 遍历源文件夹中的所有文件
for filename in os.listdir(source_folder):
    if filename.endswith('.md'):
        md_file_path = os.path.join(source_folder, filename)
        txt_file_path = os.path.join(target_folder, filename.replace('.md', '.txt'))
        
        # 读取.md文件并写入.txt文件
        with open(md_file_path, 'r', encoding='utf-8') as md_file:
            content = md_file.read()
        
        with open(txt_file_path, 'w', encoding='utf-8') as txt_file:
            txt_file.write(content)

print("转换完成！")
