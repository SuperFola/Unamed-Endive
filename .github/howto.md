# How to ...

## Change the SFML version used

If you are using Code::Block like me, just download the .cbp project file, open the project in C::B and go in Project > Build Options > "project name" (not Debug or Release) > Custom variables and change the value of `SFML_VERSION`.

By default the SFML folders should be in the drive `C:\\` and named as follow : `SFML-X.Y.Z`

The SFML versions used are the one for GCC 4.9.2 (SJLJ), 32 bits