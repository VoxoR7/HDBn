#include "HDBn.h"

#include <stdio.h>

int main () {

    printf("--- Creation ---\n");

    /*HDBn hdbn = HDBn("10010100000011000011100", 2);*/
    HDBn hdbn = HDBn("1010010101", 2);

    printf("--- Affichage ---\n");

    hdbn.afficher();

    /*printf("--- Encodage ---\n");

    hdbn.encodage();

    printf("--- Affichage ---\n");

    hdbn.afficher();*/
}