#ifndef _ASTRODEVLIB_H
#define _ASTRODEVLIB_H

#include <stdint.h>
#include <Arduino.h>
#include <queue>
#include <vector>
#include <map>
#include <support/packetcomm.h>

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            class Astrodev
            {
            public:
                static constexpr uint8_t SYNC0 = 'H';
                static constexpr uint8_t SYNC1 = 'e';
                static constexpr uint8_t COMMAND = 0x10;
                static constexpr uint8_t RESPONSE = 0x10;
                static constexpr uint8_t MTU = 254;
                static constexpr uint16_t PACKETCOMM_DATA_SIZE = MTU - (sizeof(Cosmos::Support::PacketComm::Header) + 2);

                std::map<uint8_t, uint16_t> RF_BAUD = {{0, 9600}, {1, 19200}, {2, 38400}, {3, 57600}, {4, 115200}};
                std::map<uint8_t, uint16_t> RF_INDEX = {{1200, 0}, {9600, 1}, {19200, 2}, {38400, 3}, {57600, 4}, {115200, 5}};
                std::map<uint8_t, uint16_t> UART_BAUD = {{0, 1200}, {1, 9600}, {2, 19200}, {3, 38400}, {4, 57600}, {5, 115200}};
                std::map<uint8_t, uint16_t> UART_INDEX = {{9600, 0}, {19200, 1}, {38400, 2}, {57600, 3}, {115200, 4}};

                Astrodev();

                enum class Command
                {
                    NAK = 0x00,
                    NOOP = 0x01,
                    RESET = 0x02,
                    TRANSMIT = 0x03,
                    RECEIVE = 0x04,
                    GETTCVCONFIG = 0x05,
                    SETTCVCONFIG = 0x06,
                    TELEMETRY = 0x07,
                    FLASH = 0x08,
                    RFCONFIG = 0x09,
                    BEACONDATA = 0x10,
                    BEACONCONFIG = 0x11,
                    FIRMWAREREV = 0x12,
                    DIOKEY = 0x13,
                    FIRMWAREUPDATE = 0x14,
                    FIRMWAREPACKET = 0x15,
                    WRITE_KEY_A_128 = 0x16,
                    WRITE_KEY_B_128 = 0x17,
                    WRITE_KEY_A_256 = 0x18,
                    WRITE_KEY_B_256 = 0x19,
                    FASTSETPA = 0x20,
                    GET_RTC = 0x41,
                    SET_RTC = 0x42,
                    ALARM_RTC = 0x42
                };

                enum class Modulation
                {
                    ASTRODEV_MODULATION_GFSK,
                    ASTRODEV_MODULATION_AFSK,
                    ASTRODEV_MODULATION_BPSK
                };

                enum class OACommand
                {
                    TELEMETRY_DUMP = 0x30,
                    PING_RETURN = 0x31,
                    CODE_UPLOAD = 0x32,
                    RADIO_RESET = 0x33,
                    PIN_TOGGLE = 0x34,
                };

                struct __attribute__((packed)) uart_status
                {
                    bool buffer_full : 1;
                    bool gpio_a_high : 1;
                    bool gpio_b_high : 1;
                    bool external_event_high : 1;
                };

                struct __attribute__((packed)) function_config1
                {
                    // Nybble 1
                    unsigned ext_event_pin12_functions : 2; // 0: Off, Logic Low / 1: 2.5s Toggle / 2: TX Packet Toggle (12ms) / 3: Rx Packet Toggle (1.25ms)
                    unsigned config2_pin13_functions : 2;   // 0: Off, Logic Low / 1: Tx/Rx Switch / 2: 2.5Hz WDT / 3: Rx Packet Toggle (1.25ms)

                    // Nybble 2
                    unsigned config1_pin14_dio_enable : 1;  // 0: CONFIG1 Off, Logic Low  /  1: CONFIG1 Digital IO Over the Air Key Enable
                    unsigned config1_pin14_dio_pattern : 1; // 0: CONFIG1 Pattern A, Latch High  /  1: CONFIG1 Pattern B, Toggle, 72ms High
                    unsigned rx_crc_enable : 1;             // RX CRC Enable 1/Disable 0
                    unsigned tx_crc_enable : 1;             // TBD // TX CRC Enable 1/Disable 0

                    // Nybble 3
                    unsigned tlm_packet_logging_enable : 1; // Telemetry Packet Logging Enable 1/Disable 0
                    unsigned tlm_packet_logging_rate : 2;   // Logging Rate 0 1/10 Hz, 1 1 Hz, 2 2 Hz,3 4 Hz
                    unsigned tlm_dump_enable : 1;           // Telemetry Dump Enable 1/Disable 0

                    // Nybble 4
                    unsigned oa_commands_enable : 1;        // Enable OA Commands Enable 1/Disable 0
                    unsigned code_upload_enable : 1;        // Code Upload Enable 1/Disable 0
                    unsigned radio_reset_enable : 1;        // Radio Reset Enable 1/Disable 0
                    unsigned factory_defaults_restored : 1; // Flag: Factory settings restore complete
                };

                struct __attribute__((packed)) function_config2
                {
                    // Nybble 1
                    unsigned rx_afc_enable : 1;   // Receiver Automatic Frequency Control On/Off
                    unsigned test_mode_rx_cw : 1; // RX CW (CAUTION TEST MODE), Set to 0 for normal operation
                    unsigned test_mode_tx_cw : 1; // TX CW (CAUTION TEST MODE), Set to 0 for normal operation
                    unsigned test_mode_tbd : 1;   // TBD (CAUTION TEST MODE), Set to 0 for normal operation

                    // Nybble 2-4
                    unsigned zeros : 4; // Set to 0
                };

                struct __attribute__((packed)) tcv_config
                {
                    uint8_t interface_baud_rate;
                    uint8_t power_amp_level;
                    uint8_t rx_baud_rate;
                    uint8_t tx_baud_rate;
                    uint8_t rx_modulation;
                    uint8_t tx_modulation;
                    uint16_t rx_freq_low;
                    uint16_t rx_freq_high;
                    uint16_t tx_freq_low;
                    uint16_t tx_freq_high;
                    uint8_t ax25_source[6];
                    uint8_t ax25_destination[6];
                    uint16_t ax25_preamble_length;
                    uint16_t ax25_postamble_length;
                    function_config1 config1;
                    function_config2 config2;
                    uint16_t cs;
                };

                struct __attribute__((packed)) rf_config
                {
                    uint8_t front_end_level;
                    uint8_t power_amp_level;
                    uint16_t tx_freq;
                    uint16_t rx_freq;
                    uint16_t cs;
                };

                struct __attribute__((packed)) telemetry
                {
                    uint16_t op_counter;
                    int16_t msp430_temp;
                    unsigned time_count : 24;
                    uint8_t rssi;
                    uint32_t bytes_rx;
                    uint32_t bytes_tx;
                    uint16_t cs;
                };

                struct __attribute__((packed)) firmware
                {
                    float rev;
                    uint16_t cs;
                };

                struct __attribute__((packed)) rtc
                {
                    uint16_t year;      // Year = 0x2021
                    uint8_t mon;        // Month = 0x10 = October
                    uint8_t day;        // Day = 0x07 = 7th
                    uint8_t dow;        // Day of week = 0x05 = Friday
                    uint8_t hour;       // Hour = 0x11
                    uint8_t min;        // Minute = 0x59
                    uint8_t sec;        // Seconds = 0x30
                    uint8_t alarm_dow;  // RTC Day of week alarm = 0x2
                    uint8_t alarm_day;  // RTC Day Alarm = 0x20
                    uint8_t alarm_hour; // RTC Hour Alarm
                    uint8_t alarm_min;  // RTC Minute Alarm
                };

                struct __attribute__((packed)) beacon_config
                {
                    uint8_t beacon_interval; // 0 is off, 2.5 sec delay per LSB
                    uint16_t cs;
                };

                struct __attribute__((packed)) frame
                {
                    union
                    {
                        struct
                        {
                            uint8_t sync0;
                            uint8_t sync1;
                            uint8_t type;
                            uint8_t command;
                            unsigned ack : 4;
                            uart_status status;
                            uint8_t size;
                            uint16_t cs;
                        } header;
                        uint8_t preamble[8];
                    };
                    union
                    {
                        uint8_t payload[MTU + 20];
                        tcv_config tcv;
                        rf_config rf;
                        firmware firmw;
                        telemetry telem;
                    };
                };

                telemetry last_telem;
                int32_t last_error = 0;
                bool last_ack = false;
                Command last_command = Command::NAK;

                int32_t Queue(std::queue<Cosmos::Support::PacketComm> &queue, Threads::Mutex &mtx, const Cosmos::Support::PacketComm &p);
                int32_t DeQueue(std::queue<Cosmos::Support::PacketComm> &queue, Threads::Mutex &mtx, Cosmos::Support::PacketComm &p);
                int32_t PacketIn(Cosmos::Support::PacketComm &p);
                int32_t PacketInSize();
                int32_t PacketOut(Cosmos::Support::PacketComm &p);
                int32_t PacketOutSize();
                int32_t Clear(std::queue<Cosmos::Support::PacketComm> &queue, Threads::Mutex &mtx);
                int32_t Init(HardwareSerial *serial = &Serial8, uint32_t baud_rate = 9600);
                void Join();
                int32_t Packetize(Cosmos::Support::PacketComm &packet);
                int32_t UnPacketize(Cosmos::Support::PacketComm &packet);

                int32_t Ping();
                int32_t Reset();
                int32_t SendData(std::vector<uint8_t> data);
                int32_t GetTCVConfig();
                int32_t SetTCVConfig(tcv_config config);
                int32_t GetTelemetry();
                int32_t SetRFConfig(rf_config config);
                int32_t Transmit(frame message);
                uint16_t CalcCS(uint8_t *data, uint16_t size);

                int32_t connect(String dev);
                int32_t disconnect();
                int32_t recvframe();
                int32_t setupframe(frame *frame);
                uint16_t loadframe(uint8_t *data, uint16_t size);
                int32_t unloadframe(uint8_t *data, uint16_t size);
                int32_t checkframe(frame *frame);
                int32_t rfconfig();
                int32_t firmwarerev();
                int32_t transmit(uint8_t *data, uint16_t size);
                int32_t receive(uint8_t *data, uint16_t size);

            private:
                bool running = true;
                HardwareSerial *serial;

                std::queue<std::vector<uint8_t>> queue_in;
                Threads::Mutex qmutex_in;
                int32_t push_queue_in(std::vector<uint8_t> data);
                int32_t pop_queue_in(std::vector<uint8_t> &data);
                int32_t check_queue_in();

                std::queue<Cosmos::Support::PacketComm> packet_queue_out;
                void receive_loop();
                Threads rthread;
                Threads::Mutex qmutex_out;
            };
        }
    }
}

#endif // _ASTRODEVLIB_H
