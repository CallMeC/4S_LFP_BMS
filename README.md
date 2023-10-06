# 4S Lithium Iron Phosphate BMS RP2040 based
This project was made between feb 2023 and apr 2023, it is still under developpement during my free time
This BMS is intented to be used with EVE LF105 LFP cells. It goes on top of them, and is attached thanks to screws. The busbar is inside the PCB.

# Features
This BMS is ables to :
- charge / discharge cells independently.
- monitor voltage of each cell
- monitor temperature of group of cells
- monitor current accurately thanks to an inside shunt

# Software
This BMS is based on the RP2040, the software is in early stage and not fully functionnal and tested yet.
Nevertheless, it can communicate to a computer by UART through USB. This way with a small TKINTER python programm we can easily monitor the different cells caracteristics.

![BMS](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/72ab0f5e-a8e1-4a47-b392-04adf977eee5)
![NEW](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/1208fe64-b45c-4f6f-b602-378c9aea19e5)
![ON_CELLS](https://github.com/CallMeC/4S_LFP_BMS/assets/47334555/59b917cf-1c63-4b61-be73-1db8412fd798)
