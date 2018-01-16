# game script

if _progress['p'] == 1:
    # ask player name
    Unamed.playMusic("01.ogg")
    Unamed.triggerBalloonPrompt("Quel est ton nom ? ", 18)
    _progress['p'] = 2

if _progress['p'] == 2:
    # we check if we got the name of the player
    n = Unamed.balloonPromptGetOuput()
    if n != -1:
        # set the name of the player
        Unamed.setPlayerName(n)
        # tell the beginning of the story
        Unamed.triggerBalloonPrompt("""{name}, te voici sur l'île d'Unamed. Tu es un élu d'après une ancienne prophétie,
ce qui te donne la possibilité de dresser des créatures. Te voilà donc déclaré
élu depuis moins d'un an, cherchant encore comment utiliser correctement ce don.""".format(name=Unamed.getPlayerName()), 0)
        _progress['p'] = 3

if _progress['p'] == 3:
    if Unamed.balloonPromptGetOuput() != -1:
        Unamed.triggerBalloonPrompt("message no2", 0)
        _progress['p'] = 4

if _progress['p'] == 4:
    if Unamed.balloonPromptGetOuput() != -1:
        Unamed.unloadScript("chapter1.py")
        _progress['p'] = 5