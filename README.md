# Recupérer des données depuis le port Série et les formater pour les rendre compatibles à l'exploitation du site electrorun.

*__Electrorun__* est un site de course à pied particulière : (révélé en JUIN 2024).  
Vous trouverez ici de quoi lire des données depuis le port série de votre ordinateur (notamment pour récupérer et stocker des données provenant d'une carte Arduino) : c'est le rôle du fichier *recup_serial_data.py*. Il contient quelques autres fonctions utiles (afficher tous les ports disponibles par exemple).  
Ensuite, le fichier *use_data.py* est celui qui va utiliser les données stockées sur l'ordinateur avec le code précédent pour les formater à un format nécessaire pour être exploité par Electrorun.  
Enfin le fichier *main.py* combine les deux précédents, c'est la fonction main qu'il faut appeler pour réaliser le bon formatage. Le fichier .erun créé sur votre ordinateur sera celui à téléverser dans l'onglet "ajouter une prestation" du site.  
Retrouvez le site à cette addresse : [Electrorun](addresse).
