# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE

ev_onmap = _progress.get(Unamed.getMapId(), {})

if ev_onmap:
    for case, data in ev_onmap.items():
        if data["triggered"]:
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
            if v != False: var = _var[v] >= ast.literal_eval(data["variable"][1])
            else:          var = True
            # if all the switchs are OK and the value if OK too
            if ok == len(sws) and var:
                # execute the commands, giving the global env
                #    + the locals switchs used by the event
                for line in data["commands"]:
                    eval(line, globals(), {"self_switch": data["self_switch"]})
                if data["trigger"] not in ("autorun", "parallel process"):
                    data["triggered"] = False