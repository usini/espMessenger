/* English Language for ESPMessenger
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/
#include <Arduino.h>
#include "../web/web_index_fr.h"
#include "../web/web_lang_fr.h"

//banner.h
const String TEXT_INTRO = "|Un afficheur connecté à base de Matrice 8x8 fait par usini|";
const String TEXT_CONNECTION = "~~~~~~~~~~  Connexion ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

//matrix.h
const String TEXT_MESSAGE = "--> Message : ";
const String TEXT_MESSAGE_TOO_LONG = "--> ERREUR Message trop long!";

//settings.h
const String TEXT_MOUNT_SPIFFS = "*ST: Montage du système de fichier";
const String TEXT_READ_CONFIG_FILE = "*ST: Lecture de settings.json";
const String TEXT_OPEN_CONFIG_FILE = "*ST: Ouverture de settings.json";
const String TEXT_PARSING_JSON = "\n*ST:Analyse du JSON";
const String TEXT_NB_MATRIX = "*ST: Matrices: ";
const String TEXT_LOAD_CONFIG_FILE = "*ST: Echec de la lecture du JSON";
const String TEXT_MOUNT_FS = "*ST: Echec du montage du système de fichier";

const char* TEXT_NAME = "Nom";
const char* TEXT_USER = "Utilisateur";
const char* TEXT_PASSWORD = "Mot de passe";
const char* TEXT_MATRIX = "Matrices";

const String TEXT_TIMEOUT = "*ST: Echec de la connexion et délai d'attente dépassé";
const String TEXT_CONNECTED = "*ST: Connecté :-)";
const String TEXT_SAVING_CONFIG = "*ST: Sauvegarde de settings.json";
const String TEXT_FAILED_OPEN_CONFIG = "*ST: Echec de l'écriture de settings.json";

//web.h
const String TEXT_COMMAND = "Commande:";
const String TEXT_WEB_MESSAGE = "Message Web:";
const String TEXT_MESSAGE_SENT = "Message envoye";
const String TEXT_NOT_FOUNDED = "404 Page non trouvee";
const String TEXT_OPEN_UPDATE = "Ouverture de update.bin";
const String TEXT_ERROR_UPDATE = "ERREUR Impossible d'ouvrir update.bin";
const String TEXT_BEGIN_UPDATE = "Début de la mise à jour...";
const String TEXT_UPDATE_SUCCESS = "Mise à jour effectué: \nRedémarrage...\n";
const String TEXT_UPDATE_END = "Mise à jour fini...";
const String TEXT_UPDATE_FINISHED = "Téléversement fini, démarrage de la mise à jour...";
const String TEXT_UPDATE_FAILED = "Echec du téléversement ...";
const String TEXT_WEB_STARTED = "... Démarrage du serveur web .. ";
