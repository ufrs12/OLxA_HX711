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
GyverHX711 sensor(3, 2, HX_GAIN128_A);
// HX_GAIN128_A - канал А усиление 128
// HX_GAIN32_B - канал B усиление 32
// HX_GAIN64_A - канал А усиление 64
// Переменные для усреднения
const int NUM_READINGS = 24;
long readings[NUM_READINGS];
int readIndex = 0;
long total = 0;
long average = 0;
long lastRaw = 0;


void setup() {
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);  //  █▀▄▀█ █▀▀█ █▀▀▄ █▀▀▄ █░░█ █▀▀ 
  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);                          //  █░▀░█ █░░█ █░░█ █▀▀▄ █░░█ ▀▀█ 
  modbus.begin(MODBUS_UNIT_ID, MODBUS_BAUD, MODBUS_CONFIG);                 //  ▀░░░▀ ▀▀▀▀ ▀▀▀░ ▀▀▀░ ░▀▀▀ ▀▀▀
  holdingRegisters[10] = 300;                                               //

  // Инициализация массива для усреднения нулями
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // Обработка Modbus запросов
  modbus.poll();
  
  // Обработка измерений с HX711
  processScale();
  
  // Обновление регистров Modbus
  updateModbusRegisters();

  delay(4);  
}

void processScale() {
  // чтение только по доступности! if available
  if (sensor.available()) {
    lastRaw = sensor.read();
    
    // Обновление массива усреднения
    total = total - readings[readIndex];
    readings[readIndex] = lastRaw;
    total = total + readings[readIndex];
    
    readIndex = (readIndex + 1) % NUM_READINGS;
    average = total / NUM_READINGS;
  }
}

void updateModbusRegisters() {
  // Разбиваем 32-битные значения на два 16-битных регистра
  holdingRegisters[0] = (average >> 16) & 0xFFFF;  // RAW_AVERAGE_HI
  holdingRegisters[1] = average & 0xFFFF;          // RAW_AVERAGE_LO
  holdingRegisters[2] = (lastRaw >> 16) & 0xFFFF;  // LAST_RAW_HI
  holdingRegisters[3] = lastRaw & 0xFFFF;          // LAST_RAW_LO
}


