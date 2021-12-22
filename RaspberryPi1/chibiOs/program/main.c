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
#include <math.h>
/*===========================================
GLOBAL VARIABLES
=============================================*/
#define NUM_HISTORY 3
#define MAX_Y 63
#define MAX_X 127
MUTEX_DECL(mtx1);
//Mutex mtx1 = _MUTEX_DATA(mtx1);
static const uint8_t slave_address = 0x04;
static WORKING_AREA(waThread_I2C, 512);
static WORKING_AREA(waThread_LCD, 256);
int cnt = 0;
char provastr[20] = "Arnau";
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
void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void drawDegree(uint8_t x, uint8_t y);
/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 1
=============================================*/
typedef struct {
  uint8_t humidity;
  uint8_t temperature;
  char name_id[8];
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
  
chMtxLock(&mtx1); 
  dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&p1, sizeof(dht11) , MS2ST(5000)); 
  
  //dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&prova,4, TIME_INFINITE);  
  
  chThdSleepMilliseconds(2000);
  
  cnt ++;
  
  reset();
  chMtxUnlock();
                                    
  chThdSleepMilliseconds(2000);   
                    
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

 p1.temperature = (uint8_t) 0;
 p1.humidity = (uint8_t) 2;
 chThdSleepMilliseconds(100);
  while (TRUE) {
    recieveDataProd1();
    //recieveDataProd2();
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
    printDHT();
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
  /*
   * I2C initialization.
   */
  I2CConfig i2cConfig;
  //i2cStop(&I2C0);
  i2cConfig.ic_speed = 10000;
  i2cStart(&I2C0, &i2cConfig);
  /*
   * Thread LCD Screen
   */
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), LOWPRIO, Thread_LCD, NULL);

   /*
   * Thread I2c BUS
   */
  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C),HIGHPRIO, Thread_I2C, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
  
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
  
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
  
    reverse(str, i);
    str[i] = '\0';
    return i;
}
int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y % 2 == 0)
        return power(x, y / 2) * power(x, y / 2);
    else
        return x * power(x, y / 2) * power(x, y / 2);
}

void printfloat(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    float fpart = n - (float)ipart;
  
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * power(10, afterpoint);
  
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
void printDHT(void){
    chThdSleepMilliseconds(100);
  chMtxLock(&mtx1);
  if(dataprod1_msg==0){
    clearScreen();
    drawBox(0, MAX_Y, 46, MAX_Y-10);
    setPos(2,MAX_Y -2);
    chprintf((BaseSequentialStream *)&SD1, "%s", p1.name_id);
    setPos(1,MAX_Y -24);
    chprintf((BaseSequentialStream *)&SD1, "%d ", p1.temperature);
    drawDegree(15, MAX_Y-25);
    setPos(17,MAX_Y -24);
    chprintf((BaseSequentialStream *)&SD1, "%s", "C");
    //setpixels
    chThdSleepMilliseconds(100);

    setPos(1,MAX_Y -41);

    chprintf((BaseSequentialStream *)&SD1, "%d ", p1.humidity);
    sdPut(&SD1, (uint8_t)37);
    chThdSleepMilliseconds(100);

    chThdSleepMilliseconds(4000);
    
  }
  chMtxUnlock();
}

void reset(void){
    switch (dataprod1_msg)
  {
    case RDY_OK:
      //clearScreen();
      //chprintf((BaseSequentialStream *)&SD1, "RECIVED OK");
      break;
    case RDY_TIMEOUT:
      //clearScreen();
      //chprintf((BaseSequentialStream *)&SD1, "TIMEOUT");
      break;
    case RDY_RESET:
      clearScreen();
      chprintf((BaseSequentialStream *)&SD1, "Reset: %d", dataprod1_msg);
      i2cflags_t i2cFlags = i2cGetErrors(&I2C0);
      chprintf((BaseSequentialStream *)&SD1, " Flags: %d", i2cFlags);
      i2cStop(&I2C0);
      chThdSleepMilliseconds(500);
      I2CConfig i2cConfig;
      i2cConfig.ic_speed = 10000;
      chThdSleepMilliseconds(500);
      i2cStart(&I2C0, &i2cConfig);
      chThdSleepMilliseconds(1000);
      break;
    default:
      break;
  }
  chThdSleepMilliseconds(2000);
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

void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  //draws a box from two given points. You can set and reset just as the pixel function. 
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x0F);
  sdPut(&SD1, (uint8_t)x1);
  sdPut(&SD1, (uint8_t)y1);
  sdPut(&SD1, (uint8_t)x2);
  sdPut(&SD1, (uint8_t)y2);
  //sdPut(&SD1, (uint8_t)0x01);
  chThdSleepMilliseconds(10);
}

void setPixel(uint8_t x, uint8_t y)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x10);//CTRL p
  sdPut(&SD1, (uint8_t)x);//CTRL p
  sdPut(&SD1, (uint8_t)y);
  sdPut(&SD1, (uint8_t)0x01);
}
void drawDegree(uint8_t x, uint8_t y)
{
  setPixel(x,y);
  setPixel(x-1,y);
  setPixel(x-2,y);
  setPixel(x-2,y-1);
  setPixel(x-2,y-2);
  setPixel(x,y-1);
  setPixel(x,y-2);
  setPixel(x-1,y-2);
  
}
