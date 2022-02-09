# webserv_local

Création d'un serveur HTTP type nginx en C++ 98 :

- Interdiction d'utiliser des librairies externes
- Le serveur est paramétrable via un fichier de configuration (type nginx) qui est parsé au lancement
- Méthodes gérées : GET, POST et DELETE
- Possibilité d'exécuter des CGIs
- Le serveur doit rester disponible à tout prix sous stress test
