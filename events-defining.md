Implémentation des évenements (type RMXP)
-----------------------------------------

table de switch :
```
{
    id: True||False
}
```

table de variables :
```
{
    name: value (int/string/n'importe quoi qui peut etre comparé avec >=)
}
```

évenements :
```
{
    name: {
        "switch_1": id d'un switch || False, => si False, pas de switch  |\ switch globaux
        "switch_2": même chose,                                          |/
        "variable": [
            id d'une variable globale,
            valeur minimale à avoir
        ],
        "self_switch": {                            |----------> local à l'évenement, activable via les commandes de script
            id: True || False
        },
        "triggered": True||False,     |-----> utile uniquement pour le jeu qui va activer lui même les events
        "trigger": triggtype,         |-----> si le trigger est lancé, on vérifie switch_1, switch_2, variable
                                            > uniquement s'ils sont spécifiés
        "commands": [... code python par exemple]
    }   |_____________> code a executer si le trigger s'active
}
```

triggtype :
```
[
action button       player touch        event touch
autorun             parallel process    null
]
```

action button => appuie sur une touche d'action (SPACE ou clic)         > fait (SPACE dans DefaultView)
autorun => se lance quand on arrive sur la map de l'event               > fait (quand on change de map, dès le changement, dans Map)
player touch => le joueur entre dans la case où il y a l'event          > fait (Character, quand on change de case seulement)
parallel process => se lance automatiquement en laissant les autres     
                    events tourner en parallèle
event touch => ??                                                       

les event sont stockés par map et par case