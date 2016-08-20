cd src
echo Start building
echo You need a computer under Windows >= 8 with Python 3.4 and Pygame installed

echo -------------------------------

REM add an icon !
C:\Python34\Scripts\cxfreeze -s -O --target-dir=..\build main.py
REM --icon=ICON

echo -------------------------------

echo Building finished !

cd ..

pause