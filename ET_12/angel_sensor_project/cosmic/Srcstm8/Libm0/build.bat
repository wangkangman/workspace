@echo off
cls
echo.
echo Building the LIBM0.SM8 Machine Library in progress...
echo.
castm8 -vb -i..\..\hstm8 -hmacro.sm8 *.s
clib -c libm0.sm8 *.o
del *.o
echo.
echo Done.
