# Run-Length Encoding (RLE)

## Mi az a Run-Length Encoding?

A Run-Length Encoding (RLE) egy egyszerű tömörítési algoritmus, amely egy adatfolyam ismétlődő elemeit hatékonyabban reprezentálja. Az algoritmus azonosítja az egymás után következő azonos karaktereket vagy elemeket, és helyettesíti őket egyetlen példánnyal és az ismétlődések számával.

### Hogyan működik?

Az RLE algoritmus célja a redundáns adatok minimalizálása azáltal, hogy az egymás után ismétlődő karakterek sorozatát egyetlen karakterrel és egy számmal helyettesíti, amely az ismétlődések számát jelöli.

**Példa:**

- Bemeneti adat: `AAAAABBBCCDAA`
- RLE tömörített adat: `5A3B2C1D2A`

Az eredeti adatsorban az "A" karakter 5-ször ismétlődik egymás után, ezt követi 3 "B", majd 2 "C", 1 "D", és végül 2 "A". Az RLE tehát jelentős helymegtakarítást eredményezhet hosszú ismétlődések esetén.

### Előnyök

- Egyszerű és gyorsan implementálható.
- Hatékony, ha az adat sok ismétlődő elemet tartalmaz.

### Hátrányok

- Ha az adatokban kevés az ismétlődés, a tömörítés nem hatékony, és akár növelheti is az adatok méretét.
- Nem minden típusú adat esetén ideális, főleg ha a változékonyság nagy.

## Demo

Ez a demo egy egyserű bináris filet tömörít le RLE segítségével. A demo.sh szkript fordítja és futtatja is a main.c filet, majd az alábbi argumentumokat adja meg:
-   ./main -c input.bin compressed.bin
-   ./main -d compressed.bin recovered.bin
A végén végzünk egy file komparálást a recovered.bin és az input.bin között.