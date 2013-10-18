##Medical robotic arm sensitizer

###This poject includes two parts:
1. Embedded system on STM32 microcontroller which handles ATI Force/Torque Sensor System and communicate with medical robotic arm control system via CAN bus
2. PC program module which provides sensor calibration interface and Force/Torque compensation of gravity

###Directories description:
**/target/** - target embedded system code  
**/can_adapter/** - auxiliary embedded system code which emulate medical robotic arm control system for target  
**/pc/** - PC program modules  
**/pc/calc/** - module providing real time calculations  
**/pc/context/** - module providing context generation for **calc**  
**/pc/converter/** - module providing data format convertation from logic analizer raw to **calc** module input stream  
**/pc/toolkit/** - starting all the modules and plotting before and after processing  

All code was wtitten in clear C99.  
All embedded code was compiled and debugged in Keil uVision.  
All pc code was compilled by gcc-4.8.1 and debugged in native linux environment.  

**Manual of integrating calc module whith medical robotic arm control system is in /pc/manual**