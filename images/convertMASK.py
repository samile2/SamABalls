from PIL import Image
import sys
import os

if len(sys.argv) != 3:
    print("Usage:")
    print("    python png_alpha_to_mask.py sprite.png mask.h")
    sys.exit(1)

png_file = sys.argv[1]
h_file = sys.argv[2]

img = Image.open(png_file).convert("RGBA")

w, h = img.size
pixels = img.load()

bytes_out = []

for y in range(h):

    byte = 0
    bit = 7

    for x in range(w):

        r, g, b, a = pixels[x, y]

        # Visible si alpha > 128
        if a >= 128:
            byte |= (1 << bit)

        bit -= 1

        if bit < 0:
            bytes_out.append(byte)
            byte = 0
            bit = 7

    # largeur non multiple de 8
    if bit != 7:
        bytes_out.append(byte)


name = os.path.splitext(os.path.basename(h_file))[0]


with open(h_file, "w") as f:

    f.write("// Generated from {}\n\n".format(
        os.path.basename(png_file)
    ))

    f.write("#pragma once\n\n")

    f.write("#define {}_WIDTH {}\n".format(
        name.upper(), w
    ))

    f.write("#define {}_HEIGHT {}\n\n".format(
        name.upper(), h
    ))

    f.write("static const uint8_t {}[] =\n{{\n".format(name))

    for i, b in enumerate(bytes_out):

        if i % 12 == 0:
            f.write("    ")

        f.write("0x{:02X},".format(b))

        if i % 12 == 11:
            f.write("\n")
        else:
            f.write(" ")

    f.write("\n};\n")


print("Done.")
print("Size:", w, "x", h)
print("Mask bytes:", len(bytes_out))