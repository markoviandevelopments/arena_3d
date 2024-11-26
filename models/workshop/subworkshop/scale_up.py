from PIL import Image

def scale_up_image(input_path, output_path):
    """
    Scales up a 1024x1024 image to 2048x2048 by copying each pixel into a 2x2 block.
    
    Args:
        input_path (str): Path to the input image.
        output_path (str): Path to save the scaled-up image.
    """
    # Open the input image
    image = Image.open(input_path)
    
    # Ensure the input image is 1024x1024
    if image.size != (1024, 1024):
        raise ValueError("Input image must be 1024x1024 in size.")
    
    # Create a new image with the desired size
    new_image = Image.new("RGB", (2048, 2048))
    new_pixels = new_image.load()
    original_pixels = image.load()
    
    # Scale up each pixel
    for x in range(1024):
        for y in range(1024):
            original_pixel = original_pixels[x, y]
            # Copy the pixel into a 2x2 block
            for dx in range(2):
                for dy in range(2):
                    new_pixels[x * 2 + dx, y * 2 + dy] = original_pixel
    
    # Save the scaled-up image
    new_image.save(output_path)
    print(f"Image saved as {output_path}")

# Example usage
scale_up_image("mansion_map.png", "mansino_map.png")
