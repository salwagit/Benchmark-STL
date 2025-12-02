#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <map>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// =============================
// ========= PROGRAMME =========
// =============================
int main() {
    const int nombreElements = 10000000;
    const string nomFichier = "donnees.txt";

    map<string, bool> arbreMap;  // map est implémenté comme un arbre rouge-noir

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

    cout << "\nInsertion dans la map...\n";
    auto debutInsertion = high_resolution_clock::now();

    string mot;
    int compteur = 0;
    while (fichier >> mot && compteur < nombreElements) {
        arbreMap[mot] = true;  // Insertion avec opérateur []
        compteur++;
    }
    fichier.close();

    auto finInsertion = high_resolution_clock::now();
    auto dureeInsertion = duration_cast<milliseconds>(finInsertion - debutInsertion);
    cout << "Insertion terminee. (" << compteur << " mots)\n";
    cout << "Temps: " << dureeInsertion.count() << " ms\n";

    // === Recherche ===
    string motRecherche = "mot_" + to_string(nombreElements / 2);
    cout << "\nRecherche de \"" << motRecherche << "\"...\n";

    auto debutRecherche = high_resolution_clock::now();
    bool trouve = (arbreMap.find(motRecherche) != arbreMap.end());  // Recherche avec find()
    auto finRecherche = high_resolution_clock::now();
    auto dureeRecherche = duration_cast<microseconds>(finRecherche - debutRecherche);

    cout << (trouve ? "Mot trouve." : "Mot non trouve.") << "\n";
    cout << "Temps: " << dureeRecherche.count() << " us\n";

    // === Parcours ===
    cout << "\nParcours de la map...\n";
    auto debutParcours = high_resolution_clock::now();
    
    // Parcours avec itérateur
    for (const auto& paire : arbreMap) {
        // Accès à paire.first (clé) si nécessaire
        // cout << paire.first << " "; // Désactivé pour performance
    }
    
    auto finParcours = high_resolution_clock::now();
    auto dureeParcours = duration_cast<milliseconds>(finParcours - debutParcours);

    cout << "Temps de parcours: " << dureeParcours.count() << " ms\n";

    // === Statistiques supplémentaires (optionnel) ===
    cout << "\n=== Statistiques ===" << endl;
    cout << "Taille de la map: " << arbreMap.size() << " elements" << endl;
    
    // Vérification de l'absence de doublons (optionnel)
    if (arbreMap.size() == nombreElements) {
        cout << "Aucun doublon detecte." << endl;
    } else {
        cout << "Doublons detectes: " << nombreElements - arbreMap.size() << " elements" << endl;
    }

    return 0;
}