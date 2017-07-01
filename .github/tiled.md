# Configurer Tiled pour créer des maps pour le jeu

Téléchargez l'éditeur [ici](https://thorbjorn.itch.io/tiled/download/)

Téléchargez le convertisseur [ici]()

**LE CONVERTISSEUR N'EST PAS ENCORE CODE MAIS VOUS POUVEZ DEJA MAPPER**

Lancez l'installateur, acceptez tout ce qu'il veut, puis lancez Tiled.

Allez dans Fichier > Nouveau > Nouveau Tileset :

- nommez le par un nombre (très important pour la suite). Ce nombre est l'id unique de la carte (il ne doit pas être utilisé par une autre carte du jeu)

- comme source, indiquez l'emplacement du tileset du jeu (dans dossier_d'installation/assets/tileset.png)

- largeur : 16 pix, hauteur : 16 pix

- marge : 0 pix, espacement : 1 pix

Faites enregistrer sous ... et sauvegardez le précieusment (il ne faudra sous aucun prétexte le modifier).

Puis retournez dans Fichier > Nouveau puis choisissez Nouvelle carte cette fois-ci :

- orientation : orthogonale

- format de calque de Tile : CSV (très très très important !)

- ordre d'affichage des tiles : en haut à gauche

- taille de la carte :

    - largeur : le nombre de tiles que vous voulez
    
    - hauteur : idem

- taille des éléments (Tiles) :

    - largeur : 16 pix
    
    - hauteur : 16 pix

Faites enregistrer sous ... et donnez lui un nom.

Maintenant il faut configurer le nombre de calques :

- à droite, dans la section calques :

    - clic droit > Nouveau > Calque de Tiles
    
        - CHAQUE CALQUE DOIT IMPERATIVEMENT ETRE NOMME COMME TEL : `calque[NUMERO]` (pas d'espace)
        
        - n'hésitez donc pas à renommer un calque (sélectionnez le, puis à gauche, section Propriétés > Nom)
        
        - créez 3 calques
    
    - le `calque1` est le premier layer affiché (au dessus du joueur même)
    
    - le `calque2` est directement sous les pieds du joueur
    
    - le `calque3` est en dessous du calque 2
    
- le `calque4` est spécial, car c'est lui qui va définir les téléporteurs, il faut faire clic droit > Nouveau > Calque d'Objets

Définir les données pour le calque de téléporteurs (obligatoire) :

- allez dans Vue puis cochez Editeur de Types d'Objets

    - une fenêtre s'ouvre :
    
        - en bas à gauche, faite le petit plus :
        
            - type : tp
            
            - couleur : peu importe
            
            - gardez le sélectionné et faites le petit plus en bas à droite :
            
                - dans la boite Ajouter une propriété :
                
                    - écrivez `x` et comme type : int
                    
                    - faites OK
                    
                - laissez la valeur par défaut
                
                - faites pareil et ajoutez `y` (type int), `x_dest` (type int), `y_dest` (type int), `dest_map_id` (type int)
                
    - fermez cette fenêtre

Sur votre `calque4`, placez les objets (carrés, 1 case sur 1, réglez la largeur en 16 pix et la hauteur en 16 pix dans la colonne propriété de gauche) que vous voulez pour indiquer un téléporteur, puis pour indiquer sa destination :

- sélectionnez votre objet (doit être exactement placé sur une case, au pixel près)

- colonne de gauche, Propriétés :

    - nommez votre objet (c'est toujours mieux)
    
    - dans type, tapez `tp` (il doit vous être proposé comme nous l'avons créé ensemble)
    
    - remplissez les propriétés `x_dest` et `y_dest` en NOMBRE DE CASES par rapport au coin supérieur gauche de la map de destination, `dest_map_id`

Et maintenant vous pouvez mapper :smile: !

Quand vous avez fini, sauvegardez, copiez vos fichiers de maps dans le dossier `maps` du convertisseur, puis lancez le convertisseur. Vos maps seront converties selon ce modèle de nommage : `id donné.umd`

Copiez les dans le dossier `assets/map` du jeu et tout est bon !

**NOTE** : il n'est pas possible d'éditer une map convertie. Pour cela, gardez **toujours** les .tmx pour éditer vos maps, vous n'aurez qu'à reconvertir une fois vos maps une fois que vous les aurez éditées.

















