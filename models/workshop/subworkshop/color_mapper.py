import pygame
from PIL import Image

# Define color palette with abbreviations
COLOR_MAP = {
    "R": (255, 0, 0),      # Red
    "O": (255, 165, 0),    # Orange
    "Y": (255, 255, 0),    # Yellow
    "G": (0, 255, 0),      # Green
    "B": (0, 0, 255),      # Blue
    "P": (128, 0, 128),    # Purple
    "BR": (165, 42, 42),   # Brown
    "BL": (0, 0, 0),       # Black
    "W": (255, 255, 255),  # White
    "GR": (128, 128, 128)  # Grey
}

def rotate_pattern(pattern):
    """Generates all 4 rotations of a 3x3 pattern."""
    rotations = [pattern]
    for _ in range(3):
        rotated = [list(reversed(col)) for col in zip(*rotations[-1])]
        rotations.append(rotated)
    return rotations

def match_pattern(image_pixels, x, y, pattern):
    """Checks if a given pattern matches the image at block position (x, y)."""
    for dx in range(3):  # over pattern blocks
        for dy in range(3):
            # Map block coordinates to pixel coordinates
            px = (x + dx) * 2
            py = (y + dy) * 2
            if px + 1 >= 2048 or py + 1 >= 2048:  # Bounds check
                return False
            # Get the color in the block from the image
            block_color = image_pixels[px, py]
            # Compare the block color to the pattern color
            if block_color != pattern[dx][dy]:
                return False
    return True

def get_user_colors():
    """Prompts the user to input colors using abbreviations."""
    print("\nEnter the 9 colors for the 3x3 pattern using these abbreviations:")
    for abbr, color in COLOR_MAP.items():
        print(f"{abbr}: {color}")
    colors = []
    for i in range(9):
        while True:
            color_abbr = input(f"Enter color {i + 1} abbreviation: ").strip().upper()
            if color_abbr in COLOR_MAP:
                colors.append(COLOR_MAP[color_abbr])
                break
            else:
                print("Invalid abbreviation. Try again.")
    return colors

def search_and_display_with_highlight(input_path="mansion_map.png"):
    # Load the image
    image = Image.open(input_path)
    pixels = image.load()

    while True:
        # Get the colors to search
        colors_to_search = get_user_colors()
        pattern = [colors_to_search[i:i + 3] for i in range(0, 9, 3)]
        rotations = rotate_pattern(pattern)

        # Search for the pattern in the image
        found_coords = None
        for x in range(1022):  # Iterate over blocks (0 to 1021)
            for y in range(1022):
                for rotated_pattern in rotations:
                    if match_pattern(pixels, x, y, rotated_pattern):
                        found_coords = (x + 1, y + 1)  # Center block position
                        print(f"Pattern found at center block ({x + 1}, {y + 1})")
                        break
                if found_coords:
                    break
            if found_coords:
                break

        if not found_coords:
            print("Pattern not found.")
        else:
            # Use Pygame to display the image with the highlight
            pygame.init()
            screen = pygame.display.set_mode((2048, 2048))
            pygame.display.set_caption("Pattern Highlight")

            # Convert the PIL image to a format usable by Pygame
            mode = image.mode
            size = image.size
            data = image.tobytes()
            pygame_image = pygame.image.fromstring(data, size, mode)

            running = True
            while running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False
                    if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                        running = False

                # Draw the image
                screen.blit(pygame_image, (0, 0))

                # Highlight the found 3x3 area in yellow
                # Each block is 2x2 pixels, so the 3x3 blocks cover a 6x6 pixel area
                x_pixel, y_pixel = (found_coords[0] - 1) * 2, (found_coords[1] - 1) * 2
                highlight_rect = pygame.Surface((6 * 2, 6 * 2), pygame.SRCALPHA)
                highlight_rect.fill((255, 255, 0, 100))  # Semi-transparent yellow
                screen.blit(highlight_rect, (x_pixel, y_pixel))

                pygame.display.flip()

            pygame.quit()

        # Ask user if they want to search again
        retry = input("Do you want to search for another pattern? (yes/no): ").strip().lower()
        if retry != "yes":
            break

# Run the program
search_and_display_with_highlight()
