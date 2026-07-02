# DIY-DAP-walkman-style
Hear your favourite music without any interruption by the palm of your hands,
with an old school vibes going on with DIY DAP Walkman style.

## Description 
  With ESP32 as the brain (Main processor) you could make a cheap DAP (Digital Audio Player) filled with your favouri♪te 
Music playlist, with ESP32 as the microcontroller, SD card module to filled with your Mp3 music files and some Audio jack 3.5 mm to hear it throught your earphone
with some niche button like the walkman has. And you could add display to make some UI ! >:D.

## Project Preview
<img width="3504" height="2478" alt="Assembly 1 (4)" src="https://github.com/user-attachments/assets/d1b925e1-8660-4ad1-b4a9-4647ffaabc56" />
<img width="3504" height="2478" alt="Assembly 1" src="https://github.com/user-attachments/assets/c014ba1f-02a6-408f-b7ce-49a0e978652e" />


## 3D model
Bwlow here is the full CAD assembly of the project, the .STEP files on the /CAD Files Folder
<img width="3504" height="2478" alt="Assembly 1 (2)" src="https://github.com/user-attachments/assets/c26b8b57-2dc4-4ba1-8e1e-a2e0092fc991" />
<img width="3504" height="2478" alt="Assembly 1 (4)" src="https://github.com/user-attachments/assets/1e2a1cc8-7655-4b34-bda6-d7fb63b399a6" />

## Source Design
https://cad.onshape.com/documents/cdf923aa097df220a4b09fb1/w/958292e6abd58ac5f25ffee2/e/462f93d2c8d04da2a323aa71?renderMode=0&uiState=6a429a18e3ae4b6fae6d3625

## Wiring Diagram
Shows how the ESP32 connect or wired with the DAC and other components
<img width="668" height="403" alt="Screenshot 2026-07-02 100318" src="https://github.com/user-attachments/assets/df059d82-8199-4f08-817c-55e55d9f02ac" />


## Features :D
- Play Mp3 files by the SD card
- The button have contorol to Play/pause, Next, Previous
- 3.5 mm headphone or earphone outpur
- "Walkman Style" with 3D Printed case

## Component I use (almost everthing in module)
- ESP32-C3-DevKit module (mainboard)
- PCM5102 DAC Module with audiojack 3.5 mm
 (if the DAC module doesn't have any audio jack you need) audiojack 3.5 mm
- Micro SDcard Module
- OLED I2C 0.91'' inch
- MT608 Modul Step Up Boost DC to DC 2A USB Type-C
- TP4056 module TYPE-C
- 3.7 V 18650 li ion battery
- One battery holder
- 3 Push button

## BOM (Bill of Materials)
BOM Files on the /BOM (Bill Of Materials) folder ;)
Components List

| Name | Purpose | Qty | Total Cost (USD) | Source / Link | Distributor |
| :--- | :--- | :---: | :---: | :---: | :---: |
| **Tactile Switch** | As the control of the DAP | 3 | $0.17 | [Link](https://tk.tokopedia.com/ZSCdCuva5/) | Tokopedia |
| **One battery holder** | To hold the battery | 1 | $0.27 | [Link](https://tk.tokopedia.com/ZSCdCpkyx/) | Tokopedia |
| **3.7 V 18650 li-ion battery** | As the power supply | 1 | $0.49 | [Link](https://tk.tokopedia.com/ZSCdCrD4L/) | Tokopedia |
| **TP4056 module TYPE-C** | To charge up the battery | 1 | $0.34 | [Link](https://tk.tokopedia.com/ZSCdQsH1c/) | Tokopedia |
| **MT608 Modul Step Up Boost DC to DC 2A USB TYPE C** | For stepping up the current | 1 | $0.63 | [Link](https://tk.tokopedia.com/ZSCdQSWaG/) | Tokopedia |
| **OLED I2C 0.91'' inch Module** | As the screen | 1 | $1.76 | [Link](https://tk.tokopedia.com/ZSCdxTECL/) | Tokopedia |
| **Micro SD card module reader** | To store our music file | 1 | $0.56 | [Link](https://tk.tokopedia.com/ZSCdQ1gt9/) | Tokopedia |
| **PCM5102 DAC Module with audiojack 3.5 mm** | For changing Digital signal into Analog | 1 | $2.80 | [Link](https://tk.tokopedia.com/ZSCdxNCav/) | Tokopedia |
| **ESP32 C3 DevKit** | As the mainboard | 1 | $4.54 | [Link](https://tk.tokopedia.com/ZSCdxLQtW/) | Tokopedia |

---
*Note: Costs are estimation values in USD based on local distributor pricing.*


## Firmware
FIrmware is on the /Firmware folder and it is written using C++ in Arduino IDE framework

## Files Included
- CAD files (.STEP & source)
- Wiring Diagram
- BOM
- firmware




