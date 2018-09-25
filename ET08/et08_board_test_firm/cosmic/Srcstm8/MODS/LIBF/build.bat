@echo off
cls
echo.
echo Building LIBFS.SM8 and LIBFSL.SM8 libraries, MODS model in progress ...
echo.
cxstm8 -gna +mods *.c
castm8 -i..\..\..\hstm8 -hmacro.sm8 *1.s
clib -c libfs.sm8 *.o
cxstm8 -gna +modsl *.c
clib -c libfsl.sm8 *.o
del *.o
echo.
echo Building MODS 64K Memory model libraries in progress ...
echo.
cxstm8 -gna +mods0 *.c
castm8 -i..\..\..\hstm8 -hmacro.sm8 *0.s
clib -c libfs0.sm8 *.o
cxstm8 -gna +modsl0 *.c
clib -c libfsl0.sm8 *.o
del *.o
echo.
echo Done.
