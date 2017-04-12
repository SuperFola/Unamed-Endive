#include "pythonscripting.hpp"
#include "scripting.hpp"

extern "C"
{
    // module functions
    static PyObject* registerScript(PyObject* self, PyObject* args)
    {
        const char* kind;
        const char* id;
        char result[256];
        if (!PyArg_ParseTuple(args, "ss", &kind, &id))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the kind of the script, and a const char* representing the id of the script");
            return NULL;
        }
        if (PyScripting::setModuleKind(kind, id) == -1)
        {
            strcpy(result, "You have already defined a kind for this script : ");
            strcat(result, id);
            PyErr_SetString(UnamedError, result);
            return NULL;
        }

        RETURN_NONE
    }

    static PyObject* print(PyObject* self, PyObject* args)
    {
        PyObject* content;
        char result[256];
        if (!PyArg_ParseTuple(args, "O", &content))
        {
            strcpy(result, "Can not parse argument : need a single argument, of any type");
            PyErr_SetString(UnamedError, result);
            return NULL;
        }
        PyObject* objectsRepresentation = PyObject_Repr(content);
        const char* parsed = PyUnicode_AsUTF8(objectsRepresentation);
        Py_XDECREF(objectsRepresentation);

        PyScripting::print(parsed);

        RETURN_NONE
    }

    static PyObject* loadTexture(PyObject* self, PyObject* args)
    {
        const char* name;
        const char* id;
        char result[128];

        if (!PyArg_ParseTuple(args, "ss", &name, &id))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the path to the image, and a const char* giving the id of the image");
            return NULL;
        }
        if (PyScripting::loadImage(name, id) == -1)
        {
            strcpy(result, "Can not find the image at ");
            strcat(result, name);
            PyErr_SetString(UnamedError, result);
            return NULL;
        }

        RETURN_NONE
    }

    static PyObject* displayTexture(PyObject* self, PyObject* args)
    {
        const char* id;
        int x;
        int y;
        char result[128];

        if (!PyArg_ParseTuple(args, "sii", &id, &x, &y))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the id of the image, and 2 integers representing the position of the image (relative to the upper right corner of the map)");
            return NULL;
        }
        if (PyScripting::displayImage(id, x, y) == -1)
        {
            strcpy(result, "Can not find the image with id ");
            strcat(result, id);
            PyErr_SetString(UnamedError, result);
            return NULL;
        }

        RETURN_NONE
    }

    static PyObject* display_with_view(PyObject* self, PyObject* args)
    {
        const char* id;
        int x;
        int y;
        char result[128];

        if (!PyArg_ParseTuple(args, "sii", &id, &x, &y))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a const char* representing the id of the image, and 2 integers representing the position of the image (relative to the upper right corner of the map)");
            return NULL;
        }
        if (PyScripting::displayImageWithView(id, x, y) == -1)
        {
            strcpy(result, "Can not find the image with id ");
            strcat(result, id);
            PyErr_SetString(UnamedError, result);
            return NULL;
        }

        RETURN_NONE
    }

    static PyObject* createGlobal(PyObject* self, PyObject* args)
    {
        const char* name;
        svar_t gvar;

        if (!PyArg_ParseTuple(args, "si", &name, &gvar.ivar))
        {
            if (!PyArg_ParseTuple(args, "sf", &name, &gvar.fvar))
            {
                if (!PyArg_ParseTuple(args, "ss", &name, &gvar.cvar))
                {
                    if (!PyArg_ParseTuple(args, "sp", &name, &gvar.bvar))
                    {
                        PyErr_SetString(UnamedError, "Can not parse argument, need either an int, a float, or a char*");
                        return NULL;
                    } else gvar.kind = "p";
                } else gvar.kind = "s";
            } else gvar.kind = "f";
        } else gvar.kind = "i";
        // if we are here, we successfully casted the python var into a C/C++ one
        int v = PyScripting::createGlobal(name, gvar);
        return PyLong_FromLong(v);
    }

    static PyObject* getGlobal(PyObject* self, PyObject* args)
    {
        const char* name;
        if (!PyArg_ParseTuple(args, "s", &name))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a char* representing the name of the global var to get");
            return NULL;
        }
        char result[128];
        svar_t gvar = PyScripting::getGlobal(name);
        PyObject* v;

        switch (*gvar.kind)
        {
        case 'e':
            goto failed;
            break;

        case 'p':
            v = Py_BuildValue(gvar.kind, gvar.bvar);
            break;

        case 's':
            v = Py_BuildValue(gvar.kind, gvar.cvar);
            break;

        case 'f':
            v = Py_BuildValue(gvar.kind, gvar.fvar);
            break;

        case 'i':
            v = Py_BuildValue(gvar.kind, gvar.ivar);
            break;

        default:
            goto failed;
            break;
        }
        return v;

        failed:
        {
            strcpy(result, "Could not find the global var named ");
            strcat(result, name);
            PyErr_SetString(UnamedError, result);
            return NULL;
        }
    }

    static PyObject* playMusic(PyObject* self, PyObject* args)
    {
        const char* name;
        if (!PyArg_ParseTuple(args, "s", &name))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a char* representing the name of the song to play");
            return NULL;
        }
        return Py_BuildValue("i", PyScripting::playMusic(name));
    }

    static PyObject* stopMusic(PyObject* self, PyObject* args)
    {
        PyScripting::stopMusic();

        RETURN_NONE
    }

    static PyObject* getCurrentMusicName(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::getCurrentMusicName());
    }

    static PyObject* getCurrentView(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getCurrentView());
    }

    static PyObject* hasActiveHud(PyObject* self, PyObject* args)
    {
        int vid;
        if (!PyArg_ParseTuple(args, "i", &vid))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the id of the view to check");
            return NULL;
        }
        return Py_BuildValue("i", PyScripting::hasActiveHud(vid));
    }

    static PyObject* getEvent(PyObject* self, PyObject* args)
    {
        sf::Event ev = PyScripting::getEvent();
        std::string sev = eventToString(ev);
        PyObject *builtins = PyImport_ImportModule("builtins");
        PyObject *eval = PyObject_GetAttrString(builtins, "eval");
        PyObject *fargs = Py_BuildValue("(s)", sev.c_str());
        PyObject *result = PyObject_CallObject(eval, fargs);

        return result;
    }

    static PyObject* createPNJ(PyObject* self, PyObject* args)
    {
        int mid;
        const char* name;
        const char* text;
        int pnjkind_i;
        const char* dname;
        int x;
        int y;
        if (!PyArg_ParseTuple(args, "issisii", &mid, &name, &text, &pnjkind_i, &dname, &x, &y))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the id of the map, a const char* for the filename, a const char* for the text of the pnj, an int representing the kind of pnj, a const char* for its display name and two int for its position (x and y)");
            return NULL;
        }
        PyScripting::createPNJ(mid, name, text, pnjkind_i, dname, x, y);

        RETURN_NONE
    }

    static PyObject* is_tp(PyObject* self, PyObject* args)
    {
        int x;
        int y;
        if (!PyArg_ParseTuple(args, "ii", &x, &y))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need two int representing the position of the block to test");
            return NULL;
        }
        return Py_BuildValue("i", PyScripting::map_is_tp(x, y));
    }

    static PyObject* getMapFromTp(PyObject* self, PyObject* args)
    {
        int x;
        int y;
        if (!PyArg_ParseTuple(args, "ii", &x, &y))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need two int representing the position of the teleporter");
            return NULL;
        }
        int v = -1;
        if (PyScripting::map_is_tp(x, y))
            v = PyScripting::map_getMapFromTp(x, y);
        return Py_BuildValue("i", v);
    }

    static PyObject* map_width(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getMapWidth());
    }

    static PyObject* map_height(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getMapHeight());
    }

    static PyObject* getMapId(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getMapId());
    }

    static PyObject* changeBlockAttrib(PyObject* self, PyObject* args)
    {
        int rid;
        const char* id;
        int v;
        if (!PyArg_ParseTuple(args, "isp", &rid, &id, &v))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the case (rpos), a string representing the attribute to change, and an int for the value (0 | 1)");
            return NULL;
        }

        PyScripting::changeBlockAttrib(rid, id, v);

        RETURN_NONE
    }

    static PyObject* getPlayerName(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::getPlayerName());
    }

    static PyObject* tpPlayerOn(PyObject* self, PyObject* args)
    {
        int x;
        int y;
        if (!PyArg_ParseTuple(args, "ii", &x, &y))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need two integers (x, y)");
            return NULL;
        }

        PyScripting::map_tpPlayerOn(x + y * PyScripting::getMapWidth());

        RETURN_NONE
    }

    static PyObject* screenshot(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::screenshot());
    }

    static PyMethodDef UnamedMethods[] = {
        // ...
        {"upr", print, METH_VARARGS, "Print function using std::cout"},
        {"registerScript", registerScript, METH_VARARGS, "Register a script in the PyScripting singleton, as a specific kind given as an argument, with an id also given"},
        {"loadImage", loadTexture, METH_VARARGS, "Load an image using a given path, and assigne it to a given id"},
        {"displayImage", displayTexture, METH_VARARGS, "Display an image loaded before using loadImage with its id, and its position (2 integers, x and y)"},
        {"displayWithView", display_with_view, METH_VARARGS, "Display an image loaded before, relative to the top left corner of the view. Need the same argument as displayImage(...)"},
        {"createGlobal", createGlobal, METH_VARARGS, "Create a global value from a given id (of type char*), with a specified value (int, float and char* are currently supported)"},
        {"getGlobal", getGlobal, METH_VARARGS, "Return a global value with the name given"},
        {"getCurrentMusicName", getCurrentMusicName, METH_VARARGS, "Return the name of the current playing music, if one is playing"},
        {"getCurrentViewId", getCurrentView, METH_VARARGS, "Return the current view id"},
        {"stopMusic", stopMusic, METH_VARARGS, "Stop the current, if one is playing"},
        {"playMusic", playMusic, METH_VARARGS, "Start a song from its name"},
        {"hasActiveHud", hasActiveHud, METH_VARARGS, "Check if a view, with a given id, has currently an active HUD"},
        {"getEvent", getEvent, METH_VARARGS, "Get the current event and return it using a dict"},
        {"createPNJ", createPNJ, METH_VARARGS, "Create a PNJ (refer to the wiki to learn more about the PNJkind)"},
        {"isTp", is_tp, METH_VARARGS, "Return 1 if the block at the pos x y is a tp, otherwise 0"},
        {"getMapIdFromTpPos", getMapFromTp, METH_VARARGS, "Return a map id from a tp pos. If the tp can not be found, return -1"},
        {"getMapWidth", map_width, METH_VARARGS, "Return the width of the map (in blocks)"},
        {"getMapHeight", map_height, METH_VARARGS, "Return the height of the map (in blocks)"},
        {"getMapId", getMapId, METH_VARARGS, "Return the id of the map (int)"},
        {"changeBlockAttribute", changeBlockAttrib, METH_VARARGS, "To change attributes of a specified block on the current map"},
        {"getPlayerName", getPlayerName, METH_VARARGS, "Return the name of the player"},
        {"tpPlayerOn", tpPlayerOn, METH_VARARGS, "Take two integers (x, y). Will teleport the player on this position, on the current map"},
        {"screenshot", screenshot, METH_VARARGS, "Take a screenshot and save it to screenshots/. Return the name of the file"},
        // ...
        {NULL, NULL, 0, NULL}  // sentinel
    };
    static PyModuleDef UnamedModule = {
        PyModuleDef_HEAD_INIT,
        name,
        "C++ functions to modify the way some components of the game works",
        -1,
        UnamedMethods
    };

    // init the module
    PyMODINIT_FUNC PyInit_Unamed(void)
    {
        PyObject* m;

        m = PyModule_Create2(&UnamedModule, PYTHON_API_VERSION);
        if (m == NULL)
            return NULL;

        UnamedError = PyErr_NewException("Unamed.error", NULL, NULL);
        Py_INCREF(UnamedError);
        PyModule_AddObject(m, "error", UnamedError);

        return m;
    }
}

