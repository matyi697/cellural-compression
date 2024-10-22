from PIL import Image
import io

def bin_to_image(bin_path, output_image_path, size, mode='RGB'):
    with open(bin_path, 'rb') as bin_file:
        img_bytes = bin_file.read()  # Bináris adatok beolvasása
    
    # Kép létrehozása a bináris adatokból
    img = Image.frombytes(mode, size, img_bytes)
    img.save(output_image_path)  # Kép mentése

# Paraméterek
bin_path = 'recovered.bin'  # Bináris fájl neve
output_image_path = 'output_image.png'  # Kimeneti kép neve
size = (800, 600)  # Kép mérete (szélesség, magasság)
mode = 'RGBA'  # Színmód

# Bináris fájl visszaalakítása képre
bin_to_image(bin_path, output_image_path, size, mode)
print(f"Kép visszaalakítva: {output_image_path}")
