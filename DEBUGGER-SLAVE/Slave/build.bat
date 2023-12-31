@echo off
REM ***********************************************************************************************************
REM	Setting
REM ***********************************************************************************************************
REM -----------------------------------------------------------------------------------------------------------
REM (64, 96, 128, and 160 are valid)
SET DUAL_BOOT_FLAG=0

	path=D:\Program File\nuclei_riscv_newlibc_prebuilt_win32_2020.07\gcc\bin;
REM -----------------------------------------------------------------------------------------------------------


REM ***********************************************************************************************************
REM	Parameter
REM ***********************************************************************************************************
REM -----------------------------------------------------------------------------------------------------------
IF  "%1" == ""     GOTO BUILD
IF  "%1" == "all"     GOTO BUILD
IF  "%1" == "ALL"     GOTO BUILD
IF  "%1" == "?"    GOTO OPTIONS

IF  "%1" == "clear"   GOTO clear
IF  "%1" == "CLEAR"   GOTO clear

REM ***********************************************************************************************************
REM	makefile
REM ***********************************************************************************************************
:BUILD
    make compile PROGRAM=ec_proj KERNEL=n101 DOWNLOAD=flash PFLOAT=0 DUAL_BOOT_FLAG=0
    riscv-nuclei-elf-objcopy -O binary ./ec_main  ./ec_main.bin
GOTO done
:OPTIONS
cls
ECHO    ********************************************************************
ECHO    *    Spksilicon Embedded Controller Firmware Build Process         *
ECHO    *    Solutions for Embedded Controller Applications.               *
ECHO    ********************************************************************
ECHO.    
ECHO.
GOTO done
REM ***********************************************************************************************************
REM	To clear obj, lst, and bin files.
REM ***********************************************************************************************************
:clear
del /q .\AE10X\*.o*
del /q .\KERNEL\*.o*
del /q .\HARDWARE\*.o*
del /q .\CUSTOM\*.o*
del /q .\TEST\*.o*
del /q .\ec_main*
GOTO done
::-----------------------------------------------------------
:: Done
::-----------------------------------------------------------
:done
::pause
@echo on