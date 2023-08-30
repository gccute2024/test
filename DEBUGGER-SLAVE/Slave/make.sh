#!/bin/sh
make compile
##sed 's/cc.*/& eeeee/g' ./ec_main.bin
cp ./ec_main.bin /home/Share
cp ./ec_main.bin /mnt/hgfs/Share
cp ./ec_main.hex /home/Share
cp ./ec_main.s19 /home/Share
#cp ./ec_main.hex /mnt/hgfs/Share
