# Miniprojet de RO

Ce dossier contient les données + éléments de codes pour le projet du cours de RO de l'ENPC.

Le sujet du projet dans le fichier project.pdf

Le dossier input/common/ contient les instances communes à tous les groupes. Chaque instance est composés de 3 fichiers:
 - ***_fenetres.csv 
 - ***_voletsGauches.csv
 - ***_voletsDroits.csv
qui donnent respectivement les caractéristiques des fenêtres, des volets gauches et des volets droits.

Les fichiers de solutions rendus doivent être au même format que ceux du dossier output/examples.

Le fichier valeursSolutionRetourneesParUneHeuristiqueSimple.txt vous donne les valeurs des solutions obtenues en utilisant une heuristique simple avec un temps de calcul de 1 min pour les instances versaillesFeasible et versaillesOpt.csv, et 5s pour toutes les instances.

Le dossier src/ contient le code source du programme evaluateSolution, qui permet d'évaluer une solution. Lancer le programme sans argument vous donnera la commande à utiliser. Le Makefile vous permet de compiler ce programme sous linux.

Le dossier output/examples/ contient les fichiers solutions des petites instances.
