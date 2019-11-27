#include "HDBn.h"

#include <string.h>
#include <stdio.h>

HDBn::HDBn( const char *messagep, uint8_t HDBn ) {

    if ( HDBn < 2 ) {

        printf("Impossible d'initialier n avec une valeur inferieure a 2, la valeur mise dans n est de 2\n");
        HDBn = 2;
    }

    n = HDBn;

    uint16_t i = 0, j = 0;
    uint16_t taille = strlen(messagep), p = 1;

    while ( messagep[j] != '\0' ) {

        switch ( messagep[j] ) {
            case '0':
                *( message + i ) = 0;
                break;
            case '1':
                *( message + i ) = 1;
                break;
            default:
                printf("\033[33mwarning : \033[00mcaractere non reconnu [%c]\n", messagep[j] );
                i--;
                taille--;
        }

        i++;
        j++;
    }

    if ( taille == 0 ) {

        printf("\033[31merror : \033[00maucun caracetere valide\n");
        return;
    }

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

    printf("\n");
}

void HDBn::encodage () {

    int8_t dernier1 = -1;
    int8_t dernierViol = -1;
    uint16_t i = 0, j, k;

    uint16_t it = 0, jt = 1, taille = 16;
    jt = jt << 15;

    while ( it < 16 ) {

        if ( message[it] == 1 )
            taille += jt;

        printf("%" PRId8" ", message[it]);
        
        jt = jt >> 1;
        it++;
    }

    while ( taille --> 0 ) {

        if ( message[i] == 0 ) {

            j = i;
            k = 0;

            while ( ++j <= i + n && message[j] == 0 );

            if ( j > i + n ) { /* il y a plus de n symbole 0 a la suite */

                j = 0;

                if ( dernierViol == 1 ) {
                    if ( dernier1 == 1 ) {

                        message[i] = -1;
                        while ( j++ < n - 1)
                            message[++i] = 0;
                        message[++i] = -1;
                    } else {

                        while ( j++ < n)
                            message[++i] = 0;
                        message[++i] = -1;
                    }

                    dernierViol = -1;
                    dernier1 = -1;
                } else {
                    if ( dernier1 == 1 ) {

                        while ( j++ < n - 1)
                            message[++i] = 0;
                        message[++i] = 1;
                    } else {

                        message[i] = 1;
                        while ( j++ < n - 1)
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

void HDBn::decodage() {

    int8_t dernier1 = -1;
    int8_t dernierViol = -1;
    uint16_t i = 0, j, k;

    while ( message[i] != 2 ) {

        if ( message[i] == 0 ) {

            j = i;
            k = 0;

            while ( ++j <= i + n && message[j] == 0 );

            if ( j > i + n ) {
                j = 0;

                if ( dernierViol == 1 ) {
                    if ( dernier1 == 1 ) {

                        message[i] = -1;
                        while ( j++ < n - 1)
                            message[++i] = 0;
                        message[++i] = -1;
                    } else {

                        while ( j++ < n)
                            message[++i] = 0;
                        message[++i] = -1;
                    }

                    dernierViol = -1;
                    dernier1 = -1;
                } else {
                    if ( dernier1 == 1 ) {

                        while ( j++ < n - 1)
                            message[++i] = 0;
                        message[++i] = 1;
                    } else {

                        message[i] = 1;
                        while ( j++ < n - 1)
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

void HDBn::afficher() {

    uint16_t i = 0, j = 1, taille = 0;
    j = j << 15;

    while ( i < 16 ) {

        if ( message[i] == 1 )
            taille += j;

        printf("%" PRId8" ", message[i]);
        
        j = j >> 1;
        i++;
    }

    printf("taille du message : %" PRIu16"\n", taille);

    while ( taille --> 0 ) {

        printf("%" PRId8" ", message[i]);

        i++;
    }

    printf("\n");
}