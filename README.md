# PFE
# Projet de Fin d'Études : Ferme Intelligente

## Description

Ce projet de **ferme intelligente** utilise des **microcontrôleurs ESP32**, **ESP8266**, et des **cartes Arduino** pour automatiser diverses fonctions de gestion de la ferme. L'objectif principal est de rendre l'agriculture plus efficace, durable et accessible en automatisant les processus critiques de gestion tels que l'accès sécurisé à la ferme, l'irrigation automatique, la surveillance de l'environnement et de la qualité de l'air, ainsi que l'alimentation automatique des animaux. Le projet est conçu pour être modulaire, évolutif et facile à entretenir.

## Fonctionnalités

### 1. Zone 1 : Accès Sécurisé à la Ferme
- **Accès sécurisé à la serre** : Un système RFID permet de contrôler l'accès à la serre, en activant un servomoteur pour ouvrir automatiquement la porte lorsque des badges autorisés sont détectés.
- **Accès sécurisé à la chambre de sécurité** : Un contrôle similaire est mis en place pour garantir la sécurité de zones sensibles, telles que la chambre de sécurité de la ferme.
- **Alimentation automatique dans l’étable** : Des capteurs et des actionneurs permettent de gérer l'alimentation des animaux automatiquement, en fonction des besoins détectés par des capteurs de poids ou de niveau de stock.

### 2. Zone 2 : Irrigation et Surveillance de l'Environnement
- **Irrigation automatique des plantes** : Des capteurs d'humidité du sol détectent les niveaux d'humidité et régulent l'irrigation de manière autonome pour assurer la santé des plantes.
- **Surveillance de la qualité de l'air dans la serre** : Des capteurs de gaz (CO₂, ammoniac) permettent de mesurer la qualité de l'air dans la serre, en régulant l'aération ou en activant un système d'aspiration si nécessaire.

### 3. Zone 3 : Surveillance d'Atmosphère et Ouverture de Toit
- **Surveillance de l'atmosphère de la ferme** : Des capteurs de température et d'humidité surveillent en permanence les conditions environnementales à l'intérieur de la ferme pour garantir des conditions optimales.
- **Ouverture automatique du toit de l’étable** : Un système automatisé permet d'ouvrir le toit de l’étable en fonction des conditions météorologiques ou de l'humidité intérieure.

## Technologies utilisées

- **Microcontrôleurs** : ESP32, ESP8266, Arduino (utilisé pour diverses tâches comme la gestion des capteurs et des actionneurs)
- **Protocoles de communication** : MQTT, HTTP, WebSocket pour la communication entre les capteurs et l’application centrale.
- **Capteurs** : RFID, capteurs de température et d'humidité, capteurs de mouvement, capteurs de gaz (CO₂, ammoniac), capteurs d'humidité du sol, capteurs de lumière.
- **Actionneurs** : Servomoteurs, vannes d'irrigation, LED, moteurs pour l'ouverture automatique du toit.
- **Base de données** :MySQL.
- **Frameworks** : Django (pour le back-end web), Bootstrap (pour le front-end web).
- **Outils de développement** : Arduino IDE, Visual Studio Code.

## Installation

### Prérequis

- Arduino IDE installé sur votre machine avec les bibliothèques pour ESP32, ESP8266 et Arduino.
- Serveur web pour héberger l’application Django.
- Python 3.x et pip pour installer les dépendances Django.
- MQTT Broker (Mosquitto recommandé) pour la gestion des communications des capteurs.

### Étapes d'installation

1. **Cloner le dépôt GitHub** :

```bash
git clone https://github.com/salmaBouabdallaoui/PFE.git

