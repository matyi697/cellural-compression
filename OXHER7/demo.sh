gcc -o main main.c
gcc -o RLE RLE.c
python image_to_binary.py image.png input.bin
./main input.bin stringThing.txt 10 test "./RLE"
