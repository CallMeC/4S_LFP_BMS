# 4S Lithium Iron Phosphate BMS RP2040 based
This project was made between feb 2023 and apr 2023, it is still under developpement during my free time.
This BMS is intented to be used with EVE LF105 LFP cells. It goes on top of them, and is attached thanks to screws. The busbar is inside the PCB.

# Capabilities
The BMS can output up to 200A continuously and 400 peak. I tried to jumpstart a car, it works without problem.
The output voltage depends on capacity, a connctor was placed in order to add a daughter card to add USB-C power delivery capability.

# Features
This BMS is ables to :
- charge / discharge cells independently
- monitor voltage of each cell
- monitor temperature of group of cells
- monitor current accurately thanks to an inside shunt

# Performance
The BMS has the following characteristics :
- 4S Lithium Capability (LiFePO4)
- 12V Power Output
- 200A Continuous & 400A Peak current capability
- 1A Balancing current
- High accuracy cell voltage measurement
- Cell temperature measurement
- High accuracy current measurement
- Daisy-chaining capability (in case of pack)
- CAN 2.0 Communication
- Deep-Sleep mode with auto wake-up capbility
- Computer monitoring
- Active balancing extension board
- EEPROM

# Software
This BMS is based on the RP2040, the software is in early stage and not fully functionnal and tested yet.
Nevertheless, it can communicate to a computer by UART through USB. This way we can easily monitor the different cells caracteristics.

# Preview
![BMS_GUI_PREVIEW](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/9a78f79c-589f-48e5-881a-750def5a0536)
QT C++ Programm to visualise BMS characteristics un real time.

![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/76dbcbad-aa51-4397-b71e-6b40d7a2273c)
PCB v2 Preview

![BMS](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/72ab0f5e-a8e1-4a47-b392-04adf977eee5)
PCB v1 Preview

![NEW](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/1208fe64-b45c-4f6f-b602-378c9aea19e5)
PCB v1 Prototype

![ON_CELLS](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/59b917cf-1c63-4b61-be73-1db8412fd798)
PCB v1 Prototype put on cells

# Rev 2
A new version is currently under developpement. Here are some previews :

![Capture d'écran 2024-02-19 112637](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/953ecdad-eed0-418a-b597-118a0777cbf7)
![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/76dbcbad-aa51-4397-b71e-6b40d7a2273c)
![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/298e1745-1c2e-4b97-8dc4-2f3d7fb2bee3)
![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/ce9d1496-16b1-4a63-81b4-489c1bb8399a)

# License
MIT License

Copyright (c) 2024 Clément VERDA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
