# game launcher (for the source)
cd src/
python3 main.py >"err_log.log" 2>&1
echo If the game shut and you think it's a bug, please open a new issue on the repository of the project
echo and add the file which will show in a few seconds
echo --- Hxokunlug
gedit "err_log.log"
cd ..
read -n1 -r -p "Press any key to continue ..." key
