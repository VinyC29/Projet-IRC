```mermaid
sequenceDiagram

    Utilisateur->>+Interface: Écris un username et trois nick name dans les cases

    Utilisateur->>+Interface: Ajout / supression de network au besoin

    Utilisateur->>+Interface: L'utilisateur edite le network au besoin

    Utilisateur->>+Interface: L'utilisateur se connecte au serveur

    Interface->>+Serveur: Envoie les informations de l'utilisateur au network

    Serveur-->>-Interface: Envoie si la connection a échoué / réussi

    Interface-->>-Utilisateur: Affiche les résultats de la connexion au serveur

    Utilisateur->>+Interface: Choisie un channel a rejoindre

    Interface->>+Serveur: Envoie la requête pour rejoindre

    Serveur-->>-Interface: Se connecte au channel en vérifiant la disponibilité des nick name

    Interface-->>-Utilisateur: Affiche si la connexion réussi avec quel user name où si tous les usernames sont utilisé
```   
