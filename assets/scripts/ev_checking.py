# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE

ev_onmap = _progress.get(Unamed.getMapId(), {})

if ev_onmap:
    for case, data in ev_onmap.items():
        # if the event was triggered (all the pages are triggered)
        if data["triggered"]:
            # checking all the pages
            for page in data["pages"]:
                # checking the switchs
                sws, ok = [], 0
                if page["switch_1"]  != False: sws.append(page["switch_1"])   # we are checking for all the switchs specified, including :
                if page["switch_2"]  != False: sws.append(page["switch_2"])   # game (global) switchs, and switch local to the event
                if page["ev_switch"] != False: sws.append(page["ev_switch"])  # 
                for s in sws:
                    if _switchs.get(s) is not None or \
                            data["local_switchs"].get(s) is not None:
                        # the switch to check can be either in the game (global) switchs
                        # or in the switchs local to the event
                        ok += 1
                
                # checking finally the variable (the value must be >= to )
                v = page["variable"][0]
                if v != False: var = _var[v] >= ast.literal_eval(page["variable"][1])
                else:          var = True
                
                # if all the switchs are OK and the value if OK too
                if ok == len(sws) and var:
                    # execute the commands, giving the global env
                    #    + the locals switchs used by the event
                    for line in page["commands"]:
                        eval(line, globals(), {"self_switch": data["local_switchs"]})
                    
                    # we have found an executable page, so we stop searching for one
                    break
            
            # unset the triggered attribute
            if data["trigger"] not in ("autorun", "parallel process"):
                data["triggered"] = False