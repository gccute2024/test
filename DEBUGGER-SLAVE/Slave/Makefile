#############################################################
# Configuration
#############################################################
# default the program is downloaded to ilm
DOWNLOAD ?= flash
RISCV_ARCH?=rv32ec#按照RISC-V架构命名规则，以下指令子集的组合可表示为RV32EC
#RV32架构：riscv-32架构通用寄存器宽度32位
#E：支持16个通用寄存器
#C：支持编码长度为16位的压缩指令，提高代码密度
RISCV_ABI?=ilp32e#int long pointer 都是32bit,不使用硬件浮点数,支持rv32e
DUAL_BOOT_FLAG?=0#双启动开关
CHIP?=AE103 # x替换当前芯片型号
PUBLIC_KEY?=10 # 公钥
USER_RISCV_LIBC_A?=0#0 USER_AE10X_LIBC_A 1 USER_RISCV_LIBC_A
PROGRAM ?= ec_main
PROGRAM_FILE ?= $(PROGRAM).elf
# Default target
# Pointers to various important tools in the toolchain.
RISCV_GCC     := riscv-nuclei-elf-gcc
RISCV_GXX     := riscv-nuclei-elf-g++
RISCV_OBJDUMP := riscv-nuclei-elf-objdump
RISCV_OBJCOPY := riscv-nuclei-elf-objcopy
RISCV_GDB     := riscv-nuclei-elf-gdb
RISCV_READELF := riscv-nuclei-elf-readelf
RISCV_AR      := riscv-nuclei-elf-ar
RISCV_SIZE    := riscv-nuclei-elf-size
DD			  := dd
#############################################################
# This Section is for Software Compilation
#############################################################
#############################################################
TOPDIR 			:= .
KERNEL_DIR 		= $(TOPDIR)/KERNEL
AE10X_DIR 		= $(TOPDIR)/AE10X
CUSTOM_DIR  	= $(TOPDIR)/CUSTOM
HARDWARE_DIR  	= $(TOPDIR)/HARDWARE
TEST_DIR  		= $(TOPDIR)/TEST
ENV_DIR 		= $(AE10X_DIR)/ENV

CINCLUDE 			+= -I $(KERNEL_DIR)/INCLUDE
CINCLUDE 			+= -I $(AE10X_DIR)/INCLUDE
CINCLUDE 			+= -I $(CUSTOM_DIR)/INCLUDE
CINCLUDE 			+= -I $(HARDWARE_DIR)/INCLUDE
CINCLUDE 			+= -I $(TEST_DIR)/INCLUDE
CFLAGS 				+= $(CINCLUDE)

export TOPDIR KERNEL_DIR AE10X_DIR CUSTOM_DIR HARDWARE_DIR TEST_DIR ENV_DIR CFLAGS
##############################################################
#编译
#############################################################

PROGRAM_ELF = $(TOPDIR)/$(PROGRAM_FILE)
PROGRAM_BOOT1 ?= ec_main_1
PROGRAM_BOOT2 ?= ec_main_2
PROGRAM_FILE_BOOT1 ?=$(PROGRAM_BOOT1).elf
PROGRAM_FILE_BOOT2 ?= $(PROGRAM_BOOT2).elf
PROGRAM_ELF_BOOT1 = $(TOPDIR)/$(PROGRAM_FILE_BOOT1)
PROGRAM_ELF_BOOT2 = $(TOPDIR)/$(PROGRAM_FILE_BOOT2)
DD_ELF_BOOT:= if=/dev/zero ibs=4k count=64 | tr "\000" "\377"
DD_ELF_BOOT1:= if=./ec_main_1.bin  of=ec_main.bin seek=0   conv=notrunc
DD_ELF_BOOT2:= if=./ec_main_2.bin  of=ec_main.bin seek=256 conv=notrunc
compile: software 
#	@gcc ./TOOLS/SMx/SM2.c -D$(CHIP) -I/usr/include/openssl/ $(CINCLUDE) -L /usr/lib/x86_64-linux-gnu/ -lcrypto  -o sm2 #-D_DEBUG
#	@cp ./sm2 ./TOOLS/SMx/
	@if [ $(CHIP) == AE103 ];then cp  ./TOOLS/SMx/sm2 ./;./sm2 $(PUBLIC_KEY);rm ./sm2;fi;
	@if [ -e SM3_hash.txt ];then mv SM3_hash.txt ./TOOLS/SMx/;fi;
	@if [ -e SM2_sig.txt ];then mv SM2_sig.txt ./TOOLS/SMx/;fi;
	@if [ -e SM2_user_key.txt ];then mv SM2_user_key.txt ./TOOLS/SMx/;fi;
	@echo CHIP $(CHIP) CODE
	@echo Compiled successfully.
.PHONY: software
software: 
	@echo make -f $(TOPDIR)/common.mk
	@$(MAKE) -f $(TOPDIR)/common.mk --no-print-directory CHIP=$(CHIP) USER_RISCV_LIBC_A=$(USER_RISCV_LIBC_A)   DOWNLOAD=$(DOWNLOAD)   DUAL_BOOT_FLAG=$(DUAL_BOOT_FLAG) TARGET=$(PROGRAM_FILE)
ifeq ($(DUAL_BOOT_FLAG),0)
#riscv-nuclei-elf-objdump -m riscv:rv32 -D -b binary *.bin > *.dump
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF) > $(PROGRAM).dump
	@echo "OBJDUMP $(PROGRAM).dump"
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM).verilog
#	@$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM).vh
	@echo "OBJCOPY $(PROGRAM).verilog"
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF) $(PROGRAM).bin
	@echo "OBJCOPY $(PROGRAM).bin"
	@$(RISCV_OBJCOPY) -I binary -O ihex $(PROGRAM).bin $(PROGRAM).hex
