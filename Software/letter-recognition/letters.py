import os
from PIL import Image, ImageDraw, ImageFont, ImageOps
import numpy as np
import string

def generate_8x8_images(font_path, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    font = ImageFont.truetype(font_path, 64)  # Use a large font size
    letters = string.ascii_lowercase

    images = {}
    for letter in letters:
        # Create a high-resolution blank image with a pure black background
        high_res_image = Image.new('L', (128, 128), color=0)  # Larger canvas to avoid cut-off
        draw = ImageDraw.Draw(high_res_image)

        # Calculate the bounding box of the text in advance
        text_bbox = draw.textbbox((0, 0), letter, font=font)
        text_width = text_bbox[2] - text_bbox[0]
        text_height = text_bbox[3] - text_bbox[1]

        # Center the text
        text_x = (128 - text_width) // 2
        text_y = (128 - text_height) // 2

        # Draw the letter on the high-res image in white
        draw.text((text_x, text_y), letter, font=font, fill=255)  # Draw the letter in white

        # Find the bounding box of the non-black pixels
        bbox = high_res_image.getbbox()

        if bbox:
            high_res_image = high_res_image.crop(bbox)  # Crop to bounding box

            # Determine the scaling factor to fit the letter in an 8x8 image
            bbox_width, bbox_height = high_res_image.size
            scaling_factor = min(8 / bbox_width, 8 / bbox_height)
            new_size = (int(bbox_width * scaling_factor), int(bbox_height * scaling_factor))

            # Check for zero width or height after resizing
            if new_size[0] == 0 or new_size[1] == 0:
                print(f"Abandoning font {font_path} due to zero width or height for letter '{letter}'")
                return False

            # Resize the cropped image to fit within 8x8
            resized_image = high_res_image.resize(new_size, Image.LANCZOS)

            # Create an 8x8 blank image with black background and paste the resized letter in the center
            final_image = Image.new('L', (8, 8), color=0)
            paste_position = ((8 - new_size[0]) // 2, (8 - new_size[1]) // 2)
            final_image.paste(resized_image, paste_position)

            # Invert the image to make the letter black on a white background
            final_image = ImageOps.invert(final_image)

            # Apply binary threshold to create a black and white image
            thresholded_image = final_image.point(lambda p: 255 if p > 128 else 0, mode='1')

            # Save the image
            image_path = os.path.join(output_dir, f'{letter}.png')
            thresholded_image.save(image_path)

            # Convert to numpy array
            images[letter] = np.array(thresholded_image)

    return True

def supports_roman_letters(font_path):
    try:
        font = ImageFont.truetype(font_path, 64)
        for letter in string.ascii_lowercase:
            if not font.getmask(letter).getbbox():
                return False
        return True
    except:
        return False

def process_fonts(font_dir, output_base_dir):
    # Get a list of all ttf files in the font directory
    font_files = []
    for root, _, files in os.walk(font_dir):
        for file in files:
            if file.endswith('.ttf'):
                font_files.append(os.path.join(root, file))

    # Create the output base directory if it doesn't exist
    if not os.path.exists(output_base_dir):
        os.makedirs(output_base_dir)

    # List to store the relative paths of generated image folders
    folders_list = []

    # Process each font file
    for font_file in font_files:
        if supports_roman_letters(font_file):
            font_name = os.path.splitext(os.path.basename(font_file))[0]
            output_dir = os.path.join(output_base_dir, font_name)
            if generate_8x8_images(font_file, output_dir):
                folders_list.append(os.path.relpath(output_dir, start=output_base_dir))

    # Write the list of folders to a text file
    with open(os.path.join(output_base_dir, 'font_folders.txt'), 'w') as f:
        for folder in folders_list:
            f.write(folder + '\n')

# Define the font directory and output directory
font_dir = '/usr/share/fonts/truetype'
output_base_dir = 'font_images'

# Process the fonts and generate the image sets
process_fonts(font_dir, output_base_dir)
