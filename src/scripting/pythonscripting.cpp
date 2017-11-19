#include "pythonscripting.hpp"
#include "scripting.hpp"

extern "C"
{
    // module functions
    PyObject* registerScript(PyObject* self, PyObject* args)
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

    PyObject* print(PyObject* self, PyObject* args)
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

    PyObject* loadTexture(PyObject* self, PyObject* args)
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

    PyObject* displayTexture(PyObject* self, PyObject* args)
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

    PyObject* display_with_view(PyObject* self, PyObject* args)
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

    PyObject* createGlobal(PyObject* self, PyObject* args)
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

    PyObject* getGlobal(PyObject* self, PyObject* args)
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

    PyObject* playMusic(PyObject* self, PyObject* args)
    {
        const char* name;
        if (!PyArg_ParseTuple(args, "s", &name))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a char* representing the name of the song to play");
            return NULL;
        }
        return Py_BuildValue("i", PyScripting::playMusic(name));
    }

    PyObject* stopMusic(PyObject* self, PyObject* args)
    {
        PyScripting::stopMusic();

        RETURN_NONE
    }

    PyObject* getCurrentMusicName(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::getCurrentMusicName());
    }

    PyObject* getCurrentView(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getCurrentView());
    }

    PyObject* hasActiveHud(PyObject* self, PyObject* args)
    {
        int vid;
        if (!PyArg_ParseTuple(args, "i", &vid))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need an int representing the id of the view to check");
            return NULL;
        }
        return Py_BuildValue("i", PyScripting::hasActiveHud(vid));
    }

    PyObject* getEvent(PyObject* self, PyObject* args)
    {
        sf::Event ev = PyScripting::getEvent();
        std::string sev = eventToString(ev);
        PyObject *builtins = PyImport_ImportModule("builtins");
        PyObject *eval = PyObject_GetAttrString(builtins, "eval");
        PyObject *fargs = Py_BuildValue("(s)", sev.c_str());
        PyObject *result = PyObject_CallObject(eval, fargs);

        return result;
    }

    PyObject* createPNJ(PyObject* self, PyObject* args)
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

    PyObject* is_tp(PyObject* self, PyObject* args)
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

    PyObject* getMapFromTp(PyObject* self, PyObject* args)
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

    PyObject* map_width(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getMapWidth());
    }

    PyObject* map_height(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getMapHeight());
    }

    PyObject* getMapId(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::getMapId());
    }

    PyObject* changeBlockAttrib(PyObject* self, PyObject* args)
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

    PyObject* getPlayerName(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::getPlayerName());
    }

    PyObject* getPlayerFolder(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::getPlayerFolder());
    }

    PyObject* tpPlayerOn(PyObject* self, PyObject* args)
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

    PyObject* screenshot(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("s", PyScripting::screenshot());
    }

    PyObject* setCurrentView(PyObject* self, PyObject* args)
    {
        int vid;
        if (!PyArg_ParseTuple(args, "i", &vid))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need an integer");
            return NULL;
        }
        PyScripting::setCurrentView(vid);

        RETURN_NONE
    }

    PyObject* countCreaturesInEquip(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::countCreaturesInEquip());
    }

    PyObject* countCreaturesInPC(PyObject* self, PyObject* args)
    {
        return Py_BuildValue("i", PyScripting::countCreaturesInPC());
    }

    PyObject* giveObject(PyObject* self, PyObject* args)
    {
        int id;
        int qu;
        int p;
        if (!PyArg_ParseTuple(args, "ii", &id, &qu, &p))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need 3 integers : id, quantity, and pocket id");
            return NULL;
        }
        PyScripting::giveObject(id, qu, p);

        RETURN_NONE
    }

    PyObject* turnPlayer(PyObject* self, PyObject* args)
    {
        int o;
        if (!PyArg_ParseTuple(args, "i", &o))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need an integer representing the orientation of the player");
            return NULL;
        }
        PyScripting::turnPlayer(o);

        RETURN_NONE
    }

    PyObject* createText(PyObject* self, PyObject* args)
    {
        int x, y, w;
        const char* text;
        int fs, c;
        const char* id;
        if (!PyArg_ParseTuple(args, "iiisiis", &x, &y, &w, &text, &fs, &c, &id))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need 3 integers (x, y, number of characters), a string for the text, the font size, the color and finally an id (string)");
            return NULL;
        }
        PyScripting::createText(x, y, w, text, fs, c,id);

        RETURN_NONE
    }

    PyObject* writeText(PyObject* self, PyObject* args)
    {
        const char* id;
        if (!PyArg_ParseTuple(args, "s", &id))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need a string id for the text to display");
            return NULL;
        }
        PyScripting::writeText(id);

        RETURN_NONE
    }

    PyObject* setFightEnv(PyObject* self, PyObject* args)
    {
        int i;
        if (!PyArg_ParseTuple(args, "i", &i))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need an int representing the type of environment to set");
            return NULL;
        }
        PyScripting::setFightEnv(i);

        RETURN_NONE
    }

    PyObject* setFightEsc(PyObject* self, PyObject* args)
    {
        int e;
        if (!PyArg_ParseTuple(args, "p", &e))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need a boolean to set the escape mode");
            return NULL;
        }
        PyScripting::setFightEscape(e == 1);

        RETURN_NONE
    }

    PyObject* triggerBalloonPrompt(PyObject* self, PyObject* args)
    {
        const char* s;
        int len;
        if (!PyArg_ParseTuple(args, "si", &s, &len))
        {
            // let's try to parse only a "s"
            if (!PyArg_ParseTuple(args, "s", &s))
            {
                PyErr_SetString(UnamedError, "Can not parse arguments, need a string for the prompt");
                return NULL;
            }
            // if we're here it means we only parsed a string
            len = 0; /// max length
        }

        std::string s2 = fix_my_utf8(s);
        int e = 0;
        PyScripting::triggerBalloonPrompt(s2, e, len);
        if (e == 1)
        {
            PyErr_SetString(UnamedError, "Balloon prompt is already triggered, can not re-trigger it while it is running");
            return NULL;
        }
        RETURN_NONE
    }

    PyObject* balloonPromptGetOuput(PyObject* self, PyObject* args)
    {
        const char* s = PyScripting::balloonPromptGetOuput();
        if (s == 0)
        {
            return Py_BuildValue("i", -1);
        }
        return Py_BuildValue("s", s);
    }

    PyObject* setFightOpponents(PyObject* self, PyObject* args)
    {
        int id_1, lvl_1, life_1, atk_1, def_1, stype_1, sdmg_1, stargets_1;
        int id_2, lvl_2, life_2, atk_2, def_2, stype_2, sdmg_2, stargets_2;
        int id_3, lvl_3, life_3, atk_3, def_3, stype_3, sdmg_3, stargets_3;
        int c = 0;
        if (!PyArg_ParseTuple(args, "[iiiiiiii][iiiiiiii][iiiiiiii]",
                              &id_1, &lvl_1, &life_1, &atk_1, &def_1, &stype_1, &sdmg_1, &stargets_1,
                              &id_2, &lvl_2, &life_2, &atk_2, &def_2, &stype_2, &sdmg_2, &stargets_2,
                              &id_3, &lvl_3, &life_3, &atk_3, &def_3, &stype_3, &sdmg_3, &stargets_3))
        {
            if (!PyArg_ParseTuple(args, "[iiiiiiii][iiiiiiii]",
                                  &id_1, &lvl_1, &life_1, &atk_1, &def_1, &stype_1, &sdmg_1, &stargets_1,
                                  &id_2, &lvl_2, &life_2, &atk_2, &def_2, &stype_2, &sdmg_2, &stargets_2))
            {
                if (!PyArg_ParseTuple(args, "[iiiiiiii]",
                                      &id_1, &lvl_1, &life_1, &atk_1, &def_1, &stype_1, &sdmg_1, &stargets_1))
                {
                    PyErr_SetString(UnamedError, "Can not parse arguments for setFightOpponents. Need between 1 and 3 lists of 8 arguments (int)");
                    return NULL;
                }
                else
                {
                    c = 1;
                }
            }
            else
            {
                c = 2;
            }
        }
        else
        {
            c = 3;
        }

        if (c == 0)
        {
            PyErr_SetString(UnamedError, "Can not parse arguments for setFightOpponents. Need between 1 and 3 lists of 8 arguments (int)");
            return NULL;
        }
        else if (c == 1)
        {
            fight_opponent fo_1 = {id_1, lvl_1, life_1, atk_1, def_1, stype_1, sdmg_1, stargets_1};
            PyScripting::set_fight_opponents(fo_1);
        }
        else if (c == 2)
        {
            fight_opponent fo_1 = {id_1, lvl_1, life_1, atk_1, def_1, stype_1, sdmg_1, stargets_1};
            fight_opponent fo_2 = {id_2, lvl_2, life_2, atk_2, def_2, stype_2, sdmg_2, stargets_2};
            PyScripting::set_fight_opponents(fo_1, fo_2);
        }
        else if (c == 3)
        {
            fight_opponent fo_1 = {id_1, lvl_1, life_1, atk_1, def_1, stype_1, sdmg_1, stargets_1};
            fight_opponent fo_2 = {id_2, lvl_2, life_2, atk_2, def_2, stype_2, sdmg_2, stargets_2};
            fight_opponent fo_3 = {id_3, lvl_3, life_3, atk_3, def_3, stype_3, sdmg_3, stargets_3};
            PyScripting::set_fight_opponents(fo_1, fo_2, fo_3);
        }

        RETURN_NONE
    }

    PyObject* setCreatureName(PyObject* self, PyObject* args)
    {
        int in, id;
        const char* name;
        if (!PyArg_ParseTuple(args, "iis", &in, &id, &name))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need two int (one to tell if you want a creature in the PC or in the team, another for the id of the creature in the container chosen) and a string for the name");
            return NULL;
        }
        int out = PyScripting::set_creature_name(in, id, name);
        if (out == -1)
        {
            PyErr_SetString(UnamedError, "The creature chosen is not in range");
            return NULL;
        }
        RETURN_NONE
    }

    PyObject* removeObjectsFromPocket(PyObject* self, PyObject* args)
    {
        int pocket, id, qu;
        if (!PyArg_ParseTuple(args, "iii", &pocket, &id, &qu))
        {
            if (!PyArg_ParseTuple(args, "ii", &pocket, &id))
            {
                PyErr_SetString(UnamedError, "Can not parse arguments, need three int, one for the pocket id, and another for the unique id of the object to remove. The last one is the quantity of the object to remove, 1 by default");
                return NULL;
            }
            else
                qu = 1;
        }
        PyScripting::remove_object_from_pocket(pocket, id, qu);
        RETURN_NONE
    }

    PyObject* addObjectsToPocket(PyObject* self, PyObject* args)
    {
        int pocket, id, qu;
        if (!PyArg_ParseTuple(args, "iii", &pocket, &id, &qu))
        {
            PyErr_SetString(UnamedError, "Can not parse arguments, need three int, one for the pocket id, one for the unique id of the object to add, and another for the quantity of the object to add");
            return NULL;
        }
        PyScripting::add_object_to_pocket(pocket, id, qu);
        RETURN_NONE
    }

    PyObject* setPlayerName(PyObject* self, PyObject* args)
    {
        const char* n;
        if (!PyArg_ParseTuple(args, "s", &n))
        {
            PyErr_SetString(UnamedError, "Can not parse argument, need a string for the name of the character");
            return NULL;
        }
        PyScripting::setPlayerName(n);
        RETURN_NONE
    }

    static PyMethodDef UnamedMethods[] = {
        // ...
        {"upr", print, METH_VARARGS, "Print function using std::cout instead of the standard output stream of Python"},
        {"registerScript", registerScript, METH_VARARGS, "Register a script in the PyScripting singleton, as a specific kind given as an argument, with a string id also given"},
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
        {"getPlayerFolder", getPlayerFolder, METH_VARARGS, "Return the save folder of the player"},
        {"tpPlayerOn", tpPlayerOn, METH_VARARGS, "Take two integers (x, y). Will teleport the player on this position, on the current map"},
        {"screenshot", screenshot, METH_VARARGS, "Take a screenshot and save it to `screenshots/`. Return the name of the file"},
        {"setCurrentView", setCurrentView, METH_VARARGS, "Set the current view. Take the id of the new view"},
        {"countCreaturesInEquip", countCreaturesInEquip, METH_VARARGS, "Return how many creatures has the player in his/her equip"},
        {"countCreaturesInPC", countCreaturesInPC, METH_VARARGS, "Return how many creatures has the player in his/her PC"},
        {"giveObject", giveObject, METH_VARARGS, "Give an object to the player. Need the id of the object, the quantity to give, and in which pocket (id) it will be put"},
        {"turnPlayer", turnPlayer, METH_VARARGS, "Turn the player in the specified direction. 0: up, 1: down, 2: left, 3: right"},
        {"createText", createText, METH_VARARGS, "Create a text at (x, y) of a number of characters/lines given, with the text of a font size, and a color given, the whole thing having a string id given"},
        {"writeText", writeText, METH_VARARGS, "Write a text, giving the id of a created text"},
        {"setFightEnv", setFightEnv, METH_VARARGS, "Set the environment for a fight"},
        {"setFightEscape", setFightEsc, METH_VARARGS, "Set the fight escape mode"},
        {"triggerBalloonPrompt", triggerBalloonPrompt, METH_VARARGS, "Display a ballon message with a given prompt (possibly a max length to get (in characters), default to 0 which means no limit), and wait for an input (validated by Return key). Not blocking the main thread"},
        {"balloonPromptGetOuput", balloonPromptGetOuput, METH_VARARGS, "Get the output of the balloon prompt. -1 if the user did not validate ; if the balloon was not triggered, raises UnamedError"},
        {"setFightOpponents", setFightOpponents, METH_VARARGS, "Set the opponents of the next fight. Need between 1 and 3 lists of 8 elements : [id, lvl, life, atk, def, sort_type, sort_dmg, sort_targets]. To let the game choose random values based on others already computed, set the wanted value to -1. The id is always needed, but all the other values can be set to -1"},
        {"setCreatureName", setCreatureName, METH_VARARGS, "Set the name of a creature. Need an int (0 if you are in the team, 1 in the PC) to choose a container, another for the id of the creature in the container, and the name (string)"},
        {"removeObjectsFromPocket", removeObjectsFromPocket, METH_VARARGS, "Remove an object from a specified pocket, given its unique id and the quantity to remove (the quantity is optional, 1 by default)"},
        {"addObjectsToPocket", addObjectsToPocket, METH_VARARGS, "Add an object to a specified pocket, given its unique id and a quantity"},
        {"setPlayerName", setPlayerName, METH_VARARGS, "Set the name of the player. Need a string"},
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

