# game script

# register chapters loading here

if _progress.get('current_map', 0) == 0 and _progress.get('new_game', True) and _progress.get('p', 0) == 0:
    # load chapter 1
    Unamed.registerScript("runWhenUpdatingGame", "chapter1.py")
    _progress['p'] = 1