# 4S Lithium Iron Phosphate BMS RP2040 based
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
![2024-07-21-15-53-03-ezgif com-optimize](https://github.com/user-attachments/assets/21468d37-094f-4e1a-8739-3b3db1198431)


QT C++ Programm to visualise BMS characteristics in real time.


Assembly View

![TOP_VIEW](https://github.com/user-attachments/assets/ef44eb45-9708-485b-be5c-602ac48da8d0)

![SIDE_VIEW](https://github.com/user-attachments/assets/3444f80f-d5fe-44f3-8984-06d90694113d)


PCB Preview

![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/76dbcbad-aa51-4397-b71e-6b40d7a2273c)
![Capture d'écran 2024-02-19 112637](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/953ecdad-eed0-418a-b597-118a0777cbf7)
![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/76dbcbad-aa51-4397-b71e-6b40d7a2273c)
![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/298e1745-1c2e-4b97-8dc4-2f3d7fb2bee3)
![image](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/ce9d1496-16b1-4a63-81b4-489c1bb8399a)

# Thanks
Thanks to @Ustan26 for his great help reviewing code !

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
