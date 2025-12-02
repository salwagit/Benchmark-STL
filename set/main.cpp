#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <set>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// =============================
// ========= PROGRAMME =========
// =============================
int main() {
    const int nombreElements = 10000000;
    const string nomFichier = "donnees.txt";

    set<string> arbreSet;  // set est implémenté comme un arbre rouge-noir

    cout << "Verification du fichier " << nomFichier << "...\n";

    // === Génération du fichier ===
    if (!fs::exists(nomFichier)) {
        cout << "Fichier introuvable. Generation de " << nombreElements << " mots...\n";
        ofstream fichier(nomFichier);
        for (int i = 0; i < nombreElements; ++i) fichier << "mot_" << i << "\n";
        fichier.close();
        cout << "Fichier genere.\n";
    } else {
        cout << "Fichier deja existant.\n";
    }

    // === Lecture des données ===
    ifstream fichier(nomFichier);
    if (!fichier.is_open()) {
        cerr << "Erreur: impossible d'ouvrir le fichier.\n";
        return 1;
    }

    cout << "\nInsertion dans le set...\n";
    auto debutInsertion = high_resolution_clock::now();

    string mot;
    int compteur = 0;
    while (fichier >> mot && compteur < nombreElements) {
        arbreSet.insert(mot);  // Insertion dans le set
        compteur++;
    }
    fichier.close();

    auto finInsertion = high_resolution_clock::now();
    auto dureeInsertion = duration_cast<milliseconds>(finInsertion - debutInsertion);
    cout << "Insertion terminee. (" << compteur << " mots lus)\n";
    cout << "Elements inseres: " << arbreSet.size() << " (sans doublons)\n";
    cout << "Temps: " << dureeInsertion.count() << " ms\n";

    // === Recherche ===
    string motRecherche = "mot_" + to_string(nombreElements / 2);
    cout << "\nRecherche de \"" << motRecherche << "\"...\n";

    auto debutRecherche = high_resolution_clock::now();
    bool trouve = (arbreSet.find(motRecherche) != arbreSet.end());  // Recherche avec find()
    auto finRecherche = high_resolution_clock::now();
    auto dureeRecherche = duration_cast<microseconds>(finRecherche - debutRecherche);

    cout << (trouve ? "Mot trouve." : "Mot non trouve.") << "\n";
    cout << "Temps: " << dureeRecherche.count() << " us\n";

    // === Tests de recherche supplémentaires ===
    cout << "\n=== Tests de recherche supplementaires ===" << endl;
    
    // Recherche d'un élément qui devrait exister
    string motExistant = "mot_500000";
    auto debut1 = high_resolution_clock::now();
    bool existe = (arbreSet.find(motExistant) != arbreSet.end());
    auto fin1 = high_resolution_clock::now();
    cout << "Recherche \"" << motExistant << "\": " 
         << (existe ? "trouve" : "non trouve") 
         << " en " << duration_cast<microseconds>(fin1 - debut1).count() << " us" << endl;

    // Recherche d'un élément qui ne devrait pas exister
    string motInexistant = "mot_invente_12345";
    auto debut2 = high_resolution_clock::now();
    bool existe2 = (arbreSet.find(motInexistant) != arbreSet.end());
    auto fin2 = high_resolution_clock::now();
    cout << "Recherche \"" << motInexistant << "\": " 
         << (existe2 ? "trouve" : "non trouve") 
         << " en " << duration_cast<microseconds>(fin2 - debut2).count() << " us" << endl;

    // === Parcours ===
    cout << "\nParcours du set...\n";
    auto debutParcours = high_resolution_clock::now();
    
    // Parcours avec itérateur (déjà trié car set est ordonné)
    for (const auto& element : arbreSet) {
        // Accès à l'élément
        // cout << element << " "; // Désactivé pour performance
    }
    
    auto finParcours = high_resolution_clock::now();
    auto dureeParcours = duration_cast<milliseconds>(finParcours - debutParcours);

    cout << "Temps de parcours: " << dureeParcours.count() << " ms\n";

    // === Statistiques ===
    cout << "\n=== Statistiques ===" << endl;
    cout << "Taille du set: " << arbreSet.size() << " elements" << endl;
    
    if (!arbreSet.empty()) {
        cout << "Plus petit element: " << *arbreSet.begin() << endl;
        cout << "Plus grand element: " << *arbreSet.rbegin() << endl;
    }

    // Vérification de l'absence de doublons
    if (arbreSet.size() == nombreElements) {
        cout << "Aucun doublon detecte." << endl;
    } else if (arbreSet.size() < nombreElements) {
        cout << "Attention: " << nombreElements - arbreSet.size() 
             << " doublons ont ete elimines (comportement normal du set)." << endl;
    }

    // === Test de comptage (méthode alternative) ===
    cout << "\nTest avec count() (methode alternative):" << endl;
    auto debutCount = high_resolution_clock::now();
    size_t nbOccurrences = arbreSet.count(motRecherche);
    auto finCount = high_resolution_clock::now();
    cout << "count(\"" << motRecherche << "\") = " << nbOccurrences 
         << " en " << duration_cast<microseconds>(finCount - debutCount).count() << " us" << endl;

    return 0;
}