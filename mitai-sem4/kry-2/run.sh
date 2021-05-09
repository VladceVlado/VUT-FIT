#!/usr/bin/env bash

# VUT FIT MITAI
# KRY 2021/2022
# Project: Elliptic-curve cryptography
# Author: Vladimir Dusek
# Login: xdusek27

cd src/ || exit

echo "Executing example from the assignment..."
make decipher publicKey="(0x52910A011565810BE90D03A299CB55851BAB33236B7459B21DB82B9F5C1874FE,0xE3D03339F660528D511C2B1865BCDFD105490FFC4C597233DD2B2504CA42A562)"
echo

echo "================================================================================"
echo

echo "Executing example from the Discord..."
make decipher publicKey="(0x6863aeecb4af52b068aed5c8e3bf2d7030b7c46e16609ce2ba2af1c9c54c116, 0xe69b3f636feb14eb303579a28477e0e8880fb697580c2092c57007c79cb000bd)"
echo

cd ../
