import random
from PIL import Image

def create_random_color_png(output_path="mansion.png"):
    # Define the palette of colors
    colors = [
        (255, 0, 0),    # Red
        (255, 165, 0),  # Orange
        (255, 255, 0),  # Yellow
        (0, 255, 0),    # Green
        (0, 0, 255),    # Blue
        (128, 0, 128),  # Purple
        (165, 42, 42),  # Brown
        (0, 0, 0),      # Black
        (255, 255, 255),# White
        (128, 128, 128) # Grey
    ]
    
    # Create a new image
    image = Image.new("RGB", (2048, 2048))
    pixels = image.load()
    
    # Fill the image with random colors
    for x in range(1024):
        for y in range(1024):
            colorr = random.choice(colors)
            pixels[2 * x, 2 * y] = colorr
            pixels[2 * x + 1, 2 * y] = colorr
            pixels[2 * x, 2 * y + 1] = colorr
            pixels[2 * x + 1, 2 * y + 1] = colorr
    
    # Save the image
    image.save(output_path)
    print(f"Image saved as {output_path}")

# Run the function to create the PNG
create_random_color_png()
