@echo off
cls
echo.
echo Building LIBIS.SM8 and LIBISL.SM8 libraries, MODS model in progress ...
echo.
cxstm8 -gna +mods *.c
castm8 -i..\..\..\hstm8 -hmacro.sm8 *1.s
del cksum16x0.o
clib -c libis.sm8 *.o
cxstm8 -gna +modsl *.c *1.s
castm8 -i..\..\..\hstm8 -hmacro.sm8 *1.s
del cksum16x0.o
clib -c libisl.sm8 *.o
del *.o
echo.
echo Building MODS 64K Memory model libraries in progress ...
echo.
cxstm8 -gna +mods0 *.c
castm8 -i..\..\..\hstm8 -hmacro.sm8 *0.s
clib -c libis0.sm8 *.o
cxstm8 -gna +modsl0 *.c
clib -c libisl0.sm8 *.o
del *.o
echo.
echo Done.
