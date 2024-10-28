# Reverzibilis Sejtautomata Demo

Ez a projekt egy reverzibilis sejtautomata modellt mutat be C nyelven. A modell célja, hogy a reverzibilitást demonstrálja egy ASCII képen végrehajtott lépések előre- és visszaléptetésével, így visszakapva az eredeti mintát.

## Leírás

A program egy  megadott ASCII karakterekből álló mintát olvas be egy `.txt` fileból, majd végrehajt rajta egy adott számú lépést a reverzibilis sejtautomata szabályai szerint. Ezután ugyanennyi lépést visszalép, és a kapott eredményt kiírja, így bemutatva, hogy a folyamat valóban reverzibilis, hiszen az eredeti képet kapjuk vissza.

## Használat

1. **Bemeneti fájl készítése**: Készíts egy `.txt` filet egy ASCII művészeti képpel, amely a sejtautomata bemenete lesz.
2. **Ruleset file készítése**: A modellnek szükséges egy sejtauomata szabályrendszer amit számpárokként olvas fel egy `.txt` fileból
3. **Fordítás és futtatás**: Fordítsd és futtasd a programot a következő parancsokkal:

   ```bash
   gcc -o automata automata.c
   ./automata message.txt rule.txt
