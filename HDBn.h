#include <inttypes.h>

class HDBn {

    private:
        int8_t *message;

        void decodage( uint16_t tailleDecodage );

    public:
        HDBn( const char *messagep, uint8_t HDBn );
        void encodage();
        void decodage();
        void afficher();
        void afficher( uint16_t forceTaille );
};