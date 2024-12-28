import os
import shutil

def flatten_directory(source_dir, target_dir):
    # 创建目标文件夹，如果不存在
    os.makedirs(target_dir, exist_ok=True)

    # 遍历源文件夹
    for root, _, files in os.walk(source_dir):
        for file in files:
            # 构造源文件的完整路径
            source_file = os.path.join(root, file)
            # 复制文件到目标文件夹
            shutil.copy2(source_file, target_dir)

source_directory = 'tugraph_db'  # 源文件夹路径
target_directory = 'tugraph_db1'   # 目标文件夹路径

flatten_directory(source_directory, target_directory)

print("所有文件已复制到目标文件夹。")
