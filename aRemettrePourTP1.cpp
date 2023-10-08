//
// Created by Mario Marchand
//

#include "DonneesGTFS.h"
#include <fstream>

using namespace std;


//! \brief ajoute les lignes dans l'objet GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les lignes
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterLignes(const std::string &p_nomFichier)
{
    ifstream fichier(p_nomFichier);

    if (!fichier.is_open())
    {
        throw std::logic_error("Erreur lors de l'ouverture du fichier de lignes.");
    }

    string ligne;
    getline(fichier, ligne); // Ignorer la première ligne (en-têtes)

    while (getline(fichier, ligne))
    {
        stringstream ss(ligne);
        string id, numero, description, categorieStr;
        getline(ss, id, ',');           // Lire l'identifiant de la ligne
        getline(ss, numero, ',');       // Lire le numéro de la ligne
        getline(ss, description, ',');  // Lire la description de la ligne
        getline(ss, categorieStr, ','); // Lire la catégorie de la ligne

        // Convertir la chaîne de caractères en enum CategorieBus
        CategorieBus categorie;
        try
        {
            categorie = Ligne::couleurToCategorie(categorieStr);
        }
        catch (const std::logic_error &e)
        {
            // Gérer l'erreur de catégorie invalide
            std::cerr << "Erreur lors de la conversion de la catégorie : " << e.what() << std::endl;
            continue; // Passer à la ligne suivante dans le fichier
        }

        // Créer un objet Ligne
        Ligne nouvelleLigne(id, numero, description, categorie);

        // Ajouter la nouvelle ligne à m_lignes
        m_lignes[id] = nouvelleLigne;

        // Ajouter la nouvelle ligne à m_lignes_par_numero
        m_lignes_par_numero.insert(std::make_pair(numero, nouvelleLigne));
    }

    fichier.close();
}


//! \brief ajoute les stations dans l'objet GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les stations
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterStations(const std::string &p_nomFichier)
{
    ifstream fichier(p_nomFichier);

    if (!fichier)
    {
        throw logic_error("Erreur lors de l'ouverture du fichier de stations.");
    }

    string ligne;
    getline(fichier, ligne); // Ignorer la première ligne (en-têtes)

    while (getline(fichier, ligne))
    {
        stringstream ss(ligne);
        string id, nom, description, latitude, longitude;
        getline(ss, id, ',');          // Lire l'identifiant de la station
        getline(ss, nom, ',');         // Lire le nom de la station
        getline(ss, description, ',');  // Lire la description de la station
        getline(ss, latitude, ',');     // Lire la latitude de la station
        getline(ss, longitude, ',');    // Lire la longitude de la station

        // Convertir les coordonnées en objet Coordonnees
        Coordonnees coords(stod(latitude), stod(longitude));

        // Créer un objet Station et l'ajouter à l'objet DonneesGTFS
        Station nouvelleStation(id, nom, description, coords);
        m_stations[id] = nouvelleStation;
    }

    fichier.close();
}

//! \brief ajoute les transferts dans l'objet GTFS
//! \breif Cette méthode doit âtre utilisée uniquement après que tous les arrêts ont été ajoutés
//! \brief les transferts (entre stations) ajoutés sont uniquement ceux pour lesquelles les stations sont prensentes dans l'objet GTFS
//! \brief les transferts sont ajoutés dans m_transferts
//! \brief les from_station_id des stations de m_transferts sont ajoutés dans m_stationsDeTransfert
//! \param[in] p_nomFichier: le nom du fichier contenant les station
//! \throws logic_error si un problème survient avec la lecture du fichier
//! \throws logic_error si tous les arrets de la date et de l'intervalle n'ont pas été ajoutés
void DonneesGTFS::ajouterTransferts(const std::string &p_nomFichier)
{

    ifstream fichier(p_nomFichier);

    if (!fichier.is_open())
    {
        throw std::logic_error("Erreur lors de l'ouverture du fichier de transferts.");
    }

    string ligne;
    getline(fichier, ligne); // Ignorer la première ligne (en-têtes)

    while (getline(fichier, ligne))
    {
        vector<string> elements = string_to_vector(ligne, ',');

        // Vérifier que tous les éléments nécessaires sont présents
        if (elements.size() < 3)
        {
            // Ignorer les lignes invalides
            continue;
        }

        string fromStationId = elements[0];
        string toStationId = elements[1];
        unsigned int minTransferTime = stoi(elements[2]);

        // Vérifier si les stations de transfert sont présentes dans l'objet GTFS
        auto fromStationItr = m_stations.find(fromStationId);
        auto toStationItr = m_stations.find(toStationId);

        if (fromStationItr != m_stations.end() && toStationItr != m_stations.end())
        {
            // Ajouter le transfert dans m_transferts
            m_transferts.push_back(std::make_tuple(fromStationId, toStationId, minTransferTime));

            // Ajouter from_station_id dans m_stationsDeTransfert
            m_stationsDeTransfert.insert(fromStationId);
        }
    }

    fichier.close();
}


