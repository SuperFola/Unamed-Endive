Si vous utilisez l'éditeur de scripts, vous voudrez certainement créer votre propre PNJ en Python, vous avez donc besoin de quelques informations pour cela.

Type de PNJ
========
`0` est un `PNJkind::normal` (Tous contenus dans le dossier 'pnj')

`1` est un `PNkind::special` (Ils sont tous contenus dans le sous-dossier `.specials` du dossier `pnj`)

`2` est un `PNJkind::system` (Tous contenus dans le dossier 'pnj')

Le nom et son affichage
=====================
Le nom du PNJ est utilisé pour trouver le sous-dossier (soit dans `pnj` soit dans `pnj/.specials`) qui contient toutes les images des PNJ.

Le nom affiché du PNJ est le nom qui sera affiché lorsque vous parlerez au PNJ.
