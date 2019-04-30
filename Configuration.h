#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#define SLEEP_TIME SLEEP_15MS
//#define SLEEP_TIME   SLEEP_30MS
//#define SLEEP_TIME   SLEEP_60MS
//#define SLEEP_TIME   SLEEP_120MS
//#define SLEEP_TIME   SLEEP_250MS
//#define SLEEP_TIME   SLEEP_500MS
#define SLEEP_TIME   SLEEP_1S
//#define SLEEP_TIME   SLEEP_2S
//#define SLEEP_TIME   SLEEP_4S
//#define SLEEP_TIME   SLEEP_8S

// XBee Definitions
#define BAUD      9600
// Destination Address - Coordinator Address
#define XB_ADR_MSB 0x0
#define XB_ADR_LSB 0x0
//#define SLEEP_PIN 7

// SHT15 Pins Definitions
//#define PWR_SHT   2
//#define GND_SHT   3
#define DAT_SHT   6
#define SCK_SHT   5



#endif //CONFIGURATION_H
