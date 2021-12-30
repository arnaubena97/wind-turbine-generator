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
#define NUM_HISTORY 8
#define MAX_Y 63
#define MAX_X 127
#define SPEED 10000
MUTEX_DECL(mtx1);
//Mutex mtx1 = _MUTEX_DATA(mtx1);
static const uint8_t slave_address = 0x04;
static WORKING_AREA(waThread_I2C, 512);
static WORKING_AREA(waThread_LCD, 256);
/*===========================================
DECLARE FUNCTIONS
=============================================*/
void recieveDataProd1(void);
void recieveDataProd2(void);
void printDHT(void);
void printAdxl(void);
void setX(uint8_t posX);
void setY(uint8_t posY);
void setPos(uint8_t posX, uint8_t posY);
void clearScreen(void);
void reset(msg_t data, int type);
void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void drawDegree(uint8_t x, uint8_t y);
void drawGraphic(uint8_t x, uint8_t y);
void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void drawColumn(uint8_t x, uint8_t y, uint8_t val);
void drawCube(uint8_t x, uint8_t y);
/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 1
=============================================*/
typedef struct {
  uint8_t humidity;
  uint8_t temperature[NUM_HISTORY];
  char name_id[3];
} dht11;
dht11 p1;
msg_t dataprod1_msg;

/*===========================================
STRUCT AND VARIABLES OF DATA PRODUCER 2
=============================================*/
typedef struct {
  uint8_t x_axis;
  uint8_t y_axis;
  uint8_t z_axis;
  char name_id[3];
} adxl345;
adxl345 p2;
msg_t dataprod2_msg;

