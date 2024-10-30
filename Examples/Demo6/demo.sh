gcc -o main main.c
gcc -o RLE RLE.c
./main data/input.bin -c 2 stringThing.txt "pyhon huffman" data/output.bin
./main data/output.bin -d 2 stringThing.txt "pyhon huffman" data/recovered.bin