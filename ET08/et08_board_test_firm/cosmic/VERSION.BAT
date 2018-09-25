@echo off
cls
echo.
echo .\version and date of Compiler and utilities in: VERSION.TXT...
echo -------------------------------->version.txt
echo STM8 COSMIC C Compiler>>version.txt
echo Version: 4.2.4 Date: 19 Dec 2007>>version.txt
echo -------------------------------->>version.txt
.\cxstm8 -vers 2>>version.txt
.\cpstm8 -vers 2>>version.txt
.\cgstm8 -vers 2>>version.txt
.\costm8 -vers 2>>version.txt
.\castm8 -vers 2>>version.txt
.\clnk -vers 2>>version.txt
.\chex -vers 2>>version.txt
.\clabs -vers 2>>version.txt
.\clib -vers 2>>version.txt
.\clst -vers 2>>version.txt
.\cobj -vers 2>>version.txt
.\cprd -vers 2>>version.txt
.\cvdwarf -vers 2>>version.txt
.\cv695 -vers 2>>version.txt
echo Done !
echo.
type version.txt
echo on
