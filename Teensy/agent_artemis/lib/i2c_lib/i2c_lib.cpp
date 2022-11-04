#include <i2c_lib.h>

byte data_to_echo = 0;
std::string str = "I am Teensy\n";
uint8_t response_index = 0;
String data;

namespace Artemis{
    namespace Teensy{
        Artemis_I2C i2c;
        void Artemis_I2C::init(I2CDriverWire *wire1, uint8_t address){
            i2c.SLAVE_ADDRESS = address;
            i2c.wire = wire1;
            i2c.wire->begin(i2c.SLAVE_ADDRESS);
            i2c.wire->onReceive(receiveData);
            i2c.wire->onRequest(sendData);
        }
        void Artemis_I2C::receiveData(int bytecount){
            for (int i = 0; i < bytecount; i++){
                byte data_to_echo = i2c.wire->read();
                Serial.print((char)data_to_echo);
            }
        }
        void Artemis_I2C::sendData(){
            i2c.wire->write(str[response_index]);
            response_index++;
            if(response_index == str.size())
                response_index = 0;
        }
    }
}