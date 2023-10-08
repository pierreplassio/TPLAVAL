//
// Created by Mario Marchand.
//

#ifndef TP1_GTFS_H
#define TP1_GTFS_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <sstream>

#include "auxiliaires.h"
#include "ligne.h"
#include "station.h"
#include "voyage.h"
#include "arret.h"
#include "coordonnees.h"

class DonneesGTFS
{

public:
    DonneesGTFS(const Date&, const Heure&, const Heure&);

    void ajouterLignes(const std::string &);
    void ajouterStations(const std::string &);
    void ajouterServices(const std::string &);
    void ajouterVoyagesDeLaDate(const std::string &);
    void ajouterArretsDesVoyagesDeLaDate(const std::string&);
    void ajouterTransferts(const std::string&);

    void afficherLignes() const;
    void afficherStations() const;
    void afficherArretsParVoyages() const;
    void afficherArretsParStations() const;
    void afficherTransferts() const;
    void afficherStationsDeTransfert() const;

    Heure getTempsDebut() const;
    Heure getTempsFin() const;
    size_t getNbLignes() const;
    unsigned int getNbArrets() const;
    size_t getNbStations() const;
    size_t getNbServices() const;
    size_t getNbVoyages() const;
    size_t getNbTransferts() const;
    size_t getNbStationsDeTransfert() const;
    const std::map<std::string, Voyage> & getVoyages() const;
    const std::map<std::string, Station> & getStations() const;
    const std::unordered_map<std::string, Ligne> & getLignes() const;
    const std::set<std::string> & getStationsDeTransfert() const;
    const std::vector<std::tuple<std::string, std::string, unsigned int> > & getTransferts() const;

private:

    std::vector<std::string> string_to_vector(std::string &s, char delim);

    Date m_date; //la date d'intérêt
    Heure m_now1;  //l'heure de début d'intérêt (à partir de laquelle on considère les arrêts)
    Heure m_now2;  //l'heure de fin d'intérêt (à partir de laquelle on ne considère plus les arrêts

    unsigned int m_nbArrets; //le nombre d'arrets au total présents dans cet objet
    bool m_tousLesArretsPresents; //indique si tous les arrêts de la date et de l'intervalle [now1, now2) ont été ajoutés

    std::unordered_map<std::string, Ligne> m_lignes; //la clé string est l'identifiant m_id de l'objet Ligne
    std::map<std::string, Station> m_stations; //la clé string est l'identifiant m_id de l'objet Station
    std::unordered_set<std::string> m_services; //le string est l'identifiant du service (service_id)
    std::map<std::string, Voyage> m_voyages; //le string est l'identifiant (trip_id) de l'objet Voyage
    std::vector<std::tuple<std::string, std::string, unsigned int> > m_transferts; // <from_station_id, to_station_id, min_transfer_time>
    std::set<std::string> m_stationsDeTransfert; //Chaque élément est l'identifiant from_station_id d'une station présente dans m_transferts

    std::multimap<std::string, Ligne> m_lignes_par_numero; //le string est l'attribut m_numero de l'objet ligne

};

#endif //TP1_GTFS_H
