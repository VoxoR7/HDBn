#include <inttypes.h>

class HDBn {

    private:
        int8_t *message;
        uint8_t n;

    public:
        HDBn( const char *messagep, uint8_t HDBn );
        void encodage();
        void decodage();
        void afficher();
};