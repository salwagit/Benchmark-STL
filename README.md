# Benchmark-STL

Ce projet a pour objectif de comparer les performances de différentes implémentations de conteneurs / structures de données en C++, notamment :

- un arbre binaire de recherche simple (BST)  
- un arbre équilibré (Red-Black Tree)  
- les conteneurs standards `std::set`, `std::map`, `std::unordered_set`, etc.

## Structure du dépôt

- `BST/` — code source de l’implémentation d’un Binary Search Tree simple  
- `Red-Black-Tree/` — code source d’une implémentation d’arbre rouge-noir (équilibré)  
- `set/`, `map/`, `unordered-set/` — dossiers contenant les benchmarks utilisant les conteneurs STL correspondants  

## Objectif

- Mesurer le temps d’insertion, de recherche, et éventuellement de parcours pour différentes structures de données.  
- Comparer les performances entre : un BST naïf, un arbre équilibré, et les conteneurs standard (STL).  
- Illustrer l’impact de l’équilibrage (ou de son absence) sur les performances, en particulier en cas d’insertion de données « ordonnées » (ce qui peut dégrader un BST naïf).  
