**[README en anglais](README.md)**

# Unamed Rebirth [![Codacy Badge](https://api.codacy.com/project/badge/Grade/ae66540cc8d040b7955c6b93c0f9da2e)](https://www.codacy.com/app/urworld/Unamed-Endive?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Loodoor/Unamed-Endive&amp;utm_campaign=Badge_Grade) [![Build Status](https://travis-ci.org/Loodoor/Unamed-Endive.svg?branch=master)](https://travis-ci.org/Loodoor/Unamed-Endive) [![Discord badge](https://img.shields.io/badge/Discord-Unamed__Rebirth-brightgreen.svg?style=flat)](https://discord.gg/AWSWvgU)

Type de jeu
===========
Pokémon-like, ARPG, dungeon crawler, jouable sous peu en LAN (peut-être même WAN)

Histoire
========
Vous êtes un chercheur et arrivez sur Unamed, une île où vivent très peu de personnes. 
Les seules régions peuplées sont situées près du rivage, ou sont en tout cas bien protégées, 
car les habitant n'ont pas de créatures pour se défendre face à celles vivants dans les terres. 
Vous allez vous installer sur cette île après avoir vu un de vos amis chercheur mourrir à cause 
d'une attaque de la part d'une horde de créatures, et partir à la recherche de toutes les 
créatures de l'île pour comprendre ce qui s'est passé !

Il vous faudra de la patience pour venir à bout des dongeons, et les meilleures créatures possibles !

Ce qui est nécessaire au bon fonctionnement du jeu
==================================================
Ce jeu nécessite un compilateur C++ à jour et la SFML 2.4.1 pour fonctionner correctement, si vous souhaitez l'installer via la source.

Sinon, vous avez juste à télécharger la dernière release correspondant à votre système et à lancer le jeu !

Compiler le projet
==================
Si vous souhaitez compiler le projet, il vous faut installer la SFML et suivre ces étapes : https://loodoor.github.io/Unamed-Endive/compiling.html

Crédits
=======
* tiles d'extérieur : WesleyFG (à la base pour le projet Marnian)
* tiles d'intérieur : Slahs
* interface graphique (inventaire, dex, équipe/PC) : vulvoch
* interface grpahique (menu) : Eurons ??
* fond des combats : WesleyFG
* créatures : Smiley-Fakemon
* carte du monde : Sansonic
* noms des villes `Piderflor`, `Muth'ira`, `Silancard` : ShootingStarD
* musiques : Bat'
* personnages :
    * `Darth`, `Vader`, `Tress` : [OceansLugiaSpirit](http://oceanslugiaspirit.deviantart.com/)
    * `Minus` : [Lightbulb15](http://lightbulb15.deviantart.com)

En cas de bug
=============
Si vous rencontrez un bug, merci de créer une nouvelle [issue](https://github.com/Loodoor/Unamed-Endive/issues/new) 
et de l'y décrire comme suit :

```
Système d'exploitation | Version (32/64bits) | Message d'erreur (oui/non)
-----------------------|---------------------|---------------------------
Nom du système         | Version du système  | Y avait il un message d'erreur ?

Fichier log généré
==================
*contenu ici*

Erreur donnée par le système
============================
*si oui dans le tableau*

Résultat de la commande (sous Windows uniquement)
=================================================
Ouvrir un terminal en pressant `WIN` et `R` en même temps. Taper `cmd` puis appuyer sur `ENTREE`. Ensuite taper `cd PATH` avec `PATH` le chemin absolu vers le dossier principal du jeu.

commande : `echo base dir & dir /D & echo saves & dir saves /D & echo scripts & dir "assets/scripts" /D & echo register script & type ".\assets\scripts\register.py"`

*résultat ici*

Comment reproduire le bug
=========================
*si vous avez une idée de ce qui a pu déclencher le bug seulement*
```

Liens
=====
* [PokemonWorkshop](http://pokemonworkshop.com/forum/index.php?topic=3314.0)
* [ZesteDeSavoir](https://zestedesavoir.com/forums/sujet/7064/unamed-rebirth/)
* [Trello](https://trello.com/b/JdzEnDJf)
* [Le moteur de scripting conçu pour ce projet](https://github.com/Loodoor/Py3ScriptingEngine)
