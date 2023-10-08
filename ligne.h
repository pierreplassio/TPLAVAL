//
// Created by prtos
// Modified by mario.
//

#ifndef RTC_LIGNE_H
#define RTC_LIGNE_H

#include <string>
#include <vector>
#include<iostream>
#include <exception>
#include <algorithm>

/*!
 * \enum CategorieBus
 * \brief Représente les différentes catégories de bus
 *
 */
enum class CategorieBus {METRO_BUS, LEBUS, EXPRESS, COUCHE_TARD, BUS_A_VENIR};

//class Voyage;

/*!
 * \class Ligne
 * \brief Cette classe représente une ligne d'autobus du réseau de transport contenues dans le fichier routes.txt
 * Par exemple, la "800", la "801" et la "13A" sont toutes des lignes différentes.
 * Veuillez, noter qu’il y peut y avoir plus d'une  entrées dans le fichier routes.txt pour chaque ligne d’autobus.
 * Cela est dû au fait qu'un dossier gtfs puisse cheuvaucher deux saisons (des saisons différentes donnent des ids différents à une même ligne).
 * Ceci qui implique qu’un numéro de ligne est associé à deux identifiants différents, cependant vous ne devez construire qu'un seul
 * objet ligne par ligne d'autobus.
 * \note Sa déclaration nécessite la déclaration partielle des classe Voyage, en raison de référence circulaire avec cette classe.
 */
class Ligne {

public:
    Ligne(std::string p_id, const std::string & p_numero, const std::string & p_description, const CategorieBus& p_categorie);
	Ligne();
    static CategorieBus couleurToCategorie(const std::string & couleur);
    static std::string categorieToString(const CategorieBus & c);
	CategorieBus getCategorie() const;
	std::string getId() const;
	std::string getNumero() const;
	const std::string& getDescription() const;
	friend std::ostream& operator <<(std::ostream& f, const Ligne& p_ligne);

private:
	std::string m_id;
	std::string m_numero;
	std::string m_description;
	CategorieBus m_categorie;

};


#endif //RTC_LIGNE_H
