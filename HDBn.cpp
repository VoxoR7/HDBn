#include "HDBn.h"

#include <string.h>
#include <stdio.h>

#define HDBN 2

HDBn::HDBn( const char *messagep, uint8_t HDBn ) {

    uint16_t i = 0, j = 0;
    uint16_t taille = strlen(messagep), p = 1;

    if ( taille < HDBN + 1 ) {

        printf("\033[31merror : \033[00La taille du message est insuffisante pour etre envoyé ( minimum : %d )\n", HDBn + 1);
        return;
    }

    for ( i = 0, j = taille ; i < j ; i++ ) {

        if ( messagep[i] != '0' && messagep[i] != '1' ) {

            printf("\033[33mwarning : \033[00mcaractere non reconnu [%c]\n", messagep[i] );
            taille--;
        }
    }

    if ( taille == 0 ) {

        printf("\033[31merror : \033[00maucun caracetere valide\n");
        return;
    }

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

void HDBn::decodage() {

    decodage( 16);
}

void HDBn::decodage( uint16_t tailleDecodage ) {

    int8_t dernier1 = -1;
    int8_t dernierViol = -1;
    uint16_t i = 0, j, k;

    int8_t *sequence = new int8_t[HDBN + 1];

    while ( tailleDecodage --> 0 ) {

        if ( message[i] == -dernier1 ) {

            message[i] = 1;
        } else {
            j = i;
            
            while ( j++ < i + HDBN )
                sequence[j-i-1] = message[j-1];

            j = 0;

            printf("seq : %d %d %d", sequence[0], sequence[1], sequence[2]);

            if ( dernierViol == 1 ) {
                if ( dernier1 == 1 ) {

                    if ( sequence[0] == -1 && sequence[HDBN] == -1 ) {
                        while ( sequence[++j] == 0 );

                        if ( j == HDBN ) {

                            message[i] = 0;
                            message[HDBN] = 0;
                        }
                    }

                    i = i + HDBN + 1;
                } else {

                    if ( sequence[0] == 0 && sequence[HDBN] == -1 ) {
                        while ( sequence[++j] == 0 );

                        if ( j == HDBN ) {

                            message[i] = 0;
                            message[HDBN] = 0;
                        }
                    }

                    i = i + HDBN + 1;
                }
            } else {
                if ( dernier1 == 1 ) {

                    if ( sequence[0] == 0 && sequence[HDBN] == 1 ) {
                        while ( sequence[++j] == 0 );

                        if ( j == HDBN ) {

                            message[i] = 0;
                            message[HDBN] = 0;
                        }
                    }

                    i = i + HDBN + 1;
                } else {
                    
                    if ( sequence[0] == 1 && sequence[HDBN] == 1 ) {
                        while ( sequence[++j] == 0 );

                        if ( j == HDBN ) {

                            message[i] = 0;
                            message[HDBN] = 0;
                        }
                    }

                    i = i + HDBN + 1;
                }
            }
        }
    }
}

void HDBn::afficher() {

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

void HDBn::afficher( uint16_t forceTaille ) {

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