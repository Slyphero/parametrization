# Paramétrisation

* Projet réalisé en binôme dans le cadre de nos enseignements d'option de synthèse d'image (M1 Informatique)
* Le projet nécessite plus de dépendances pour fonctionner
* Le cœur du projet était de comprendre l'algorithme de la paramétrisation de Tutte et l'algorime LSCM.
* Vous trouverez le corps du code dans le dossier `src/` ainsi que les consignes dans `consignes.md`

## Instructions d'installation
* Il faut d'abord cloner `https://forge.univ-lyon1.fr/JEAN-CLAUDE.IEHL/gkit3`
* Se rendre dans le dossier puis cloner `https://forge.univ-lyon1.fr/JEAN-CLAUDE.IEHL/gkit3GL.git` 
* Modifier le fichier `gkit3GL/premake5.lua` de la manière suivante :
```
dofile "GL.lua"

project("parametrization")
  language "C++"
  kind "ConsoleApp"
  targetdir "bin"
  buildoptions ( "-std=c++20" )
  links { "gkit3GL" }
  includedirs { ".", "../src", "src", "projets/parametrization/third_party" }
  files { "projets/parametrization/src/*.cpp", "projets/parametrization/src/*.h" }
```
* Pour build le projet il suffit de se rendre dans le répertoire `gkit3/gkit3GL` puis `premake5 gmake` pour utiliser make puis faire `make parametrization` puis `./bin/parametrization`
* Les commandes clavier sont indiquées dans la console.
