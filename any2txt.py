import os
import shutil

def rename_and_copy_files(src_dir, dst_dir):
    # 创建目标文件夹
    os.makedirs(dst_dir, exist_ok=True)

    # 遍历源文件夹及其子文件夹
    for root, dirs, files in os.walk(src_dir):
        # 计算当前文件夹在目标文件夹中的路径
        relative_path = os.path.relpath(root, src_dir)
        current_dst_dir = os.path.join(dst_dir, relative_path)
        
        # 创建当前子文件夹
        os.makedirs(current_dst_dir, exist_ok=True)

        for file in files:
            # 获取文件的完整路径
            src_file_path = os.path.join(root, file)
            # 构造新文件名
            new_file_name = f"{os.path.splitext(file)[0]}.txt"
            dst_file_path = os.path.join(current_dst_dir, new_file_name)

            # 复制并重命名文件
            shutil.copy(src_file_path, dst_file_path)
            print(f"复制并重命名: {src_file_path} -> {dst_file_path}")

# 使用示例
source_directory = "tugraph_db1"
destination_directory = "tugraph_db2"
rename_and_copy_files(source_directory, destination_directory)
