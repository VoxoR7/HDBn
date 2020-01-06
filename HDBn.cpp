#include "HDBn.h"

#include <string.h>
#include <stdio.h>

#define DEFAULT_HDBN 3
#define MAX_HDBN 4

#define MAX_CAR 63

#define MARGE 0.002

HDBn::HDBn( const char *messagep, uint8_t hdbn ) {
    /* cette fonction prepare le message a etre codé */

    uint16_t i = 0, j = 0;
    uint16_t taille = strlen(messagep), p = 1;

    if ( hdbn < 1 || hdbn > MAX_HDBN ) {

        HDBN = DEFAULT_HDBN;
    } else
        HDBN = hdbn;

    if ( HDBN == 1 )
        printf("Codage arithmetique utilisé\n");
    else
        printf("Codage HDB%d utilsé\n", HDBN);
    
    if ( HDBN != 1 && taille < HDBN + 1 ) {

        printf("\033[31merreur : \033[00La taille du message est insuffisante pour etre envoyé ( minimum : %d )\n", HDBN + 1);
        return;
    } else if ( HDBN != 1 )

    for ( i = 0, j = taille ; i < j ; i++ ) {

        if ( messagep[i] != '0' && messagep[i] != '1' ) {

            printf("\033[33mwarning : \033[00mcaractere non reconnu [%c]\n", messagep[i] );
            taille--;
        }
    }

    if ( taille == 0 ) {

        printf("\033[31merreur : \033[00maucun caracetere valide\n");
        return;
    }

    if ( HDBN > 1 ) {

        i = 0, j = 0;

        p = p << 15;

        message = new int8_t[taille + 16]; /* les 16 premiers bit correspondent a la taille du message */

        while ( p != 0 ) {

            if ( taille >= p ) {

                message[i] = 1;
                taille -= p;
            } else
                message[i] = 0;

            i++;
            p = p >> 1;
        }

        taille = strlen(messagep);

        while ( messagep[j] != '\0' ) {

            switch ( messagep[j] ) {
                case '0':
                    *( message + i ) = 0;
                    break;
                case '1':
                    *( message + i ) = 1;
                    break;
                default:
                    i--;
                    taille--;
            }

            i++;
            j++;
        }

    } else {

        messageAri = new char[strlen(messagep)];
        strcpy( messageAri, messagep); /* permet de sauvegarder le message au lieu de le repasser par parametre a la fonction d'encodage */
        
        probBI = new double[MAX_CAR];
        probBS = new double[MAX_CAR];
        car = new char[MAX_CAR];

        uint16_t *apparition = new uint16_t[MAX_CAR];

        uint16_t i = 0, newCar = 0, j;

        while ( messagep[i] != '\0' ) {

            j = 0;

            while ( j < newCar && car[j] != messagep[i] )
                j++;

            if ( j == newCar ) {

                newCar++;
                car[j] = messagep[i];
                apparition[j] = 1;
            } else
                apparition[j]++;

            i++;
        }

        j = 1;

        probBI[0] = 0;
        probBS[0] = (float)apparition[0] / (float)i;

        while ( j < newCar ) {

            probBI[j] = probBS[j - 1];
            probBS[j] = probBI[j] + (float)apparition[j] / (float)i;
            j++;
        }
    }
}

void HDBn::encodage () {

    if ( HDBN > 1 )
        codage_HDBN();
    else
        codage_Ari();
}

void HDBn::decodage() {

    if ( HDBN > 1 )
        decodage_HDBN();
    else
        decodage_Ari();
}

void HDBn::afficher() {

    if ( HDBN > 1 )
        affichage_HDBN();
    else
        affichage_Ari();
}

void HDBn::afficher( uint16_t forceTaille ) {

    if ( HDBN > 1 )
        affichage_HDBN( forceTaille );
    else
        affichage_Ari();
}

void HDBn::codage_HDBN() {

    int8_t dernier1 = -1;
    int8_t dernierViol = -1;
    uint16_t i = 0, j, k;

    uint16_t it = 0, jt = 1, taille = 16;
    jt = jt << 15;

    while ( it < 16 ) {

        if ( message[it] == 1 )
            taille += jt;
        
        jt = jt >> 1;
        it++;
    }

    while ( taille --> 0 ) { /* tant que on n'a pas finit le message, on code */

        if ( message[i] == 0 ) {

            j = i;
            k = 0;

            while ( ++j <= i + HDBN && message[j] == 0 );

            if ( j > i + HDBN ) { /* il y a plus de HDBN symbole 0 a la suite */

                j = 0;

                if ( dernierViol == 1 ) {
                    if ( dernier1 == 1 ) {

                        message[i] = -1;
                        while ( j++ < HDBN - 1)
                            message[++i] = 0;
                        message[++i] = -1;
                    } else {

                        while ( j++ < HDBN - 1)
                            message[++i] = 0;
                        message[++i] = -1;
                    }

                    dernierViol = -1;
                    dernier1 = -1;
                } else {
                    if ( dernier1 == 1 ) {

                        while ( j++ < HDBN - 1)
                            message[++i] = 0;
                        message[++i] = 1;
                    } else {

                        message[i] = 1;
                        while ( j++ < HDBN - 1)
                            message[++i] = 0;
                        message[++i] = 1;
                    }

                    dernierViol = 1;
                    dernier1 = 1;
                }
            } else {

                while ( message[++i] == 0 );
                i--;
            }

        } else if ( dernier1 == 1 ) {
            
            dernier1 = -1;
            message[i] = -1;
        } else {

            dernier1 = 1;
            message[i] = 1;
        }

        i++;
    }
}

