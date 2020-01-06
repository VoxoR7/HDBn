#include <inttypes.h>

class HDBn {

    private:
        /* utilisation commune */
        uint8_t HDBN;

        /* utilisé uniquement par HDBn*/
        int8_t *message;

        /* utilisé uniquement par le codage arithmetique*/
        double *probBI, *probBS, res;
        char *car;
        char *messageAri; /* permet de sauvegarder le message au lieu de le repasser par parametre a la fonction d'encodage */

        void codage_HDBN();
        void decodage_HDBN();
        void affichage_HDBN();
        void affichage_HDBN( uint16_t forceTaille );

        void codage_Ari();
        void decodage_Ari();
        void affichage_Ari();

    public:
        HDBn( const char *messagep, uint8_t hdbn );
        void encodage();
        void decodage();
        void afficher();
        void afficher( uint16_t forceTaille );
};