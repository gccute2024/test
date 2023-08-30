# See LICENSE for license details.
TARGET ?= ec_main.elf			# elf file name ec_main
DOWNLOAD ?= flash					#download flash
SIZE:=riscv-nuclei-elf-size
CC:=riscv-nuclei-elf-gcc 
RISCV_ARCH?=rv32ec
RISCV_ABI?=ilp32e

ENV_DIR = $(AE10X_DIR)/ENV
ifeq ($(DUAL_BOOT_FLAG),0)
ifeq ($(DOWNLOAD),flash) 
LINKER_SCRIPT := $(ENV_DIR)/link_flash.lds
else ifeq ($(DOWNLOAD),ilm) 
LINKER_SCRIPT := $(ENV_DIR)/link_ilm.lds
endif
LDFLAGS += -T $(LINKER_SCRIPT)  -nostartfiles  -Wl,--check-sections#      函数检查 #-Wl,--gc-sections #不链接未用函数,会导致中断异常
else 
LINKER_SCRIPT1 := $(ENV_DIR)/link_flash.lds
LINKER_SCRIPT2 := $(ENV_DIR)/link_flash2.lds
LDFLAGS1 += -T $(LINKER_SCRIPT1)  -nostartfiles  -Wl,--check-sections
LDFLAGS2 += -T $(LINKER_SCRIPT2)  -nostartfiles  -Wl,--check-sections
LINK_DEPS1 			+= $(LINKER_SCRIPT1)
LINK_DEPS2 			+= $(LINKER_SCRIPT2)
endif

LDFLAGS += -L$(ENV_DIR)
# download has three option:  flashxip, flash, ilm
include $(AE10X_DIR)/Makefile
include $(KERNEL_DIR)/Makefile
include $(CUSTOM_DIR)/Makefile
include $(HARDWARE_DIR)/Makefile
include $(TEST_DIR)/Makefile

TARGET_BOOT1 = ec_main_1.elf
TARGET_BOOT2 = ec_main_2.elf
C_SRCS_BOOT	 += $(AE10X_DIR)/AE_INIT.c
C_SRCS_BOOT1 += $(AE10X_DIR)/AE_INIT.c
C_SRCS_BOOT2 += $(AE10X_DIR)/AE_INIT.c
C_SRCS_BOOT	 += $(C_SRCS)
C_SRCS_BOOT1 += $(C_SRCS)
C_SRCS_BOOT2 += $(C_SRCS)



ASM_OBJS 		:= $(ASM_SRCS:.S=.o)
C_OBJS 			:= $(C_SRCS_BOOT:.c=.o)
LINK_OBJS 		+= $(ASM_OBJS) $(C_OBJS)

ASM_OBJS_BOOT1		:= $(ASM_OBJS:.o=1.o)
C_OBJS_BOOT1		:= $(C_SRCS_BOOT1:.c=1.o)
LINK_OBJS_BOOT1 	+= $(ASM_OBJS_BOOT1) $(C_OBJS_BOOT1) 
ASM_OBJS_BOOT2		:= $(ASM_OBJS:.o=2.o)
C_OBJS_BOOT2		:= $(C_SRCS_BOOT2:.c=2.o)
LINK_OBJS_BOOT2 	+= $(ASM_OBJS_BOOT2) $(C_OBJS_BOOT2)

LINK_DEPS 			+= $(LINKER_SCRIPT)
CLEAN_OBJS 			+=  $(LINK_OBJS) 
CLEAN_OBJS 			+= $(TARGET_BOOT1) 
CLEAN_OBJS 			+= $(TARGET_BOOT2) 

ifdef CHIP
CFLAGS += -D$(CHIP)
endif
CFLAGS +=  -flto -Os -msave-restore  -fno-unroll-loops
CFLAGS += -g
CFLAGS += -march=$(RISCV_ARCH)
CFLAGS += -mabi=$(RISCV_ABI)
CFLAGS += -mcmodel=medlow 
CFLAGS += -ffunction-sections -fdata-sections -fno-common
CFLAGS += -D$(DOWNLOAD)
#CFLAGS +=  -std=gnu99 -std=c99

