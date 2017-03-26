Implémentation des évenements (type RMXP)
-----------------------------------------

table de switch :
```
{
    id: {
        "name": name,
        "on": True || False
    }
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
            expression python testant la valeur (et renvoyant un booléen)
        ],
        "self_switch": id switch local,                            |----------> local à l'évenement, activable via les commandes de script
        "trigger": triggtype,                                      |-----> si le trigger est lancé, on vérifie switch_1, switch_2, variable
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