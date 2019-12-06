#include "HDBn.h"

#include <stdio.h>

int main () {

    printf("--- Creation ---\n");

    HDBn hdbn = HDBn("10010100000011000011100", 2);
    /*HDBn hdbn = HDBn("101", 2);*/

    printf("--- Affichage ---\n");

    hdbn.afficher();

    printf("--- Encodage ---\n");

    hdbn.encodage();

    printf("--- Affichage ---\n");

    hdbn.afficher( 23);

    printf("--- Decodage ---\n");

    printf("--- Affichage ---\n");

    hdbn.afficher( 23);
}