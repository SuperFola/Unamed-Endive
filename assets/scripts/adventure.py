# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE

##################################################################

if _progress.get('current_map', 0) == 0 and _progress.get('new_game', True) and _progress.get('p', 0) == 0:
    # ask character name
    Unamed.triggerBalloonPrompt("Quel est ton nom ? ", 18)
    _progress['p'] = 1

if _progress['p'] == 1:
    n = Unamed.balloonPromptGetOuput()
    if n != -1:
        Unamed.setPlayerName(n)
        _progress['p'] = 2
        Unamed.triggerBalloonPrompt("Bienvenue dans cette aventure extraordinaire !", 0)

if _progress['p'] == 2:
    if Unamed.balloonPromptGetOuput() != -1:
        _progress['p'] = 3
        Unamed.triggerBalloonPrompt("message no2", 0)