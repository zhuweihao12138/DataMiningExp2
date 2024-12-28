import os

def process_md_files(issue_folder):
    # 遍历所有P1-P5文件夹
    for folder in ['P1', 'P2', 'P3', 'P4', 'P5']:
        folder_path = os.path.join(issue_folder, folder)
        if not os.path.exists(folder_path):
            continue
        
        md_files = [f for f in os.listdir(folder_path) if f.endswith('.md')]
        
        # 按照序号重命名并添加标题
        for index, filename in enumerate(md_files, start=1):
            old_file_path = os.path.join(folder_path, filename)
            
            # 写入文件名作为标题
            with open(old_file_path, 'r', encoding='utf-8') as file:
                content = file.readlines()
            
            title = f"# {os.path.splitext(filename)[0]}\n"
            content.insert(0, title)  # 插入到第一行
            
            with open(old_file_path, 'w', encoding='utf-8') as file:
                file.writelines(content)
            
            # 重命名文件
            new_file_name = f"{index}.md"
            new_file_path = os.path.join(folder_path, new_file_name)
            os.rename(old_file_path, new_file_path)

# 指定issue文件夹路径
issue_folder_path = "issue"
process_md_files(issue_folder_path)
