# Library
In dit folder vind je de OBSBOT SDK. Deze heb je nodig om de CameraController code te runnen.
In *libdev* vind je een folder *include* waarin nog 2 folders staan. *include* kan je 
in een keer kopieeren naar je CameraController folder. Ik heb het zelf in een
folder *sdk* gezet. Daarnaast moet je de library files nog kopieeren. Ik heb in *sdk* 
nog een folder *lib* aangemaakt. Daarin staat voor Linux een .so.1 bestand, en .so.1.0.3
Op windows is dit **libdev.dll** en **libdev.lib**.

De folder structuur word dus als volgt:
ProjectFolder:

    ├── include/
    │   ├── ...
    │   └── ...
    └── lib/
        ├── Linux/
        │   ├── libdev.so.1
        │   └── libdev.so.1.0.3
        └── Windows/
            ├── libdev.dll
            └── libdev.lib

Gebruik in *lib* alleen de map structuur die voor jouw machine van toepassing is.
