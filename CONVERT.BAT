@echo off
setlocal enabledelayedexpansion

Set /P NAME="������ ��� �����: "

:: ������� �������� ��⠫���� � �����⠫���� ��襩 ����
mkdir "%NAME%"
mkdir "%NAME%\LICDIR"

xcopy "PS3_GAME\TROPDIR" "%NAME%\TROPDIR" /s /i
xcopy "PS3_GAME\*.*" "%NAME%\*.*"
xcopy "PS3_GAME\USRDIR\*.sdat" "%NAME%\USRDIR\*.sdat" /e

:: ������� ᯨ᮪ 䠩��� � ��⠫���� ����� USRDIR. �㦭� ��� make_npdata
dir /b /s /a:-d "PS3_GAME\USRDIR\">list.txt

type list.txt | findstr /i /v ".sdat EBOOT.BIN" > temp.txt
del list.txt
rename temp.txt list.txt

Set infile=list.txt
Set find=%CD%\PS3_GAME\
Set replace=

for /F "tokens=*" %%n in (!infile!) do (
set LINE=%%n
set TMPR=!LINE:%find%=%replace%!
Echo !TMPR!>>TMP.TXT
)
move TMP.TXT %infile%

:: ��������㥬 � sdat �� 䠩�� �� ����� USRDIR
@echo on
for /f "tokens=*" %%B in (!infile!) do make_npdata -e "PS3_GAME\%%~B" "%NAME%\%%~B" 0 1 3 0 16

:: ������� EDAT
make_npdata -e "PS3_GAME\LICDIR\LIC.DAT" "%NAME%\LICDIR\LIC.EDAT" 1 1 3 0 16 3 00 EP9000-%NAME%_00-0000000000000001 1
@echo off

del /q /f %infile%

echo ===============================================================================
echo                                    END 
echo ===============================================================================

PAUSE
