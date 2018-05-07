Comment commencer
============
Premièrement, créez un fichier `.py` dans le dossier `assets/scripts`, que vous nommerez selon vos envies.

Puis ouvrez-le dans un éditeur de scripts (comme [Notepad++](https://notepad-plus-plus.org/)), nous allons écrire notre premier code pour le jeu.

Pour inclure quelques méthodes "built in" pour modifier le jeu, nous allons inclure le module `Unamed` comme ceci :

```py
import Unamed
```

Puis nous allons déclarer le type de fichier (à quoi va-t-il servir) dans le fichier `register.py` : 

```py
Unamed.registerScript(type, nom)
```

`type` et `nom` devraient être des chaînes de caractères.  Pour le nom, il **doit** être le nom complet de votre script (par exemple `"myscript.py"`). Pour le type de script, cela va dépendre de comment celui-ci va fonctionner:

* juste une fois, à l'ouverture du jeu ? utilisez `runOnceWhenStarting`

* juste une fois, à la fermeture du jeu ? utilisez `runOnceWhenClosing`

* chaque fois que le jeu est mis à jour ? utilisez `runWhenUpdatingGame`

* chaque fois que le jeu lance les évènements, vous avez besoin d'aller chercher ces évènements ? utilisez `runWhenProcessingEvents`

* chaque fois que le jeu affiche quelque chose ? utilisez `runWhenRenderingView`

Appeler l'ordre qui concerne le type de script
=============================================

* `runOnceWhenStarting`, seulement une fois, au lancement du jeu

puis :arrow_right: 

Pendant que le jeu tourne :

* `runWhenUpdatingGame`, chaque fois que le jeu est mis à jour

puis :arrow_right: 

* `runWhenProcessingEvents`, chaque fois que le jeu reçoit un évènement

puis :arrow_right: 

* `runWhenRenderingView`, chaque fois qu'une vue est affichée

puis :arrow_right: 

* `runOnceWhenClosing`, seulement une fois, à la fermeture du jeu

Notez que les scripts de type `runOnceWhenStarting` ne peuvent être lancés que lorsque tous les autres composants du jeu (comme le joueur, son sac, son équipe et tout ce dont il a besoin  ; ainsi que les maps, et les vues ... etc) sont chargés, donc il est possible d'appliquer directement des modifications à l'équipe ou au joueur avant que le jeu ne commence vraiment.

Ce que nous pouvons faire et ce que nous ne pouvons pas faire
==================================
Les scripts ayant cette forme : 

* `runOnceWhenStarting`

* `runOnceWhenClosing`

* `runWhenUpdatingGame`

* `runWhenProcessingEvents`

* `runWhenRenderingView`

ne devraient pas, ou le moins possible , et doivent tourner en vraiment peu de temps, parce qu'ils ne peuvent pas être mis sur un thread séparé. Si les scripts de ces types ci prennent trop de temps à s'exécuter, le jeu s'en verra donc ralenti.

Script commençant par runWhen... mais qui a besoin de variables globales
=================================================
Si votre script devrait tourner lorsque le jeu se met à jour, mais qu'il a besoin d'une variable globale, vous pouvez penser que votre script ne gardera pas la valeur entre chaque frame. Donc vous pouvez utiliser cette méthode pour créer une valeur globale: 

```py
import Unamed

Unamed.createGlobal(name, value)
```

Le code C++ qui s'en occupe essaiera de déduire le type lui-même ;) il retournera 1 si c'est un succès, sinon 0 (par exemple si 'name' existe déjà dans le tableau des variables globales). `name` devrait être un 'string'.

Et quand vous voulez obtenir une valeur globale, vous avez juste à faire ceci : 

```py
import Unamed

Unamed.getGlobal(name)
```

avec `name` l'identifiant de votre variable globale (c'est aussi un 'string')

Mais il y a une autre astuce qui est que le moteur de script charge les fichiers en texte brut, et puis envoie ce "texte" à Python. Ce qui veut dire que vous pouvez faire ceci, sans aucun souci : 

fichier 1:
```py
import Unamed

# some actions

global_var = 1
```

fichier 2:
```py
import Unamed

# some actions

print(global_var)
```

Parce que c'est pareil que : 
```py
import Unamed

# some actions

global_var = 1

import Unamed

# some actions

print(global_var)
```

Parce que les fichiers sont lus par le même interpréteur, les uns après les autres, et qu'ils ne sont pas enchaînés (donc vous avez juste à appeler le module Unamed dans register.py et vous pouvez l'utiliser sans l'importer dans tous les autres scripts), mais soyez sûr que le fichier 1 sera appelé avant le fichier 2, en vérifiant l'ordre d'appel de ces scripts.

Chargement et affichage d'images
=============================
Rien de plus simple : 

loader.py (défini comme `runOnceWhenStarting` dans register.py)
```py
import Unamed

Unamed.loadImage("path/to/image.png", "id")
```

display.py (défini comme `runWhenRenderingView` dans register.py)
```py
import Unamed

Unamed.displayImage("id", 150, 140)
```

Jouer avec les musiques
=======================
Tout le système a été simplifié, donc vous avez une très petite interface comme décrit ci-dessous : 

* `Unamed.playMusic(name)` avec `name` le nom de la chanson, comme "01.ogg" par exemple
* `Unamed.stopMusic()` qui stoppe la musique, si une musique est jouée
* `Unamed.getCurrentMusicName()` qui retourne le nom de la musique actuellement jouée

Jouer avec la map
====================
Maintenant, vous pouvez obtenir la largeur et la hauteur de la map comme ceci :

* `Unamed.getMapWidth()`

* `Unamed.getMapHeight()`

Vous pouvez aussi vérifier si un block est soit une arrivée (un spawn) d'une autre map, soit un point de téléportation pour aller sur une autre map :

* `Unamed.isSpawn(x, y)` avec `x` et `y` représentant la position du block, en blocks (pas en pixels !). Il retournera `1` si le block est un spawn, sinon `0`

* `Unamed.isTp(x, y)` avec `x` et `y` représentant la position du block, en blocks (pas en pixels !). Il retournera `1` si le block est une téléportation, sinon `0`

Puis vous pouvez également obtenir la position du spawn où vous arrivez sur une map d'ID X comme ceci : `Unamed.getSpawnPosFromMapId(X)`. Il retourne un tuple de 2 entiers (`x`, `y`) si un spawn avec la map id X est trouvé, sinon `-1`.

Enfin, vous pouvez obtenir le lieu où vous arrivez avec un point de téléportation, à une position donnée : `Unamed.getMapIdFromTpPos(x, y)` avec `x` et `y` 2 entiers. Si un point de téléportation `x, y` n'est pas trouvé, la fonction retournera `-1`.

Variables globales liées au jeu
===========================
Si vous avez besoin de savoir quel est l'ID de vue actuel, par exemple pour savoir si vous devez afficher une image, vous pouvez utiliser cette méthode: `Unamed.getCurrentViewId ()` qui renvoie un entier représentant l'ID de vue (reportez-vous à la page `Vues` sur ce wiki pour en savoir plus sur celles-ci.

Egalement, si vous voulez vérifier si la vue actuelle a un HUD actif traitant des événements / faisant de l'affichage, utilisez `Unamed.hasActiveHud(vid)` avec `vid` un view_id (pour savoir si la vue actuelle a un HUD actif, vous pouvez utiliser `Unamed.hasActiveHud(Unamed.getCurrentView ())`)

Créer un PNJ
==============
Rien n'est plus facile que de créer un PNJ : 

```py
import Unamed

Unamed.createPNJ(0, "test", "je suis un test pnj", 0, "Test PNJ", 32, 32)
```

Liste des arguments : 

* `map id` : entier

* `folder` : string (ils sont tous dans `assets/pnj`)

* `text` : string (ce que le PNJ dira)

* `kind` : entier (allez sur la page PNJ du wiki pour en savoir plus)

* `display name` : string (le nom qui sera affiché)

* `x` : entier (pixels, position relative du coin supérieur gauche de la map)

* `y` : entier (pixels, position relative du coin supérieur gauche de la map)

Obtenir des `sf::Event`s
====================
Ce n'est pas intéressant si nous ne pouvons pas jouer avec les évents que le jeu exécute, non ?

Pour récupérer le dernier `Event` que le jeu a exécuté, marquez votre script comme `runWhenProcessingEvents`, puis : 

```py
import Unamed

ev = Unamed.getEvent()
```

`Unamed.getEvent()` retourne un `dict`. Enfin, pour vérifier s'il a une clé spécifique et retrouver sa valeur associée, utilisez la méthode `.get(key, [default value])`.
Pour obtenir le type d'un évent, vous pouvez écrire: 

```
type_ev = ev.get('type')
if type_ev:
    # nous pouvons l'exécuter, il a un type
```

Les types d'évents supportés sont ici (notez qu'ils sont écrits en caractères minuscules) : 

* closed
* resized
* lostfocus
* gainedfocus
* textentered => `{'unicode': character}`
* keypressed => `{'key': keycode, 'alt': bool, 'control': bool, 'shift': bool, 'system': bool}`
* keyreleased => pareil que keypressed, mais lancé uniquement quand une touche est relâchée
* mousewheelscrolled => `{'vertical': bool, 'delta': int, 'x': int, 'y': int}`
* mousebuttonpressed => `{'button': int, 'x': int, 'y': int}`
* mousebuttonreleased => pareil que mousebuttonpressed, mais lancé uniquement quand un bouton de la souris est relâché
* mousemoved => `{'x': int, 'y': int}`
* mouseentered
* mouseleft
* unrecognized => pour tous les autres évents non-supportés par le jeu (principalement ceux liés au joystick)

Si vous n'êtes pas familier avec la SFML, cela peut vous aider à mieux comprendre comment fonctionnent les événements : [documentation SFML sur sf::Event](https://www.sfml-dev.org/tutorials/2.4/window-events.php)
