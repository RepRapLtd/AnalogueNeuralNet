from PIL import Image, ImageDraw, ImageFont, ImageOps
import numpy as np
import string
import os

def generate_8x8_images(font_path, output_dir='images'):
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

    return images

font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"  # Example path
images = generate_8x8_images(font_path)
