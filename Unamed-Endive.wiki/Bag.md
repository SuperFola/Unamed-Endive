Si vous avez besoin d'utiliser le sac grâce au scripting, cela peut être ce que vous cherchez :

Dans `register.py` L48-54, un dictionnaire est défini pour sélectionner facilement une poche, sans connaître par coeur tous les ids :

```py
POCKETS = {
    "Objets": 0,
    "Soins": 1,
    "Balls": 2,
    "Objets rares": 3,
    "Fourre-tout": 4
}
```

Le sac est composé de 5 poches différentes, chacune peut contenir beaucoup d'objets, mais prenez garde quand vous ajoutez un objet, qu'elles ne soient pas remplies complètement !  
Les objets sont regroupés par id, et quand un objet est déposé dans une poche, il peut uniquement être utilisé ou jeté (juste un ou tout), vous ne pouvez pas les déplacer.

Grâce au moteur de script, il est possible de prendre un objet du sac et de le jeter (ou d'en ajouter un nouveau) juste en connaissant son ID. Nul besoin de savoir où il est.
