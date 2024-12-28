import os
import glob

# Specify the folder path
folder_path = 'tugraph_db1'

# Use glob to find all .png and .svg files in the folder
png_files = glob.glob(os.path.join(folder_path, '*.png'))
svg_files = glob.glob(os.path.join(folder_path, '*.svg'))

# Delete each file found
for file_path in png_files + svg_files:
    try:
        os.remove(file_path)
        print(f"Deleted: {file_path}")
    except Exception as e:
        print(f"Failed to delete {file_path}: {e}")

print("Deletion completed.")
