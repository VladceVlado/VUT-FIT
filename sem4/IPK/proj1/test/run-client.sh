#!/bin/sh

# Info (2 body)
echo "Info (2 body)"
./ipk-client -h localhost -p 51515 -n rysavy
./ipk-client -h localhost -p 51515 -n biswas
./ipk-client -h localhost -p 51515 -n xzwier00
./ipk-client -h localhost -p 51515 -n _dhcp
echo

# Folder (2 body)
echo "Folder (2 body)"
./ipk-client -h localhost -p 51515 -f rysavy
./ipk-client -h localhost -p 51515 -f qpovolny
./ipk-client -h localhost -p 51515 -f zizkaj
./ipk-client -h localhost -p 51515 -f avahi
echo

# List (2 body)
echo "List (2 body)"
./ipk-client -h localhost -p 51515 -l rysavy
./ipk-client -h localhost -p 51515 -l iegorova
./ipk-client -h localhost -p 51515 -l xznebe00
./ipk-client -h localhost -p 51515 -l xxx
echo

# NF01 (1 bod)
echo "NF01 (1 bod)"
./ipk-client -h localhost -p 51515 -n r_savy
echo

# NF02 (1 bod)
echo "NF02 (1 bod)"
./ipk-client -h localhost -p 51515 -f rys_vy
echo

# PL01 (2 body)
echo "PL01 (1 bod)"
./ipk-client -h localhost -p 51515 -l x
echo

# PL02 (2 body)
echo "PL02 (1 bod)"
./ipk-client -h localhost -p 51515 -l
echo