ifeq ($(USER_RISCV_LIBC_A),0) 
CFLAGS += -fno-builtin-printf 
CFLAGS += -fno-builtin-memset
CFLAGS += -DUSER_AE10X_LIBC_A 
LDFLAGS += -L$(AE10X_DIR) -lAE10X
LDFLAGS += -nostdlib  -nodefaultlibs

endif
ifeq ($(USER_RISCV_LIBC_A),1) 
CFLAGS += -DUSER_RISCV_LIBC_A 
CFLAGS += -D_COMPILING_NEWLIB
LDFLAGS += --specs=nano.specs --specs=nosys.specs
endif
LINK_OBJS_WITHOUT_COMMENT := $(notdir $(LINK_OBJS))
dep_files := $(foreach f,$(LINK_OBJS_WITHOUT_COMMENT), $(f).d)
dep_files := $(wildcard $(dep_files))


CFLAGS1 += -DROM_QIDONG
CFLAGS1 += -DDUBLE_FIRMWARE1
CFLAGS1 += $(CFLAGS)
CFLAGS2 += -DDUBLE_FIRMWARE2
CFLAGS2 += $(CFLAGS)

ifneq ($(dep_files),)
  include $(dep_files)
endif
LDFLAGS += -Wl,-Map=ec_main.map
LDFLAGS1 += $(LDFLAGS)
LDFLAGS2 += $(LDFLAGS)

.PHONY: Target
#########################
ifeq ($(DUAL_BOOT_FLAG),0)
Target: $(TARGET)


$(TARGET): $(LINK_OBJS) $(LINK_DEPS)
	@$(CC) $(CFLAGS) $(LINK_OBJS) -o $@ $(LDFLAGS)
	@echo CC "    "$@
	@$(SIZE) $@

dep_file =$(@:.o=.d)

$(ASM_OBJS): %.o: %.S $(HEADERS)
	@$(CC) $(CFLAGS) -Wp,-MD,$(dep_file) -c -o $@ $<
	@echo CC 	$@

$(C_OBJS): %.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -include sys/cdefs.h -Wp,-MD,$(dep_file) -c -o $@ $<
	@echo CC 	$@

#########################

#########################
else ifeq ($(DUAL_BOOT_FLAG),1)

Target: $(TARGET_BOOT1) $(TARGET_BOOT2)

$(TARGET_BOOT1): $(LINK_OBJS_BOOT1) $(LINK_DEPS1)
	@$(CC) $(CFLAGS1) $(LINK_OBJS_BOOT1) -o $@ $(LDFLAGS1)
	@echo CC "    "$@
	@$(SIZE) $@

$(TARGET_BOOT2): $(LINK_OBJS_BOOT2) $(LINK_DEPS2)
	@$(CC) $(CFLAGS2) $(LINK_OBJS_BOOT2) -o $@ $(LDFLAGS2)
	@echo CC "    "$@
	@$(SIZE) $@

dep_file =$(@:.o=.d)

$(ASM_OBJS_BOOT1): %1.o: %.S $(HEADERS)
	@$(CC) $(CFLAGS1) -Wp,-MD,$(dep_file) -c -o $@ $<
	@echo CC "    "$(subst $(shell pwd)/,,$@)

$(C_OBJS_BOOT1): %1.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS1) -include sys/cdefs.h -Wp,-MD,$(dep_file) -c -o $@ $< 
	@echo CC "    "$(subst $(shell pwd)/,,$@)

$(ASM_OBJS_BOOT2): %2.o: %.S $(HEADERS)
	@$(CC) $(CFLAGS2) -Wp,-MD,$(dep_file) -c -o $@ $<
	@echo CC "    "$(subst $(shell pwd)/,,$@)
$(C_OBJS_BOOT2): %2.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS2) -include sys/cdefs.h -Wp,-MD,$(dep_file) -c -o $@ $<
	@echo CC "    "$(subst $(shell pwd)/,,$@)

endif
#########################

.PHONY: clean
clean:
	@rm -f $(CLEAN_OBJS)
	@echo RM -F CLEAN_OBJS

