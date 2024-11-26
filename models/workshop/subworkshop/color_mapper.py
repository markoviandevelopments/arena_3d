import threading
import pygame
from PIL import Image
import sys

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

def load_image_to_grid(image_path):
    """Loads the image and converts it into a grid of colors."""
    image = Image.open(image_path).convert('RGB')  # Ensure image is in RGB mode
    pixels = image.load()
    grid_size = 1024
    grid = [[None for _ in range(grid_size)] for _ in range(grid_size)]
    for x in range(grid_size):
        for y in range(grid_size):
            # Each block is 2x2 pixels
            px = x * 2
            py = y * 2
            color = pixels[px, py]
            grid[x][y] = color
    return grid

def colors_match(color1, color2, tolerance=0):
    """Checks if two colors match within a given tolerance."""
    return all(abs(c1 - c2) <= tolerance for c1, c2 in zip(color1, color2))

def get_user_color():
    """Prompts the user to input a color abbreviation."""
    print("\nEnter a color using these abbreviations:")
    for abbr, color in COLOR_MAP.items():
        print(f"{abbr}: {color}")
    while True:
        color_abbr = input("Enter color abbreviation (or 'exit' to quit): ").strip().upper()
        if color_abbr == 'EXIT':
            return None
        if color_abbr in COLOR_MAP:
            return COLOR_MAP[color_abbr]
        else:
            print("Invalid abbreviation. Try again.")

def find_matching_sequences(grid, input_colors):
    """Finds all sequences in the grid matching the input colors."""
    grid_size = len(grid)
    sequence_length = len(input_colors)
    matches = []
    directions = [
        (1, 0),   # Right
        (0, 1),   # Down
        (-1, 0),  # Left
        (0, -1),  # Up
        (1, 1),   # Down-right
        (-1, -1), # Up-left
        (1, -1),  # Up-right
        (-1, 1),  # Down-left
    ]
    for x in range(grid_size):
        for y in range(grid_size):
            for dx, dy in directions:
                positions = []
                for i in range(sequence_length):
                    nx = x + i * dx
                    ny = y + i * dy
                    if 0 <= nx < grid_size and 0 <= ny < grid_size:
                        if not colors_match(grid[nx][ny], input_colors[i]):
                            break
                        else:
                            positions.append((nx, ny))
                    else:
                        break
                else:
                    matches.append(positions)
    return matches

def display_image_with_highlights(image_path, matches_queue, stop_event):
    """Displays the image with the matching sequences highlighted in a separate thread."""
    pygame.init()
    # Get screen size and scale image to fit if necessary
    screen_width, screen_height = pygame.display.Info().current_w, pygame.display.Info().current_h
    scale_factor = min(screen_width / 2048, screen_height / 2048, 1)
    window_size = (int(2048 * scale_factor), int(2048 * scale_factor))
    screen = pygame.display.set_mode(window_size)
    pygame.display.set_caption("Pattern Highlight")

    # Load and scale the image
    image = Image.open(image_path).convert('RGB')
    mode = image.mode
    size = image.size
    data = image.tobytes()
    pygame_image = pygame.image.fromstring(data, size, mode)
    if scale_factor != 1:
        pygame_image = pygame.transform.smoothscale(pygame_image, window_size)

    block_size = 2 * scale_factor  # Each block is 2x2 pixels, scaled

    while not stop_event.is_set():
        # Check if there are new matches to display
        if not matches_queue.empty():
            matches = matches_queue.get()
            # Prepare the highlight surface
            highlight_surface = pygame.Surface(window_size, pygame.SRCALPHA)
            # Draw the highlights
            for match in matches:
                for x, y in match:
                    rect = pygame.Rect(x * block_size, y * block_size, block_size, block_size)
                    pygame.draw.rect(highlight_surface, (255, 0, 0, 228), rect)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                stop_event.set()
                return

        # Draw the image and highlights
        screen.blit(pygame_image, (0, 0))
        if 'highlight_surface' in locals():
            screen.blit(highlight_surface, (0, 0))
        pygame.display.flip()
        pygame.time.wait(100)  # Wait to reduce CPU usage

    pygame.quit()

def main():
    import queue

    grid = load_image_to_grid("mansion.png")  # Source image grid
    matches_queue = queue.Queue()
    stop_event = threading.Event()

    # Start the Pygame display in a separate thread
    display_thread = threading.Thread(
        target=display_image_with_highlights,
        args=("mansion1.png", matches_queue, stop_event),
        daemon=True
    )
    display_thread.start()

    input_colors = []

    while not stop_event.is_set():
        color = get_user_color()
        if color is None:
            print("Exiting program.")
            stop_event.set()
            break
        input_colors.append(color)
        print(f"Current input colors: {input_colors}")

        matches = find_matching_sequences(grid, input_colors)
        print(f"Found {len(matches)} matching sequences.")

        if len(matches) == 0:
            print("No matches found. The sequence does not exist in the image.")
            continue

        # Update the display by putting the matches in the queue
        matches_queue.put(matches)

    print("Program terminated.")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nProgram interrupted by user.")
        sys.exit()