void HDBn::decodage_HDBN() {

    uint16_t tailleDecodage = 16;

    int8_t dernier1 = -1;
    int8_t dernierViol = -1;
    uint16_t i = 0, j, k;

    int8_t *sequence = new int8_t[HDBN + 1];

    /* decodage des 16 bits de longueur du message */

    while ( i < tailleDecodage ) {

        j = i, k = 0;

        if ( message[j] == message[j + HDBN] ) { /* test pour savoir si il y a une plusieurs 0 a la suite ( si oui alors k = 1 ) */

            k = 1;

            while ( ++j < i + HDBN )
                if ( message[j] != 0 ) {

                    k = 0;
                    j = i + HDBN;
                }
        } else {

            k = 1;

            while ( j++ < i + HDBN )
                if ( message[j-1] != 0 ) {

                    k = 0;
                    j = i + HDBN + 1;
                }

            if ( message[j] != dernier1 )
                k = 0; 
        }

        if ( !k && message[i] != 0 ) { /* si il n'y a pas d'erreur ( pas plus de hdbn 0 a la suite, alors on traite le cas normale */

            message[i] = 1;
            dernier1 = -dernier1;
            i++;
        } else if ( k ) { /* sinon on remet les 0 */
            j = i;

            if ( message[i + HDBN] == dernier1 )
                dernier1 = -dernier1;

            while ( j++ <= i + HDBN )
                message[j-1] = 0;

            i = i + HDBN + 1;

        } else
            i++;
    }

    /* fin de decodage des 16 premiers bits */

    uint16_t ti = 0, tj = 1, taille = 0;
    tj = tj << 15;

    while ( ti < 16 ) {

        if ( message[ti] == 1 )
            taille += tj;
        
        tj = tj >> 1;
        ti++;
    }

    /* decodage du message */

    while ( i < tailleDecodage + taille ) {

        j = i, k = 0;

        if ( message[j] == message[j + HDBN] ) {

            k = 1;

            while ( ++j < i + HDBN )
                if ( message[j] != 0 ) {

                    k = 0;
                    j = i + HDBN;
                }
        } else {

            k = 1;

            while ( j++ < i + HDBN )
                if ( message[j-1] != 0 ) {

                    k = 0;
                    j = i + HDBN + 1;
                }

            if ( message[j] != dernier1 )
                k = 0; 
        }

        if ( !k && message[i] != 0 ) {

            message[i] = 1;
            dernier1 = -dernier1;
            i++;
        } else if ( k ) {
            j = i;
            
            if ( sequence[i + HDBN] == dernier1 )
                dernier1 = -dernier1;

            while ( j++ <= i + HDBN )
                message[j-1] = 0;

            i = i + HDBN + 1;

        } else
            i++;
    }
}

void HDBn::affichage_HDBN() {

    uint16_t i = 0, j = 1, taille = 0;
    j = j << 15;

    printf("<en tete du message (taille)>\t");

    while ( i < 16 ) {

        if ( message[i] == 1 )
            taille += j;

        printf("%" PRId8" ", message[i]);
        
        j = j >> 1;
        i++;
    }

    printf("\ttaille du message : %" PRIu16"\n", taille);

    printf("<message transmis>\t\t");

    while ( taille --> 0 ) {

        printf("%" PRId8" ", message[i]);

        i++;
    }

    printf("\n");
}

void HDBn::affichage_HDBN( uint16_t forceTaille ) {

    uint16_t i = 0;
    forceTaille += 16;

    while ( forceTaille --> 0 ) {

        printf("%" PRId8" ", message[i]);

        i++;

        if ( i == 16 ) {

            printf("\t\t");
        }
    }

    printf("\n");
}

void HDBn::codage_Ari() {

    uint16_t t = strlen( messageAri);
    uint8_t j;

    double BI = 0, BS = 1, diff;

    for ( uint16_t i = 0 ; i < t ; i++ ) {

        j = 0;

        while ( car[j] != messageAri[i] )
            j++;

        diff = BS - BI;

        BS = BI + diff * probBS[j];
        BI = BI + diff * probBI[j];
    }

    res = ( BS - BI ) / 2 + BI;

    printf("le message %s encodé est : %f\n", messageAri, res);
    delete[] messageAri;
}

void HDBn::decodage_Ari() {

    uint8_t j;
    uint16_t i = 0;
    messageAri = new char[256];

    while ( res <= 0.5 - MARGE || res >= 0.5 + MARGE ) {

        j = 0;

        while ( ! ( probBI[j] <= res && probBS[j] >= res ) )
            j++;

        messageAri[i] = car[j];

        res = ( res - probBI[j] ) / ( probBS[j] - probBI[j] );

        i++;
    }

    messageAri[i] = '\0';

    printf("Le message a été decoder : %s\n", messageAri);
}

void HDBn::affichage_Ari() {

    uint16_t j = 0;

    while ( probBS[j] <= 1 ) {

        printf("car : %c | BI : %2.5f | BS : %2.5f\n", car[j], probBI[j], probBS[j]);
        j++;
    }

    printf("car : %c | BI : %2.5f | BS : %2.5f\n", car[j], probBI[j], probBS[j]);
}