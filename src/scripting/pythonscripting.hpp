#ifndef DEF_PYSCRIPTING
#define DEF_PYSCRIPTING

#include <cmath>
#include <Python.h>

#define PYFUNC( x ) static PyObject* x (PyObject* self, PyObject* args)

const char* name = "Unamed";
extern "C"
{
    static PyObject* UnamedError;

    PYFUNC(registerScript);
    PYFUNC(print);
    PYFUNC(loadTexture);
    PYFUNC(displayTexture);
    PYFUNC(display_with_view);
    PYFUNC(createGlobal);
    PYFUNC(getGlobal);
    PYFUNC(playMusic);
    PYFUNC(stopMusic);
    PYFUNC(getCurrentMusicName);
    PYFUNC(getCurrentView);
    PYFUNC(hasActiveHud);
    PYFUNC(getEvent);
    PYFUNC(createPNJ);
    PYFUNC(is_tp);
    PYFUNC(getMapFromTp);
    PYFUNC(map_width);
    PYFUNC(map_height);
    PYFUNC(getMapId);
    PYFUNC(changeBlockAttrib);
    PYFUNC(getPlayerName);
    PYFUNC(getPlayerFolder);
    PYFUNC(tpPlayerOn);
    PYFUNC(screenshot);
    PYFUNC(setCurrentView);
    PYFUNC(countCreaturesInEquip);
    PYFUNC(countCreaturesInPC);
    PYFUNC(giveObject);
    PYFUNC(turnPlayer);
    PYFUNC(createText);
    PYFUNC(writeText);
    PYFUNC(setFightEnv);
    PYFUNC(setFightEsc);
    PYFUNC(triggerBalloonPrompt);
    PYFUNC(balloonPromptGetOuput);
    PYFUNC(setFightOpponents);
    PYFUNC(setCreatureName);
    PYFUNC(removeObjectsFromPocket);
    PYFUNC(addObjectsToPocket);

    PyMODINIT_FUNC PyInit_Unamed(void);
}

#endif // DEF_PYSCRIPTING
