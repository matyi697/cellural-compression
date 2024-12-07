from PIL import Image
import io

def bin_to_image(bin_path, output_image_path, size, mode='RGB'):
    with open(bin_path, 'rb') as bin_file:
        img_bytes = bin_file.read()
    
    img = Image.frombytes(mode, size, img_bytes)
    img.save(output_image_path) 

bin_path = 'recovered.bin' 
output_image_path = 'output_image.png'
size = (800, 600) 
mode = 'RGBA'

bin_to_image(bin_path, output_image_path, size, mode)
print(f"Kép visszaalakítva: {output_image_path}")
