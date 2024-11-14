gcc -o main main.c
gcc -o RLE RLE.c
./main data/input.bin -c 1 stringThing.txt "python huffman.py" data/output.bin
./main data/output.bin -d 1 stringThing.txt "python huffman.py" data/recovered.bin