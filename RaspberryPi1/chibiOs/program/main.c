/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "chvt.h"
/*===========================================
GLOBAL VARIABLES
=============================================*/
#define NUM_HISTORY 3
static const uint8_t slave_address = 0x04;
static WORKING_AREA(waThread_I2C, 256);
static WORKING_AREA(waThread_LCD, 128);
static mutex_t mtx1;
int cnt = 0;
I2CConfig i2cConfig;
/*===========================================
DECLARE FUNCTIONS
=============================================*/
void recieveDataProd1(void);
void recieveDataProd2(void);
void printDHT(void);
void setX(uint8_t posX);
void setY(uint8_t posY);
void setPos(uint8_t posX, uint8_t posY);
void clearScreen(void);
void reset(void);

/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 1
=============================================*/
typedef struct {
  float temperature;
  float humidity;
} dht11;
dht11 p1;

//dht11 data_dp1[NUM_HISTORY];
msg_t dataprod1_msg;

/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 2
=============================================*/
typedef struct {
  float x_axis;
  float y_axis;
  float z_axis;
} adxl345;
adxl345 data_dp2[NUM_HISTORY];
msg_t dataprod2_msg;
int prova =0;
/*===========================================
RECIVE DATA PRODUCER 1
=============================================*/
void recieveDataProd1(void){
  const uint8_t  req = 0; // request to know how to send on arduino
  //dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&p1,
   //                                8, MS2ST(1000));  
  dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&prova,
                                   4, MS2ST(2000));  

                                    cnt ++;
  chThdSleepMilliseconds(3000);   
                      
}
/*===========================================
RECIVE DATA PRODUCER 2
=============================================*/
void recieveDataProd2(void){
  const uint8_t  req = 1; // request to know how to send on arduino

  dataprod2_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&data_dp2,
                                   sizeof(data_dp2), MS2ST(1000));  
 
  chThdSleepMilliseconds(2000);                            
}
/*===========================================
THREAD I2C BUS
=============================================*/
static msg_t Thread_I2C(void *p) {
  (void)p;
  chRegSetThreadName("Recive I2C");
 

 p1.temperature = (float)23.1;
 p1.humidity = (float)57.6;
 //chThdSleepMilliseconds(100);
  while (TRUE) {
    chMtxLock(&mtx1);
    recieveDataProd1();
    //reset();
    //recieveDataProd2();
    //chThdSleepMilliseconds(2000);
    chMtxUnlock(&mtx1);
  }
  return 0;
}
/*===========================================
THREAD LCD PRINT
=============================================*/
static msg_t Thread_LCD(void *p) {
  (void)p;
  chRegSetThreadName("SerialLCD");

  while (TRUE) {
    chMtxLock(&mtx1);
    clearScreen();
    printDHT();
    reset();
    chMtxUnlock(&mtx1);
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();
  /*
   * Mutex initialization.
   */
  chMtxObjectInit(&mtx1);
  /*
   * LCD Screen initialization.
   */
  sdStart(&SD1, NULL); 
  /*
   * I2C initialization.
   */
  i2cStop(&I2C0);
  i2cStart(&I2C0, &i2cConfig);

  /*
   * Thread LCD Screen
   */
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), NORMALPRIO, Thread_LCD, NULL);

   /*
   * Thread I2c BUS
   */
  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), 
                                          HIGHPRIO, Thread_I2C, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}

void printDHT(void){
   chThdSleepMilliseconds(100);
  setPos(0,13);
    chprintf((BaseSequentialStream *)&SD1, "Temp: %f C", p1.temperature); 
   chThdSleepMilliseconds(100);
   setPos(0,28);
   chprintf((BaseSequentialStream *)&SD1, "Hum: %f", p1.humidity);
   chThdSleepMilliseconds(100);
   setPos(0,43);
   chprintf((BaseSequentialStream *)&SD1, " CNT: %d", cnt);
  chThdSleepMilliseconds(100);
  setPos(0,58);
   chprintf((BaseSequentialStream *)&SD1, " prov: %d", prova);
    chThdSleepMilliseconds(100);
  setPos(60,58);
   chprintf((BaseSequentialStream *)&SD1, "Reset: %d", dataprod1_msg);
   
    chThdSleepMilliseconds(5000);
}

void reset(void){
    switch (dataprod1_msg)
  {
  case Q_TIMEOUT:
    clearScreen();
  chprintf((BaseSequentialStream *)&SD1, "TIMEOUT");
    break;
  case Q_OK:
  clearScreen();
  chprintf((BaseSequentialStream *)&SD1, "RECIVED OK");
    break;
  case Q_RESET:
    clearScreen();
    chprintf((BaseSequentialStream *)&SD1, "Reset: %d", dataprod1_msg);
    i2cflags_t i2cFlags = i2cGetErrors(&I2C0);
    chprintf((BaseSequentialStream *)&SD1, "Flags: %d", i2cFlags);
    i2cStop(&I2C0);
    i2cStart(&I2C0, &i2cConfig);
    break;
  default:
    break;
  }
}

void setX(uint8_t posX)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x18);//CTRL x
  sdPut(&SD1, (uint8_t) posX);
  chThdSleepMilliseconds(50);
}

void setY(uint8_t posY)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x19);//CTRL y
  sdPut(&SD1, (uint8_t)posY);
  chThdSleepMilliseconds(50);
}

void setPos(uint8_t posX, uint8_t posY)
{
  setX(posX);
  setY(posY);
}

void clearScreen(void)
{
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t) 0);
}
