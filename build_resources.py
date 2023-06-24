import pygame
import struct

def main():
    # 1. Initialize pygame and font
    pygame.init()
    ttf_file = "fonts/notosans.ttf"  # Specify your TTF file path here
    font_size = 160  # Make sure that the letters fit within 80x160
    font = pygame.font.Font(ttf_file, font_size)

    output_file = "bin/glyphs.bin"  # Specify your output file path here

    # Open the binary file for writing
    with open(output_file, 'wb') as f:
        # 2. Render each letter a-z
        for letter in range(255):
            if letter == 0:
                continue
            surface = font.render(bytes([letter]), True, (255, 255, 255))  # white text
            surface = pygame.transform.scale(surface, (80, 160))  # resize

            # 3. Write pixel data to file
            for y in range(surface.get_height()):
                for x in range(surface.get_width()):
                    r, g, b, a = surface.get_at((x, y))  # Get the RGBA values
                    f.write(bytes([r, g, b, a]))  # Write as 4 8-bit integers

if __name__ == "__main__":
    main()