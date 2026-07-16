from PIL import Image
import sys
import os

if len(sys.argv) != 3:
    print("Usage:")
    print("    python png_to_rgb565_c.py image.png image.h")
    sys.exit(1)

png_file = sys.argv[1]
h_file = sys.argv[2]


# Chargement image
img = Image.open(png_file).convert("RGB")

w, h = img.size
pixels = img.load()


rgb565 = []


def rgb888_to_rgb565_bigindian(r, g, b):
    """
    Conversion RGB888 -> RGB565
    Format:
    RRRRR GGGGGG BBBBB
    """
    r5 = (r >> 3) & 0x1F
    g6 = (g >> 2) & 0x3F
    b5 = (b >> 3) & 0x1F

    return (r5 << 11) | (g6 << 5) | b5

def rgb888_to_rgb565_littleindian(r, g, b):

    r5 = (r >> 3) & 0x1F
    g6 = (g >> 2) & 0x3F
    b5 = (b >> 3) & 0x1F

    color = (r5 << 11) | (g6 << 5) | b5

    # ESP32 little endian / LCD byte order
    color = ((color & 0xFF) << 8) | (color >> 8)

    return color

# Conversion pixels
for y in range(h):
    for x in range(w):

        r, g, b = pixels[x, y]

        color = rgb888_to_rgb565_littleindian(r, g, b)

        rgb565.append(color)


name = os.path.splitext(os.path.basename(h_file))[0]


with open(h_file, "w") as f:

    f.write("// Generated from {}\n\n".format(
        os.path.basename(png_file)
    ))

    f.write("#pragma once\n\n")

    f.write("#define {}_WIDTH  {}\n".format(
        name.upper(), w
    ))

    f.write("#define {}_HEIGHT {}\n\n".format(
        name.upper(), h
    ))


    f.write("static const uint16_t {}[] =\n{{\n".format(name))


    for i, color in enumerate(rgb565):

        if i % 8 == 0:
            f.write("    ")

        f.write("0x{:04X},".format(color))

        if i % 8 == 7:
            f.write("\n")
        else:
            f.write(" ")


    f.write("\n};\n")


print("Done.")
print("Size : {}x{}".format(w, h))
print("Pixels:", len(rgb565))
print("Bytes :", len(rgb565) * 2)