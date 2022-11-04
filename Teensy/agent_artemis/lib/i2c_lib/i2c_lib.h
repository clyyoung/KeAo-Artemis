#include <i2c_driver.h>
#include <i2c_driver_wire.h>
#include <String.h>

namespace Artemis
{
    namespace Teensy
    {
        class Artemis_I2C
        {
            private:
            uint8_t SLAVE_ADDRESS;
            I2CDriverWire *wire;

            public:
            static void init(I2CDriverWire *wire, uint8_t address);
            static void receiveData(int bytecount);
            static void sendData();
            
        };
    }
}