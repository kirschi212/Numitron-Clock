# Numitron Clock by Kirschi

This is a small documentation if you want to build a Numitron Clock by yourself. This project is build around IV-9 tubes, an ESP8266 and the MAX7315 IO-Expander.

## Images

![alt text](https://i.imgur.com/WxeO8AT.jpeg)

![alt text](https://i.imgur.com/QRNSPmU.jpeg)

## Functioning

The ESP8266 is connected to the WiFi and retrieves the current time from a time server. The MAX7315 IO expanders are then controlled by the ESP8266 via I2C and switch the corresponding segments of the Numitron segments.
Optionally, a switch can also be installed with which the tubes can be switched off.

------

## Parts list

### Main Components:

                   
Component  | Quantity | Description |
------------- | ------------- | ------------- |
ESP8266 NodeMCU  | 1 | heart of the circuit |
IV-9 numitron tubes | 4 | retro 7 segment displays |
MAX7315AEE+ | 4 | 8-bit IO-Expanders |
Custom PCB | 1 | Eagle.CAD files are uploaded in this repository |

### Other Components:

                    
| Component  | Quantity | Description |
| ------------- | ------------- | ------------- | 
| Resistor 0603 47Ω  | 32  | current limiting resistors for numitron segments |
| Resistor 0603 4.7kΩ  | 2  | pull-up resistors for I2C-Bus |
| Ceramic capacitors  0603 0.047µF  | 4  | decoupling capacitors for MAX7315 |
| USB-C panel jack  | 1  | used for power supply via USB-C |
                    
### Optional Components:
These components can optionally be installed.

| Component  | Quantity | Description |
| ------------- | ------------- | ------------- |
| Resistor 0603 10kΩ  | 1 | pull-up resistor for switch |
| Electrolytic capacitor 100µF   | 1  | capacitor for voltage stabilization |
| Mechanical switch  | 1  | used for turning all segments on or off |

##### Notice: Either the electrolytic capacitor OR switch can be installed with this version of the PCB-design.

## PCB: schematic and board

#### Notice: This current version of the PCB shows some minor mistakes, e.g. the electrolytic capacitor can only be placed without installing a switch. Nevertheless the board is fully functional! If you are interested, I could design a second version of this board and I will try to improve it!

![alt text](https://i.imgur.com/8OrW7NU.jpeg)

![alt text](https://i.imgur.com/ZAZqbi8.jpeg)

## Casing

In the repository you can find the Fusion360 file to the casing that I designed myself. It features mounting holes for the PCB, cutouts for the USB-C panel jack and the switch. You can print it with a common 3D-printer using PLA or PETG.

------
## Code

The code was designed to be uploaded to the ESP8266 via the Arduino IDE and a USB-connection. 

![alt text](https://i.imgur.com/cIVGxHw.png)


