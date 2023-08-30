#!/bin/sh
make compile
##sed 's/cc.*/& eeeee/g' ./ec_main.bin
gcc ./TOOLS/ROM/main.c -o rom.out
./rom.out
rm ./rom.out
cp ./rom_main.bin /home/Share
cp ./ec_main.bin /home/Share
cp ./ec_main.bin /mnt/hgfs/Share
cp ./ec_main.hex /home/Share
cp ./ec_main.s19 /home/Share