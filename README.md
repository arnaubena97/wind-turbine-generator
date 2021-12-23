# Wind-turbine-generator
## Table of contents
* [General info](#general-info)
* [Data Producer 1](#data-producer-1)
* [Data Producer 2](#data-producer-2)
* [MQTT Broker](#mqtt-broker)
* [Data Consumer](#data-consumer)
* [Testing with Mosquitto](#testing-with-mosquitto)
* [Contributors](#contributors)



## General info

Project of the subject: Ubiquitous Computing & Embedded Systems.


![project_all](https://user-images.githubusercontent.com/10574631/141642325-59ed1986-85b4-43cc-8574-5b84bb682db2.png)


## Data Producer 1
Components:
* ESP-01
* DHT11
* LCD Screen 128x64

Connections:
![image](https://user-images.githubusercontent.com/10574631/146461469-68cd5f64-a277-434a-94d7-b0d80d35c5c7.png)

Working:
![image](https://user-images.githubusercontent.com/10574631/146462222-d41a657e-3c51-4086-b997-24b5d4edfbf1.png)

## Data Producer 2

  Components:
  * ESP-01
  * ADXL345
  * LCD Display 16x2
  * PCF8574T

  Connections:

  ![dp2](https://user-images.githubusercontent.com/10574631/141659065-3336e771-082f-4c07-b2f8-35d228788038.png)

  Working:

  ![dp2work](https://user-images.githubusercontent.com/10574631/141686026-fedbf773-6796-4589-894b-0f5f358a0f54.gif)

## MQTT Broker
Components:
* NodeMcu
* MCP23017
* LEDs

Connections:
![MQTT](https://user-images.githubusercontent.com/10574631/144288629-935dc973-7754-47f7-9460-0a5b0d6aa721.png)


## Data Consumer

This part consists of displaying on the LCD Screen the data collected in [Data Producer 1](#data-producer-1) and [Data Producer 2](#data-producer-2) that have been sent by the wifi and send them on the raspberry via I2C Bus with an Arduino slave.
We have put a temperature history and a drawing to understand on which axis there is acceleration.

Components:
* Arduino Uno
* ESP-01
* LCD Screen 128 x 64
* Raspberry Pi 1 2011.12

Connections:

LCD Screen Examples:

![Lcd-Examples](https://user-images.githubusercontent.com/10574631/147292413-78cee10e-c682-46a6-baf2-38729fa778c2.png)

## Testing with Mosquitto

To know if [Data Producer 1](#data-producer-1) and [Data Producer 2](#data-producer-2) works fine we use [Mosquitto](https://mosquitto.org). We publish the data on MQTT, this can be via the [MQTT Broker](#mqtt-broker) or connect to your personal wifi.
You need to connect the computer to wifi that publish the data. Open the terminal and run:

```
mosquitto_sub -h 192.168.4.1 -t # -q 1 -u q
```
In my case, the IP are 192.168.4.1(default). I'm subscribed in all: "#". The quality QoS: "1" and I'm the user: "q".
If you need more clients, can use the same command with different user.
An example for the data that i recieve from [Data Producer 1](#data-producer-1) and [Data Producer 2](#data-producer-2):

![image](https://user-images.githubusercontent.com/10574631/146458070-a1b922c5-70de-496b-b252-ccd507972ad5.png)

## Contributors
<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/arnaubena97"><img src="https://avatars0.githubusercontent.com/u/10574631?s=460&v=4" width="100px;" alt=""/><br /><sub><b>Arnau Benavides</b></sub></a></td>
    <td align="center"><a href="https://github.com/MarcFelip"><img src="https://avatars.githubusercontent.com/u/58440805?v=4" width="100px;" alt=""/><br /><sub><b>Marc Felip</b></sub></a><br /></td>

  </tr>
 </table>
<!-- ALL-CONTRIBUTORS-LIST:END -->