/*===========================================
RECIVE DATA PRODUCER 1
=============================================*/
void recieveDataProd1(void){
  const uint8_t  req = (uint8_t)0; // request to know how to send on arduino
  //const uint8_t  req2 = (uint8_t)1; 
  /*clearScreen();
    setPos(2,20);
    chprintf((BaseSequentialStream *)&SD1, "Reciving dp1");*/
  dataprod1_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&p1, sizeof(p1) , MS2ST(3000)); 
  reset(dataprod1_msg, 1);                         
  chThdSleepMilliseconds(2000);   
   //   setPos(2,40);
    //chprintf((BaseSequentialStream *)&SD1, "Recived dp1: %d", dataprod1_msg);                                
              
}
/*===========================================
RECIVE DATA PRODUCER 2
=============================================*/
void recieveDataProd2(void){
  const uint8_t  req = (uint8_t)1; // request to know how to send on arduino
  clearScreen();
    setPos(2,20);
    chprintf((BaseSequentialStream *)&SD1, "Reciving dp2");
  dataprod2_msg = i2cMasterTransmitTimeout(&I2C0, slave_address, &req, 1, (uint8_t *)&p2,
                                   sizeof(p2), MS2ST(1000));  
  reset(dataprod2_msg, 2);   
  chThdSleepMilliseconds(2000); 
        setPos(2,40);
    chprintf((BaseSequentialStream *)&SD1, "Recived dp2: %d", dataprod2_msg);                                
}
/*===========================================
THREAD I2C BUS
=============================================*/
static msg_t Thread_I2C(void *p) {
  (void)p;
  chRegSetThreadName("Recive I2C");

 chThdSleepMilliseconds(100);
  while (TRUE) {
    chMtxLock(&mtx1); 
    recieveDataProd1();
    chThdSleepMilliseconds(500);
    recieveDataProd2();
    chMtxUnlock();
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
    chMtxLock(&mtx1);
    printDHT();
    printAdxl();
    chMtxUnlock();
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
  i2cConfig.ic_speed = SPEED;
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

void printDHT(void){
    chThdSleepMilliseconds(100);
  
  if(dataprod1_msg==0){
    clearScreen();
    drawBox(0, MAX_Y, 46, MAX_Y-10);
    setPos(2,MAX_Y -2);
    chprintf((BaseSequentialStream *)&SD1, "DP1-%s", p1.name_id);
    setPos(1,MAX_Y -24);
    chprintf((BaseSequentialStream *)&SD1, "T: %d ", p1.temperature[7]);
    drawDegree(33, MAX_Y-25);
    setPos(35,MAX_Y -24);
    chprintf((BaseSequentialStream *)&SD1, "%s", "C");
    //setpixels
    chThdSleepMilliseconds(100);

    setPos(1,MAX_Y -41);

    chprintf((BaseSequentialStream *)&SD1, "H: %d ", p1.humidity);
    sdPut(&SD1, (uint8_t)37);
    chThdSleepMilliseconds(100);

    drawGraphic(65, 10);

    chThdSleepMilliseconds(4000);
    
  }
  
}

void printAdxl(void){
    chThdSleepMilliseconds(100);
  
  if(dataprod2_msg==0){
    clearScreen();
    drawBox(0, MAX_Y, 46, MAX_Y-10);
    setPos(2,MAX_Y -2);
    chprintf((BaseSequentialStream *)&SD1, "DP2-%s", p2.name_id);

    setPos(1,MAX_Y -20);
    chprintf((BaseSequentialStream *)&SD1, "X: %d ", p2.x_axis);

    setPos(1,MAX_Y -35);
    chprintf((BaseSequentialStream *)&SD1, "Y: %d ", p2.y_axis);

    setPos(1,MAX_Y -50);
    chprintf((BaseSequentialStream *)&SD1, "Z: %d ", p2.z_axis);

    chThdSleepMilliseconds(100);

    drawCube(70,23);

    chThdSleepMilliseconds(4000);
    
  }
  
}

void reset(msg_t data, int type){
  if(type==0){
    type =9;
  }
  
    switch (data)
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
      chThdSleepMilliseconds(500);
      I2CConfig i2cConfig;
      chThdSleepMilliseconds(500);
      i2cStop(&I2C0);
      chThdSleepMilliseconds(500);
      i2cConfig.ic_speed = SPEED;
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

void drawGraphic(uint8_t x, uint8_t y){
  //axis
  drawLine(x, y, x, y+50);
  drawLine(x,y,x+60, y);
  // values
  drawLine(x-3, y+50, x, y+50); //40
  drawLine(x-3, y+40, x, y+40); //30
  drawLine(x-3, y+30, x, y+30); //20
  drawLine(x-3, y+20, x, y+20); //10
  drawLine(x-3, y+10, x, y+10); //0

  setPos(x, y-3);
  chprintf((BaseSequentialStream *)&SD1, "Time ->");

  drawDegree(x-9, MAX_Y-2);
  setPos(x-7,MAX_Y -1);
  chprintf((BaseSequentialStream *)&SD1, "%s", "C");

  setPos(x-9, y+13);
  chprintf((BaseSequentialStream *)&SD1, "0");
    setPos(x-14, y+33);
  chprintf((BaseSequentialStream *)&SD1, "20");

chThdSleepMilliseconds(100);
  drawColumn(x+3, y, p1.temperature[0]);
  drawColumn(x+9, y, p1.temperature[1]);
  drawColumn(x+15, y, p1.temperature[2]);
  drawColumn(x+21, y, p1.temperature[3]);
  drawColumn(x+27, y, p1.temperature[4]);
  drawColumn(x+33, y, p1.temperature[5]);
  drawColumn(x+39, y, p1.temperature[6]);
  drawColumn(x+45, y, p1.temperature[7]);


}

void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
  //draws a line from two given points. You can set and reset just as the pixel function. 
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x0C);
  sdPut(&SD1, (uint8_t)x1);
  sdPut(&SD1, (uint8_t)y1);
  sdPut(&SD1, (uint8_t)x2);
  sdPut(&SD1, (uint8_t)y2);
  sdPut(&SD1, (uint8_t)0x01);
}

void drawColumn(uint8_t x, uint8_t y, uint8_t val){
  if (val != 0){
    drawLine(x, y+10, x, y +val +10);
    drawLine(x+1, y+10, x+1, y+ val +10);
    drawLine(x+2, y+10, x+2, y +val +10);
  }else if (val==0){
    drawLine(x, y+10, x+3, y+10);
  }
}

void drawCube(uint8_t x, uint8_t y){

  drawLine(x,y,x+50, y+25);
  drawLine(x,y,x, y-20);
  drawLine(x,y, x-30, y+20);
  drawLine(x,y-20,x+50, y+5);
  drawLine(x+50,y+25,x+50, y+5);
  drawLine(x,y-20, x-30, y);
  drawLine(x-31,y, x-31, y+20);
  drawLine(x-31,y+20, x+4, MAX_Y-1);
  drawLine(x+50,y+25, x+29, MAX_Y-1);
    setPos(x+20, y+3);
  chprintf((BaseSequentialStream *)&SD1, "X");
    setPos(x-17, y+1);
  chprintf((BaseSequentialStream *)&SD1, "Y");
    setPos(x+4, y+23);
  chprintf((BaseSequentialStream *)&SD1, "Z");

}



