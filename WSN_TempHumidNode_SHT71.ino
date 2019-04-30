/*
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CODE develop by Lucas Iacono
Contribution Alvaro Alonso
Function:

 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include <XBee.h>
#include <SHTxx.h>
#include <LowPower.h>
#include "Configuration.h"

#define DEBUG   1
#define AWAKE  LOW
#define SLEEP  HIGH

//SHT1X sensor
SHTxx sht71(DAT_SHT, SCK_SHT);
float tempC = 0;
float humidity = 0;

//XBee EndDevice
XBee xbee = XBee();
//XBee destination address - Coordinator Address
XBeeAddress64 addr64 = XBeeAddress64(XB_ADR_MSB, XB_ADR_LSB);
//XBeee Payload
uint8_t payload[] = { 0, 0, 0, 0};
//Xbee Tx & Rx functions
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

//Sleep time default
int xSeconds = 1; 

//-------------------------------------------------------------------------------------------
void setup() 
{
  /*** Setup the SHT15 ***/
#ifdef PWR_SHT
  pinMode(PWR_SHT, OUTPUT);
  digitalWrite(PWR_SHT, HIGH);
#endif
#ifdef GND_SHT
  pinMode(GND_SHT, OUTPUT);
  digitalWrite(GND_SHT, LOW);
#endif
  /*** Setup the Zigbee ***/
#ifdef SLEEP_PIN
  pinMode(SLEEP_PIN, OUTPUT);
  digitalWrite(SLEEP_PIN, AWAKE);
#endif
  Serial.begin(9600);
  xbee.begin(Serial);
  delay(1000);
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}
//-------------------------------------------------------------------------------------------
void loop() 
{
#ifdef SLEEP_PIN
  digitalWrite(SLEEP_PIN, AWAKE);//Wake up XBee
#endif
  readSensor();
  formPayload();
  sendData(); //enviamos los datos
  receiveData(); //recibimos los datos
#ifdef SLEEP_PIN
  digitalWrite(SLEEP_PIN, SLEEP); //XBee Sleep
#endif
  //delay(5000);
  enterSleep();// Arduino Sleep for received time
}
//-------------------------------------------------------------------------------------------
/*
 * Objective:
 * To read the values of temperature and Humidity from SHT sensor and store them on global variables
 */
void readSensor()
{
  // Read values from the sensor
  tempC = sht71.readTemperatureC();
  humidity = sht71.readHumidity(); 
}
//-------------------------------------------------------------------------------------------
/*
 * Objective: 
 * To store the readen values from SHT sensor on the payload array to be inserted into a ZB TX request. 
 */
void formPayload()
{
  payload[0] = (int) tempC;
  payload[1] = (int) ((tempC - payload[0]) * 100);
  payload[2] = (int) humidity;
  payload[3] = (int) ((humidity - payload[2]) * 100);
}
//-------------------------------------------------------------------------------------------
/*
 * Objective:
 * To Send the readen values from SHT via a ZB TX request to the network coordinator.
 * It will retry 3 times until a SUCCESS status is received.
 */
void sendData() 
{
  xbee.send(zbTx);
//  for (int i=0; i<3; i++) {
//    xbee.readPacket(1000);
//    if (xbee.getResponse().isAvailable() &&
//      xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
//      xbee.getResponse().getZBTxStatusResponse(txStatus);
//      if (txStatus.getDeliveryStatus() == SUCCESS) {
//        break;
//      }
//    } else {
//      xbee.send(zbTx);
//    }
//  }
}
//-------------------------------------------------------------------------------------------
/*
 * Objective:
 * To receive the number of cycles the system will sleep for and store them on global variable xSeconds.
 * It will return 
 */
int receiveData() 
{  
  for (int i=0; i<3; i++) {
    xbee.readPacket(1000);
    if (xbee.getResponse().isAvailable() && 
      xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        xbee.getResponse().getZBRxResponse(rx);    
        xSeconds = rx.getData()[0];
        //payload[5] = (int) xSeconds;
        //payload[1] = payload[2] = payload[3] = payload[4] = payload[0] = 0;
        //xbee.send(zbTx);
        break;
    }
  }
#if DEBUG
  for(int i=0; i<xSeconds; i++){
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
  }
#endif
  return xSeconds;
}
//-------------------------------------------------------------------------------------------
void enterSleep()
{
  int counter = 0;
  int xMinutes = xSeconds * 60;
  start:
  if (counter < xMinutes) {
  // ATmega328P, ATmega168
    LowPower.idle(SLEEP_TIME, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    counter ++;
    goto start; 
  }
}
