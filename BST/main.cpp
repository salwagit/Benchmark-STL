#include <iostream>
#include <fstream>
#include <stack>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <filesystem>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// =============================
// === Classe NoeudArbre ====
// =============================
class NoeudArbre {
public:
    string valeur;
    NoeudArbre* pere;
    NoeudArbre* gauche;
    NoeudArbre* droite;

    NoeudArbre(const string& donnee) {
        valeur = donnee;
        pere = gauche = droite = nullptr;
    }
};

// =============================
// ==== Classe ArbreBR ===
// =============================
class ArbreBR {
private:
    NoeudArbre* racine;

    NoeudArbre* trouverMinimum(NoeudArbre* noeudActuel) {
        while (noeudActuel->gauche != nullptr)
            noeudActuel = noeudActuel->gauche;
        return noeudActuel;
    }

public:
    ArbreBR() { racine = nullptr; }

    // --- Insertion itÃ©rative ---
    void insererIteratif(const string& donnee) {
        NoeudArbre* parentActuel = nullptr;
        NoeudArbre* noeudCourant = racine;
        NoeudArbre* nouveauNoeud = new NoeudArbre(donnee);

        while (noeudCourant != nullptr) {
            parentActuel = noeudCourant;
            if (nouveauNoeud->valeur < noeudCourant->valeur)
                noeudCourant = noeudCourant->gauche;
            else if (nouveauNoeud->valeur > noeudCourant->valeur)
                noeudCourant = noeudCourant->droite;
            else
                return; // Ã©viter doublons
        }

        nouveauNoeud->pere = parentActuel;

        if (parentActuel == nullptr)
            racine = nouveauNoeud;
        else if (nouveauNoeud->valeur < parentActuel->valeur)
            parentActuel->gauche = nouveauNoeud;
        else
            parentActuel->droite = nouveauNoeud;
    }

    // --- Recherche itÃ©rative ---
    bool chercherIteratif(const string& donnee) {
        NoeudArbre* noeudCourant = racine;
        while (noeudCourant != nullptr && donnee != noeudCourant->valeur) {
            if (donnee < noeudCourant->valeur)
                noeudCourant = noeudCourant->gauche;
            else
                noeudCourant = noeudCourant->droite;
        }
        return noeudCourant != nullptr;
    }

    // --- Parcours infixe (itÃ©rative) ---
    void parcoursInfixeIteratif() {
        stack<NoeudArbre*> pileNoeuds;
        NoeudArbre* noeudActuel = racine;
        while (noeudActuel != nullptr || !pileNoeuds.empty()) {
            while (noeudActuel != nullptr) {
                pileNoeuds.push(noeudActuel);
                noeudActuel = noeudActuel->gauche;
            }
            noeudActuel = pileNoeuds.top();
            pileNoeuds.pop();
            // cout << noeudActuel->valeur << " ";
            noeudActuel = noeudActuel->droite;
        }
    }
};

// =============================
// ========= PROGRAMME ==============
// =============================
int main() {
    ArbreBR monArbre;

    const int nombreElements = 10000000;
    string nomFichier = "donnees.txt";

    cout << "Verification du fichier " << nomFichier << "...\n";

    // === Ã‰tape 1 : GÃ©nÃ©ration du fichier ===
    if (!fs::exists(nomFichier)) {
        cout << "Fichier introuvable. Generation de " << nombreElements << " mots...\n";

        ofstream fichierSortie(nomFichier);
        if (!fichierSortie.is_open()) {
            cerr << "Erreur: impossible de creer le fichier " << nomFichier << endl;
            return 1;
        }

        for (int compteur = 0; compteur < nombreElements; ++compteur)
            fichierSortie << "mot_" << compteur << "\n";

        fichierSortie.close();
        cout << "Fichier genere avec succes.\n";
    } else {
        cout << "Fichier deja existant. Generation ignoree.\n";
    }

    // === Ã‰tape 2 : Lecture et insertion ===
    ifstream fichierEntree(nomFichier);
    if (!fichierEntree.is_open()) {
        cerr << "Erreur: impossible d'ouvrir " << nomFichier << endl;
        return 1;
    }

    cout << "\nInsertion dans l'arbre (" << nombreElements << " elements)...\n";

    auto debutInsertion = high_resolution_clock::now();

    string motCourant;
    int compteurMots = 0;
    while (fichierEntree >> motCourant && compteurMots < nombreElements) {
        monArbre.insererIteratif(motCourant);
        compteurMots++;
    }

    auto finInsertion = high_resolution_clock::now();
    auto dureeInsertion_ms = duration_cast<milliseconds>(finInsertion - debutInsertion);
    double dureeInsertion_s = dureeInsertion_ms.count() / 1000.0;

    fichierEntree.close();

    cout << "\nInsertion terminee.\n";
    cout << "Total de mots inseres: " << compteurMots << endl;
    cout << "Temps total d'insertion: " << dureeInsertion_ms.count() << " ms ("
         << dureeInsertion_s << " s)\n";

    // === Ã‰tape 3 : Mesure du temps de recherche ===
    string motAChercher = "mot_" + to_string(nombreElements / 2);
    cout << "\nRecherche de \"" << motAChercher << "\"...\n";

    auto debutRecherche = high_resolution_clock::now();
    bool trouve = monArbre.chercherIteratif(motAChercher);
    auto finRecherche = high_resolution_clock::now();
    auto dureeRecherche_us = duration_cast<microseconds>(finRecherche - debutRecherche);

    cout << (trouve ? "Mot trouve." : "Mot non trouve.") << endl;
    cout << "Temps de recherche: " << dureeRecherche_us.count() << " microSec ("
         << dureeRecherche_us.count() / 1000.0 << " ms)\n";

    // === Ã‰tape 4 : Mesure du temps de parcours ===
    cout << "\nParcours en cours...\n";
    auto debutParcours = high_resolution_clock::now();
    monArbre.parcoursInfixeIteratif();
    auto finParcours = high_resolution_clock::now();
    auto dureeParcours_ms = duration_cast<milliseconds>(finParcours - debutParcours);
    double dureeParcours_s = dureeParcours_ms.count() / 1000.0;

    cout << "Temps de parcours infixe: " << dureeParcours_ms.count() << " ms ("
         << dureeParcours_s << " s)\n";



    return 0;
}