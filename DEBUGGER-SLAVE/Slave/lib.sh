#!/bin/sh
./make.sh
rm AE10X/libAE10X.a 
riscv-nuclei-elf-gcc-ar -crsv  ./AE10X/libAE10X.a ./AE10X/*.o
./clean.sh
cp ./ ../../slave_cpy -r
#cp ../../slave_cpy/AE10X/AE_INIT.c ../../slave_cpy/ 
rm -f ../../slave_cpy/AE10X/*.c ../../slave_cpy/AE10X/*.S
rm -rf ../../slave_cpy/HARDWARE ../../slave_cpy/TEST
rm -rf ../../slave_cpy/lib.sh ../../slave_cpy/git.sh ../../slave_cpy/SMx_openssl.sh
rm -rf ../../slave_cpy/*.csv ../../slave_cpy/README.md
rm -rf ../../slave_cpy/TOOLS/*
rm -rf ../../slave_cpy/KERNEL/*_OWI* ../../slave_cpy/KERNEL/INCLUDE/*_OWI* 
rm -rf ../../slave_cpy/KERNEL/*_SMx* ../../slave_cpy/KERNEL/INCLUDE/*_SMx* 
rm -rf ../../slave_cpy/.s* ../../slave_cpy/.v* ../../slave_cpy/.c* ../../slave_cpy/.p*
#mv ../../slave_cpy/AE_INIT.c ../../slave_cpy/AE10X/