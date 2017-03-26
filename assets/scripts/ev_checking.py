# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE

for ev, data in _progress.items():
    if data["trigger"]:  # TODO : find a better way to check the trigger
        # checking also the switchs
        sws = []
        if data["switch_1"] != False: sws.append(data["switch_1"])
        if data["switch_2"] != False: sws.append(data["switch_2"])
        ok = 0
        for s in sws:
            if _switchs[s] and s != False:
                ok += 1
        # checking finally the variable
        v = data["variable"][0]
        if v != False: var = _var[v] >= eval(data["variable"][1])
        else:          var = 1
        # if all the switchs are OK and the value if OK too
        if ok == len(sws) and var:
            # execute the commands, giving the global env
            #    + the locals switchs used by the event
            for line in data["commands"]:
                eval(line, globals(), {"self_switch": data["self_switch"]})