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

/*===========================================
GLOBAL VARIABLES
=============================================*/
#define NUM_HISTORY 3
static const uint8_t slave_address = 0x04;
uint8_t rxbuf[10] = {0};
static WORKING_AREA(waThread_I2C, 128);
/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 1
=============================================*/
typedef struct {
  float temperature;
  float humidity;
} dht11;
struct dht11 data_dp1[NUM_HISTORY]
msg_t dataprod1_msg;

/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 2
=============================================*/
typedef struct {
  float x_axis;
  float y_axis;
  float z_axis
} adxl345;
struct adxl345 data_dp2[NUM_HISTORY]
msg_t dataprod2_msg;

/*===========================================
RECIVE DATA PRODUCER 1
=============================================*/
void recieveDataProd1(void){
  const uint8_t  req = 0; // request to know how to send on arduino
  dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&data_dp1,
                                   sizeof(data_dp1), MS2ST(1000));  
  chThdSleepMilliseconds(1000);                            
}
/*===========================================
RECIVE DATA PRODUCER 2
=============================================*/
void recieveDataProd2(void){
  const uint8_t  req = 1; // request to know how to send on arduino
  dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&data_dp2,
                                   sizeof(data_dp2), MS2ST(1000));  
  chThdSleepMilliseconds(1000);                            
}
/*===========================================
THREAD I2C BUS
=============================================*/
static msg_t Thread_I2C(void *p) {
  (void)p;
  chRegSetThreadName("Recive I2C");
 
  while (TRUE) {
    recieveDataProd1();
    recieveDataProd2();
    chThdSleepMilliseconds(2000);
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
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, (uint8_t)0x20);
    chThdSleepMilliseconds(10);
    
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, (uint8_t)0x20);
    chThdSleepMilliseconds(10);  
    
    chprintf((BaseSequentialStream *)&SD1, "Temperature: %d ºC  Humidity: %d ", data_dp1[0].temperature, data_dp1[0].humidity); 
   
    chThdSleepMilliseconds(2000);
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();

   /*
   * LCD Screen initialization.
   */
  sdStart(&SD1, NULL); 
  chprintf((BaseSequentialStream *)&SD1, "Loading .."); 
  
  /*
   * Thread LCD Screen
   */
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), HIGHPRIO-10, Thread_LCD, NULL);
  /*
   * I2C initialization.
   */
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);

   /*
   * Thread I2c BUS
   */
  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), 
                                          HIGHPRIO, Thread_I2C, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
