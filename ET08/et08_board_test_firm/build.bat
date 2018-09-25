@echo off


md  output
set PRJS=ET08_test_firm
set TOOL=cosmic
set INCS=-isource\etapps -isource\globals -isource\drivers -isource\hardware -isource\usmart -i%TOOL%\Hstm8
set LIBS=-l%TOOL%\Lib
set OBJS=-cooutput\
set LINK=source\prjlnks\link.scat
set SRCS=source\globals\global.c ^
         source\drivers\adcsmp.c ^
         source\drivers\clkrst.c ^
         source\drivers\eeprom.c ^
         source\drivers\keysmp.c ^
         source\drivers\ledseg.c ^
         source\drivers\motors.c ^
         source\drivers\remote.c ^
         source\drivers\timwdt.c ^
         source\drivers\uartct.c ^
         source\startup\starts.c ^
         source\usmart\usmart.c ^
         source\usmart\usmart_config.c ^
         source\usmart\usmart_str.c ^
         source\etapps\subs.c ^
         source\etapps\main.c

del /q output\*.*
%TOOL%\cxstm8 +modsl0 -pp %INCS% %OBJS% %SRCS%
%TOOL%\clnk %LIBS% -ooutput\%PRJS%.sm8 -moutput\%PRJS%.map %LINK%
%TOOL%\chex -fi -o output\%PRJS%.hex output\%PRJS%.sm8
%TOOL%\mapinfo output\%PRJS%.map
@echo.
pause
