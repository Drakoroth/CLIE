Exécuter le Makefile avec la commande "make" pour créer l'exécutable

COMMANDES CLIE -


PARTIE CHARGEMENT ET SAUVEGARDE D'IMAGES

open fen			: ouvre une fenêtre vide
open [nom_img1]	[nom_img2...]	: ouvre une ou plusieurs images (d'extension .bmp, .jpg, .jpeg, .png)
open in [id] [nom_img]		: ouvre l'image nom_img dans une fenêtre d'id déjà ouverte
transfert [id1] [id2]		: transfert l'image d'id1 dans la fenêtre d'id2 (et vide ainsi la fenêtre d'id1)
transfert [id]			: transfert l'image d'id dans une nouvelle fenêtre (et vide ainsi la fenêtre d'id)
save [id]			: enregistre la photo d'id sous son nom initial
save [id] [nom_img]		: enregistre la photo d'id sous nom_img



PARTIE TRANSFORMATIONS

rotate [id] [angle]		: rotation de l'image d'id sous un angle qui doit être multiple de 90
sym vert [id]			: symétrie verticale de l'image d'id
sym horiz [id]			: symétrie horizontale de l'image d'id
negative [id]			: mise en négative de l'image d'id
gris [id]			: mise en niveau de gris de l'image d'id
remplissage [id] [couleur]	: remplissage de l'image d'id pour une couleur donnée (bleu/rouge/vert/jaune/orange/rose/violet)
remplissage [id] [-t] [couleur]	: remplissage par transparence de l'image d'id pour une couleur donnée (blanc/bleu/rouge/vert)
lum [id] [intensité +/-]	: luminosité de l'image d'id
contraste [id] [instensité +/-]	: contraste de l'image d'id
flou [id]			: floutage de l'image d'id



PARTIE SELECTION

select [id]			: permet le focus de l'image d'id pour avoir accès aux fonctionnalités suivantes...
				  - clic droit pour marquer l'endroit où l'on souhaite coller
				  - clic gauche pour sélectionner une partie de l'image à coller
				  - touche C pour coller aux coordonnées du dernier clic droit
				  - touche X pour couper (fond noir) la dernière zone sélectionnée
				  - touche R pour réinitialiser l'image (avant les modifications) et retourner au terminal
				  - touche D pour déselectionner l'image et retourner au terminal



AUTRES

close [id1] [id2...]		: ferme une ou plusieurs fenêtres
exit				: quitte le programme
reset [id]			: charge l'image d'id à son état d'origine (avant toutes modifications effectuées)


