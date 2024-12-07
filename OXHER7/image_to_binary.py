import sys
from PIL import Image

def image_to_bin(image_path, bin_path):
    with Image.open(image_path) as img:
        img_bytes = img.tobytes()

    with open(bin_path, 'wb') as bin_file:
        bin_file.write(img_bytes)

if len(sys.argv) != 3:
    print("Használat: python script.py [bemeneti kép] [kimeneti bin fájl]")
    sys.exit(1)

image_path = sys.argv[1]
bin_path = sys.argv[2]

image_to_bin(image_path, bin_path)
print(f"Kép bináris formában elmentve: {bin_path}")
