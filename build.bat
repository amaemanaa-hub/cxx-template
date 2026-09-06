@echo off
cd /d C:\Users\bakhr\engine\build
cmake --build . --config Release
cd Release
engine.exe
pause