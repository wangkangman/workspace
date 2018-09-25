
for /r .\ %%i in (*.hex) do hex2bin.exe -m 200 -p 00 %%i

for /r .\ %%i in (*.bin) do checksum.exe %%i

pause