gcc -o main main.c
gcc -o RLE RLE.c
python image_to_binary.py data/image.png input.bin
./main input.bin -t rules/stringThing.txt 1 test "./RLE"