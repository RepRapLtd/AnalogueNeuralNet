import os
import shutil
import random

def split_font_folders(base_dir, output_dir, split_ratio=0.75):
    train_dir = os.path.join(output_dir, 'Train')
    test_dir = os.path.join(output_dir, 'Test')

    # Create Train and Test directories if they don't exist
    if not os.path.exists(train_dir):
        os.makedirs(train_dir)
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)

    print(f"Train directory: {train_dir}")
    print(f"Test directory: {test_dir}")

    # Get a list of all font folders
    font_folders = [f for f in os.listdir(base_dir) if os.path.isdir(os.path.join(base_dir, f))]
    print(f"Font folders found: {font_folders}")

    # Split the folders into Train and Test
    for folder in font_folders:
        source_path = os.path.join(base_dir, folder)
        if random.random() < split_ratio:
            dest_path = os.path.join(train_dir, folder)
            print(f"Copying {source_path} to {dest_path} (Train)")
        else:
            dest_path = os.path.join(test_dir, folder)
            print(f"Copying {source_path} to {dest_path} (Test)")
        shutil.copytree(source_path, dest_path)

# Define the base directory and the output directory (parent directory)
base_dir = 'font_images'
output_dir = os.path.abspath('.')  # Get the absolute path of the current directory

print(f"Base directory: {base_dir}")
print(f"Output directory: {output_dir}")

# Split the font folders into Train and Test
split_font_folders(base_dir, output_dir)
