VUT FIT MITAI
FLP 2019/2020
Logic project
Variant: Turing machine
Author: Vladimir Dusek
Login: xdusek27

Poznamka
=============================
Reseni ma jeden nedostatek, ktereho jsem si vedom.
Konecna konfigurace se hleda aplikaci pravidel do hloubky.
To muze byt problem v pripade, pokud mame nedeterministicky
stav a jedna posloupnost pravidel nas zavede do smycky a
jina do koncoveho stavu. V takovem pripade je mozne, ze program
zacne nejprve vykonavat vetev se smyckou a zacykli se.
Ackoliv by existovala moznost, jak se do koncoveho stavu dostat,
nikdy ji nenalezne. Problem plyne z neoptimalnosti DFS.
V zadani vsak neni konkretni pozadavek na optimalnost a zmena
algoritmu na BFS by vyzadovala znacny refaktor zdrojoveho kodu,
do ktereho jsem se jiz nepoustel.

Struktura archivu
=============================
Readme: readme.txt
Zdrojovy soubor: flp20-log.pl
Preklad: Makefile
Testy: test.sh, test/

Ovladani projektu pomoci make
=============================
Preklad:
$ make

Spusteni projektu:
$ make run

Spusteni testu:
$ make test

Smazani generovanych souboru prekladem:
$ make clean

Zabaleni projektu k odevzdani:
$ make zip

