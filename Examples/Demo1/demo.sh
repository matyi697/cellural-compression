echo "Ez a demo be es kicsomagol RLE segitsegevel:"
gcc -o main main.c
./main -c input.bin compressed.bin
./main -d compressed.bin recovered.bin

if cmp -s "input.bin" "recovered.bin"; then
    echo "A ket file megegyezik!"
else
    echo "A ket file kulonbozik!"
fi