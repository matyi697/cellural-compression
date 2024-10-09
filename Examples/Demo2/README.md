## Projekt leírása

Ez a projekt bemutatja, hogyan lehet egy PNG képet bináris fájlba konvertálni, azt Run-Length Encoding (RLE) algoritmussal tömöríteni, majd a tömörített adatokat visszaállítani és újra PNG formátumba építeni.

A folyamat a következő lépéseket tartalmazza:
1. A PNG kép bináris fájlformátummá alakítása.
2. Az így kapott bináris fájl tömörítése az RLE algoritmussal.
3. Az RLE tömörített adat visszafejtése eredeti bináris formátumra.
4. A visszanyert bináris adatokból egy új PNG fájl létrehozása.

## Fájlok

- **input_image.png**: Az eredeti PNG kép, amelyet a demó használ.
- **image_data.bin**: Az input PNG kép bináris formátuma.
- **compressed.bin**: A bináris fájl RLE tömörített változata.
- **recovered.bin**: Az RLE tömörítés visszafejtett bináris fájlja.
- **output_image.png**: A végleges PNG fájl, amelyet a visszaállított bináris adatokból készítünk.

## Használat

### 1. PNG kép konvertálása bin fájlba

python image_to_binary.py

### 2. Bináris fájl RLE kódolása

./main -c image_data.bin compressed.bin

### 3. Bináris fájl visszaállítása

./main -d compressed.bin recovered.bin

### 4. Kép visszaállítása a bin fájlból

python binary_to_image.py
