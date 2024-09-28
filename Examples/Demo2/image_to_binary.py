from PIL import Image

def image_to_bin(image_path, bin_path):
    with Image.open(image_path) as img:
        img_bytes = img.tobytes()  # Kép konvertálása bájtokba
    
    with open(bin_path, 'wb') as bin_file:
        bin_file.write(img_bytes)  # Írás bináris fájlba

# Paraméterek
image_path = 'input_image.png'  # Bemeneti kép
bin_path = 'image_data.bin'  # Bináris fájl neve

# Kép konvertálása bináris fájlba
image_to_bin(image_path, bin_path)
print(f"Kép bináris formában elmentve: {bin_path}")
