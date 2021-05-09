VUT FIT MITAI
PDS 2021/2022
Project: Identification of Mobile Traffic using TLS Fingerprinting
Author: Vladimir Dusek
Login: xdusek27

Obsah
- pcap_train/
    - Pcap soubory pro trenovani detektoru
- pcap_test/
    - Pcap soubory pro testovani detektoru
- src/
    - Python skripty pro trenovani a testovani detektoru
- train.sh
    - Bash skript jako wrapper pro spusteni trenovani detektoru
- test.sh
    - Bash skript jako wrapper pro spusteni testovani detektoru
- whitelist.json
    - Whitelist aplikaci a prislusnych SNI
- features.json
    - Priznaky pro natrenovane aplikace
- features_verbose.json
    - Priznaky pro natrenovane aplikace ve verbose formatu
- results.json
    - Vysledky testovani detektoru

Poznamka
Python skripty jsou psane pro Python3. Z knihoven tretich stran
je vyzadovan pouze Pyshark (pypi.org/project/pyshark). Skripty
byly spousteny s verzi Pythonu 3.7.
