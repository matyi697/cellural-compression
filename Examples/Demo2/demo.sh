python Examples/image-tools/image_to_binary.py

gcc -o main main.c
./main -c image_data.bin compressed.bin
./main -d compressed.bin recovered.bin

python Examples/image-tools/binary_to_image.py