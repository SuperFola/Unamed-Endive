REM game launcher (for the source)
cd src/
py -3.4 main.py "err_log.log" 2>&1
echo If the game shut and you think it's a bug, please open a new issue on the repository of the project
echo and add the file which will show in a few seconds
echo --- Hxokunlug
notepad "err_log.log"
cd ..
pause
