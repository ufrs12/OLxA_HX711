#include <ModbusRTUSlave.h>                         //  
#define MODBUS_SERIAL Serial                        //  ███╗░░░███╗░█████╗░██████╗░██████╗░██╗░░░██╗░██████╗
#define MODBUS_BAUD 19200                           //  ████╗░████║██╔══██╗██╔══██╗██╔══██╗██║░░░██║██╔════╝
#define MODBUS_CONFIG SERIAL_8N1                    //  ██╔████╔██║██║░░██║██║░░██║██████╦╝██║░░░██║╚█████╗░
#define MODBUS_UNIT_ID 1                            //  ██║╚██╔╝██║██║░░██║██║░░██║██╔══██╗██║░░░██║░╚═══██╗
const int16_t dePin = 5;                            //  ██║░╚═╝░██║╚█████╔╝██████╔╝██████╦╝╚██████╔╝██████╔╝
ModbusRTUSlave modbus(MODBUS_SERIAL, dePin);        //  ╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░░╚═════╝░╚═════╝░
const uint8_t numHoldingRegisters = 14;             //
uint16_t holdingRegisters[numHoldingRegisters];     //

#include <GyverHX711.h>
GyverHX711 sensor(3, 2, HX_GAIN32_B);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64


void setup() {
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);  //  █▀▄▀█ █▀▀█ █▀▀▄ █▀▀▄ █░░█ █▀▀ 
  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);                          //  █░▀░█ █░░█ █░░█ █▀▀▄ █░░█ ▀▀█ 
  modbus.begin(MODBUS_UNIT_ID, MODBUS_BAUD, MODBUS_CONFIG);                 //  ▀░░░▀ ▀▀▀▀ ▀▀▀░ ▀▀▀░ ░▀▀▀ ▀▀▀
  holdingRegisters[10] = 300;                                                //

}

  int16_t razn;

void loop() {
  // чтение только по доступности! if available
  if (sensor.available()) {
    holdingRegisters[10] = sensor.read() / 10;
  }

  modbus.poll();
  //analogWrite(3, holdingRegisters[1]);      // отправляем на мосфет
  delay(4);  
}
