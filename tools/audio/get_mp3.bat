@echo off
set "inputFile=%~1"
set "outputFile=%~dpn1.mp3"
set "currentPath=%~dp0"

"%currentPath%ffmpeg" -i "%inputFile%" -vn -ar 44100 -ac 2 -b:a 192k "%outputFile%"
echo Conversion complete
pause