//! \brief ajoute les services de la date du GTFS (m_date)
//! \param[in] p_nomFichier: le nom du fichier contenant les services
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterServices(const std::string &p_nomFichier)
{
    std::ifstream fichier(p_nomFichier);
    if (!fichier)
    {
        throw std::logic_error("Erreur lors de l'ouverture du fichier de services.");
    }

    std::string ligne;
    while (std::getline(fichier, ligne))
    {
        std::istringstream ss(ligne);
        std::vector<std::string> elements = string_to_vector(ligne, ',');

        if (elements.size() >= 3)
        {
            std::string serviceId = elements[0];
            std::string startDateStr = elements[1];
            std::string endDateStr = elements[2];

            // Extraction de l'année, du mois et du jour du début et de fin à partir des chaînes de caractères
            unsigned int anDebut, moisDebut, jourDebut, anFin, moisFin, jourFin;
            char delimiter;
            std::istringstream ssStartDate(startDateStr);
            std::istringstream ssEndDate(endDateStr);
            ssStartDate >> anDebut >> delimiter >> moisDebut >> delimiter >> jourDebut;
            ssEndDate >> anFin >> delimiter >> moisFin >> delimiter >> jourFin;

            // Création des objets Date
            Date debut(anDebut, moisDebut, jourDebut);
            Date fin(anFin, moisFin, jourFin);

            // Vérification si la date du service est dans la plage d'intérêt (m_now1 et m_now2)
            if (debut < m_date && fin > m_date)
            {
                // La date de début est antérieure à la date d'intérêt, et la date de fin est postérieure,
                // ajouter le service
                m_services.insert(serviceId);
            }
        }
        else
        {
            throw std::logic_error("Format de fichier de services incorrect.");
        }
    }

    fichier.close();
}

//! \brief ajoute les voyages de la date
//! \brief seuls les voyages dont le service est présent dans l'objet GTFS sont ajoutés
//! \param[in] p_nomFichier: le nom du fichier contenant les voyages
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterVoyagesDeLaDate(const std::string &p_nomFichier)
{
    // Lire le fichier des voyages
    std::ifstream fichier(p_nomFichier);
    if (!fichier.is_open()) {
        throw std::logic_error("Erreur lors de l'ouverture du fichier des voyages.");
    }

    std::string ligne;
    std::getline(fichier, ligne); // Ignorer la première ligne (en-têtes)

    // Parcourir le fichier des voyages ligne par ligne
    while (std::getline(fichier, ligne)) {
        std::vector<std::string> tokens = string_to_vector(ligne, ',');

        // Vérifier si le voyage appartient au service de la date actuelle
        if (m_services.find(tokens[1]) != m_services.end()) {
            std::string voyage_id = tokens[0];
            std::string ligne_id = tokens[1];
            std::string service_id = tokens[2];
            std::string destination = tokens[3];

            // Créer le voyage et l'ajouter à m_voyages
            Voyage nouveauVoyage(voyage_id, ligne_id, service_id, destination);
            m_voyages[voyage_id] = nouveauVoyage;
        }
    }

    fichier.close();
}


//! \brief ajoute les arrets aux voyages présents dans le GTFS si l'heure du voyage appartient à l'intervalle de temps du GTFS
//! \brief Un arrêt est ajouté SSI son heure de départ est >= now1 et que son heure d'arrivée est < now2
//! \brief De plus, on enlève les voyages qui n'ont pas d'arrêts dans l'intervalle de temps du GTFS
//! \brief De plus, on enlève les stations qui n'ont pas d'arrets dans l'intervalle de temps du GTFS
//! \param[in] p_nomFichier: le nom du fichier contenant les arrets
//! \post assigne m_tousLesArretsPresents à true
//! \throws logic_error si un problème survient avec la lecture du fichier
void DonneesGTFS::ajouterArretsDesVoyagesDeLaDate(const std::string &p_nomFichier)
{
    // On ouvre le fichier contenant les arrêts
    std::ifstream fichierArrets(p_nomFichier);
    if (!fichierArrets) {
        throw std::logic_error("Impossible d'ouvrir le fichier contenant les arrêts");
    }

    // On lit le fichier arrêt par arrêt
    std::string ligne;
    while (getline(fichierArrets, ligne)) {
        // On parse la ligne
        std::vector<std::string> champs = string_to_vector(ligne, ',');

        // On récupère les informations de l'arrêt
        std::string stationId = champs[0];
        Heure heureDepart(std::stoi(champs[2]), std::stoi(champs[3]), 0);
        Heure heureArrivee(std::stoi(champs[4]), std::stoi(champs[5]), 0);
        unsigned int numeroSequence = std::stoi(champs[4]);
        std::string voyageId = champs[5];

        // On vérifie que l'arrêt est dans l'intervalle de temps
        if (heureDepart >= m_now1 && heureArrivee < m_now2) {
            // On ajoute l'arrêt au voyage correspondant
            auto it = m_voyages.find(voyageId);
            if (it != m_voyages.end()) {
                // On vérifie si le voyage n'a plus d'arrêt
                if (it->second.getArrets().size() == 1) {
                    // On supprime le voyage
                    m_voyages.erase(it);
                }

                it->second.ajouterArret(std::make_shared<Arret>(stationId, heureDepart, heureArrivee, numeroSequence, voyageId));
            }
        }
    }

    // On ferme le fichier arrêts
    fichierArrets.close();

    // On supprime les stations qui n'ont pas d'arrêts
    for (auto it = m_stations.begin(); it != m_stations.end();) {
        if (it->second.getArrets().empty()) {
            it = m_stations.erase(it);
        } else {
            it++;
        }
    }
}
