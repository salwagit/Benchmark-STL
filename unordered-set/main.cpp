#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <unordered_set>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

// =============================
// ========= PROGRAMME =========
// =============================
int main() {
    const int nombreElements = 10000000;
    const string nomFichier = "donnees.txt";

    unordered_set<string> tableHachage;  // Table de hachage (unordered_set)

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

    cout << "\nInsertion dans la table de hachage...\n";
    auto debutInsertion = high_resolution_clock::now();

    string mot;
    int compteur = 0;
    while (fichier >> mot && compteur < nombreElements) {
        tableHachage.insert(mot);  // Insertion dans la table de hachage
        compteur++;
    }
    fichier.close();

    auto finInsertion = high_resolution_clock::now();
    auto dureeInsertion = duration_cast<milliseconds>(finInsertion - debutInsertion);
    cout << "Insertion terminee. (" << compteur << " mots lus)\n";
    cout << "Elements inseres: " << tableHachage.size() << " (sans doublons)\n";
    cout << "Temps: " << dureeInsertion.count() << " ms\n";

    // === Statistiques de la table de hachage ===
    cout << "\n=== Statistiques de la table de hachage ===" << endl;
    cout << "Facteur de charge: " << tableHachage.load_factor() << endl;
    cout << "Facteur de charge maximal: " << tableHachage.max_load_factor() << endl;
    cout << "Nombre de buckets: " << tableHachage.bucket_count() << endl;
    
    // Afficher quelques statistiques sur les buckets
    size_t bucket_vide = 0;
    size_t bucket_max = 0;
    for (size_t i = 0; i < tableHachage.bucket_count(); ++i) {
        size_t taille = tableHachage.bucket_size(i);
        if (taille == 0) bucket_vide++;
        if (taille > bucket_max) bucket_max = taille;
    }
    cout << "Buckets vides: " << bucket_vide << endl;
    cout << "Taille maximale d'un bucket: " << bucket_max << endl;

    // === Recherche ===
    string motRecherche = "mot_" + to_string(nombreElements / 2);
    cout << "\nRecherche de \"" << motRecherche << "\"...\n";

    auto debutRecherche = high_resolution_clock::now();
    bool trouve = (tableHachage.find(motRecherche) != tableHachage.end());
    auto finRecherche = high_resolution_clock::now();
    auto dureeRecherche = duration_cast<microseconds>(finRecherche - debutRecherche);

    cout << (trouve ? "Mot trouve." : "Mot non trouve.") << "\n";
    cout << "Temps: " << dureeRecherche.count() << " us\n";
    cout << "Bucket contenant l'element: " << tableHachage.bucket(motRecherche) << endl;

    // === Tests de recherche supplémentaires ===
    cout << "\n=== Tests de recherche supplementaires ===" << endl;
    
    // Recherche d'éléments qui devraient exister
    vector<string> motsATester = {
        "mot_0",
        "mot_500000",
        "mot_999999",
        "mot_2500000",
        "mot_7500000"
    };
    
    for (const auto& motTest : motsATester) {
        auto debut = high_resolution_clock::now();
        bool existe = (tableHachage.find(motTest) != tableHachage.end());
        auto fin = high_resolution_clock::now();
        cout << "Recherche \"" << motTest << "\": " 
             << (existe ? "trouve" : "non trouve") 
             << " en " << duration_cast<nanoseconds>(fin - debut).count() << " ns" 
             << " (bucket: " << tableHachage.bucket(motTest) << ")" << endl;
    }

    // Recherche d'un élément qui ne devrait pas exister
    string motInexistant = "mot_invente_12345";
    auto debutInexistant = high_resolution_clock::now();
    bool existeInexistant = (tableHachage.find(motInexistant) != tableHachage.end());
    auto finInexistant = high_resolution_clock::now();
    cout << "Recherche \"" << motInexistant << "\": " 
         << (existeInexistant ? "trouve" : "non trouve") 
         << " en " << duration_cast<nanoseconds>(finInexistant - debutInexistant).count() << " ns" << endl;

    // === Parcours (non ordonné !) ===
    cout << "\nParcours de la table de hachage (non ordonne)...\n";
    auto debutParcours = high_resolution_clock::now();
    
    long long sommeHash = 0;
    for (const auto& element : tableHachage) {
        // Les éléments ne sont pas parcourus dans l'ordre !
        // cout << element << " "; // Désactivé pour performance
        sommeHash += hash<string>{}(element);
    }
    
    auto finParcours = high_resolution_clock::now();
    auto dureeParcours = duration_cast<milliseconds>(finParcours - debutParcours);

    cout << "Temps de parcours: " << dureeParcours.count() << " ms\n";
    cout << "Somme des hash (pour verification): " << (sommeHash % 1000000) << endl;

    // === Test des performances avec rehash ===
    cout << "\n=== Test avec rehashing ===" << endl;
    tableHachage.rehash(tableHachage.size() * 2);  // Augmenter la taille des buckets
    cout << "Nouveau nombre de buckets apres rehash: " << tableHachage.bucket_count() << endl;
    cout << "Nouveau facteur de charge: " << tableHachage.load_factor() << endl;

    // Recherche après rehash
    auto debutApresRehash = high_resolution_clock::now();
    tableHachage.find(motRecherche);
    auto finApresRehash = high_resolution_clock::now();
    cout << "Recherche apres rehash: " 
         << duration_cast<nanoseconds>(finApresRehash - debutApresRehash).count() 
         << " ns" << endl;

    // === Comparaison des complexites ===
    cout << "\n=== Complexites theoriques ===" << endl;
    cout << "unordered_set (table de hachage):" << endl;
    cout << "  - Insertion moyenne: O(1)" << endl;
    cout << "  - Recherche moyenne: O(1)" << endl;
    cout << "  - Pire cas: O(n)" << endl;
    cout << "  - Pas d'ordre de tri" << endl;
    cout << "\nset (arbre rouge-noir):" << endl;
    cout << "  - Insertion: O(log n)" << endl;
    cout << "  - Recherche: O(log n)" << endl;
    cout << "  - Elements tries" << endl;

    return 0;
}