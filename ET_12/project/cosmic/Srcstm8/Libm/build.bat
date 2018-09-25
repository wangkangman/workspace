@echo off
cls
echo.
echo Building the LIBM.SM8 Machine Library in progress...
echo.
castm8 -vb -i..\..\hstm8 -hmacro.sm8 *.s
clib -c libm.sm8 *.o
del *.o
echo.
echo Done.
