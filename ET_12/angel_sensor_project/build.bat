@echo off
echo complie start~

set PRJS=ET12_FIRM
set TOOL=cosmic
set INCS=-isource\etapps -isource\globals -isource\drivers -isource\hardware -icosmic\Hstm8
set LIBS=-lcosmic\Lib
set OBJS=-cooutput\
set LINK=source\prjlnks\link.scat
set SRCS=source\globals\global.c ^
         source\drivers\adcsmp.c ^
         source\drivers\clkrst.c ^
         source\drivers\eeprom.c ^
         source\drivers\keysmp.c ^
         source\drivers\ledseg.c ^
         source\drivers\motors.c ^
         source\drivers\powers.c ^
         source\drivers\remote.c ^
         source\drivers\timwdt.c ^
         source\drivers\uartct.c ^
         source\startup\starts.c ^
         source\etapps\subs.c ^
         source\etapps\main.c

del /q output\*.*
%TOOL%\cxstm8 +modsl0 -pp %INCS% %OBJS% %SRCS%
%TOOL%\clnk %LIBS% -ooutput\%PRJS%.sm8 -moutput\%PRJS%.map %LINK%
%TOOL%\chex -fi -o output\%PRJS%.hex output\%PRJS%.sm8
%TOOL%\mapinfo output\%PRJS%.map
@echo.
pause
