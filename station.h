//
// Created by prtos.
// Modified by Mario.
//

#ifndef RTC_STATION_H
#define RTC_STATION_H

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>
#include "coordonnees.h"
#include "arret.h"
#include "auxiliaires.h"

/*!
 * \class Station
 * \brief Classe représentant une station. Une station est un emplacement physique où un bus effectue des arrêts.
 * \note Sa déclaration nécessite la déclaration partielle des classe Ligne et Voyage, en raison de références circulaires avec ces classes.
 */
class Station {


public:
    Station(std::string p_id, const std::string & p_nom, const std::string & p_description,const Coordonnees & p_coords);
    Station();
    friend std::ostream& operator<<(std::ostream& flux, const Station& p_station);
	const Coordonnees& getCoords() const;
	const std::string& getDescription() const;
	const std::string& getNom() const;
	std::string getId() const;
    void addArret(const Arret::Ptr & p_arret);
    unsigned int getNbArrets() const;
    const std::multimap<Heure, Arret::Ptr> & getArrets() const;

private:
    std::string m_id;
    std::string m_nom;
    std::string m_description;
    Coordonnees m_coords;
    std::multimap<Heure, Arret::Ptr> m_arrets;

};


#endif //RTC_STATION_H
