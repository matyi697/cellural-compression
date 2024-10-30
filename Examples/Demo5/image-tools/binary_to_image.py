from PIL import Image
import sys

def bin_to_image(bin_path, output_image_path, size, mode='RGB'):
    with open(bin_path, 'rb') as bin_file:
        img_bytes = bin_file.read()  # Bináris adatok beolvasása
    
    # Kép létrehozása a bináris adatokból
    img = Image.frombytes(mode, size, img_bytes)
    img.save(output_image_path)  # Kép mentése

def main():
    # Ellenőrzi az argumentumok számát
    if len(sys.argv) < 5:
        print("Használat: python script.py <bemeneti bin fájl> <kimeneti kép fájl> <szélesség> <magasság> [színmód]")
        sys.exit(1)

    # Parancssori argumentumok beolvasása
    bin_path = sys.argv[1]
    output_image_path = sys.argv[2]
    width = int(sys.argv[3])
    height = int(sys.argv[4])
    size = (width, height)

    # Opcionális színmód argumentum (alapértelmezett: RGB)
    mode = sys.argv[5] if len(sys.argv) > 5 else 'RGB'

    # Kép visszaalakítása bináris fájlból
    bin_to_image(bin_path, output_image_path, size, mode)
    print(f"Kép visszaalakítva: {output_image_path}")

if __name__ == "__main__":
    main()