# 	@$(RISCV_OBJCOPY) -O ihex $(PROGRAM_ELF) $(PROGRAM)_0x80000.hex
	@echo "OBJCOPY $(PROGRAM).hex"
	@$(RISCV_OBJCOPY) -I binary -O srec $(PROGRAM).bin $(PROGRAM).s19
# 	@$(RISCV_OBJCOPY) -O srec $(PROGRAM_ELF) $(PROGRAM)_0x80000.s19
	@echo "OBJCOPY $(PROGRAM).s19"
	@$(RISCV_READELF)  -a $(PROGRAM_ELF) > $(PROGRAM).readelf
	@echo "READELF $(PROGRAM).readelf"
else ifeq ($(DUAL_BOOT_FLAG),1)#Dual start
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF_BOOT1) > $(PROGRAM_BOOT1).dump
	@echo OBJDUMP $(PROGRAM_ELF_BOOT1).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF_BOOT1) -O verilog $(PROGRAM_BOOT1).verilog
	@echo OBJCOPY $(PROGRAM_ELF_BOOT1).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF_BOOT1) $(PROGRAM_BOOT1).bin
	@echo OBJCOPY $(PROGRAM_ELF_BOOT1).bin
	@$(RISCV_OBJDUMP) -D $(PROGRAM_ELF_BOOT2) > $(PROGRAM_BOOT2).dump
	@echo OBJDUMP $(PROGRAM_ELF_BOOT2).dump
	@$(RISCV_OBJCOPY) $(PROGRAM_ELF_BOOT2) -O verilog $(PROGRAM_BOOT2).verilog
	@echo OBJCOPY $(PROGRAM_ELF_BOOT2).verilog
	@$(RISCV_OBJCOPY) -O binary $(PROGRAM_ELF_BOOT2) $(PROGRAM_BOOT2).bin
	@echo OBJCOPY $(PROGRAM_ELF_BOOT2).bin
	@$(DD) $(DD_ELF_BOOT)>ec_main.bin
	@$(DD) $(DD_ELF_BOOT1)
	@$(DD) $(DD_ELF_BOOT2)
endif

##############################################################

#############################################################
# This Section is for uploading a program to SPI Flash
#############################################################
RISCV_OPENOCD ?= openocd 
ifeq ($(DOWNLOAD),ilm)
OPENOCDCFG ?= $(AE10X_DIR)/ENV/openocd_hbird_ilm.cfg
else
OPENOCDCFG ?= $(AE10X_DIR)/ENV/openocd_hbird.cfg
endif
OPENOCDARGS += -f $(OPENOCDCFG)
GDB_PORT ?= 3333
#############################################################
# This Section is for launching the debugger
#############################################################

openocd:
	$(RISCV_OPENOCD) $(OPENOCDARGS)

GDBCMDS += -ex "set remotetimeout 240"
GDBCMDS += -ex "set architecture riscv:rv32ec"
GDBCMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
gdb:
	$(RISCV_GDB) ./ec_main.elf  $(GDBCMDS)

GDB_UPLOAD_ARGS ?= --batch
GDB_UPLOAD_CMDS += "$(GDBCMDS)"
GDB_UPLOAD_CMDS += -ex "monitor reset halt"
GDB_UPLOAD_CMDS += -ex "monitor flash protect 0 0 last off"
GDB_UPLOAD_CMDS += -ex "load"
GDB_UPLOAD_CMDS += -ex "monitor resume"
GDB_UPLOAD_CMDS += -ex "monitor shutdown"
GDB_UPLOAD_CMDS += -ex "quit"
upload:
	$(RISCV_OPENOCD) $(OPENOCDARGS) & \
	$(RISCV_GDB) ./ec_main.elf $(GDB_UPLOAD_ARGS) $(GDB_UPLOAD_CMDS) && \

#############################################################
# Prints help message
#############################################################
.PHONY: help
help:
	@echo "  Makefile targets:"
	@echo " "
	@echo "     compile :"
	@echo "        	Build a software program to load with the debugger."
	@echo " 	upload :"
	@echo "    		Launch OpenOCD to flash your program to the on-board Flash."
	@echo " 	openocd :"
	@echo " 	gdb :"
	@echo "     	Launch OpenOCD or GDB seperately. Allows Ctrl-C to halt running programs."
	@echo " 	clean :"
	@echo "     	clean compile build software program file."

.PHONY:clean
clean: 
	@$(MAKE) -f $(TOPDIR)/common.mk --no-print-directory DOWNLOAD=$(DOWNLOAD) clean
	@rm -f $(shell find -name "*.verilog")
	@echo RM -F *.VERILOG
	@rm -f $(shell find -name "*.readelf")
	@echo RM -F *.READELF
	@rm -f $(shell find -name "*.dump")
	@echo RM -F *.DUMP
	@rm -f $(shell find -name "*.bin")
	@echo RM -F *.BIN
	@rm -f $(shell find -name "*.hex")
	@echo RM -F *.HEX
	@rm -f $(shell find -name "*.s19")
	@echo RM -F *.S19
	@rm -f $(shell find -name "*.map")
	@echo RM -F *.MAP
	@rm -f $(shell find -name "*.elf")
	@echo RM -F *.ELF
	@rm -f $(shell find -name "*.o")
	@echo RM -F *.O
	@rm -f $(shell find -name "*.d")
	@echo RM -F *.D
	