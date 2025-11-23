# Task-Queue RTC & LED Effects System – STM32F407 (FreeRTOS)

A menu-driven embedded application built on the **STM32F407 Discovery** board using **FreeRTOS**, featuring:

- Multiple **LED effects**
- Complete **RTC time/date configuration**
- **Real-time 1-second time reporting** over the **ITM Data Console**
- Fully non-blocking **task + queue + state-machine architecture**

This project demonstrates clean modular design for embedded systems using FreeRTOS.



## 🚀 Features

### **1. Serial Menu Interface (UART)**
A user-friendly interactive menu to control:
- LED Effects  
- Date & Time Settings  
- Time Reporting On/Off  

All user inputs flow through a command queue for safe, non-blocking processing.



### **2. LED Effects Engine**
Supports multiple built-in LED effects including:
- All Blink  
- Ping-Pong Sweep  
- Shift Left / Shift Right  
- Edge-to-Center Wave  
- Binary Counter  
- Random Sparkle  
- Wave Expand  
- Alternate Flash  
- Growing Bar  
- `none` — stop all effects  

Each effect runs independently using non-blocking logic.


### **3. RTC Configuration**
Interactive configuration for:
- Hours  
- Minutes  
- Seconds  
- AM/PM  
- Date  
- Month  
- Weekday  
- Year  

Input validation ensures only valid data is written to the RTC hardware.



### **4. Real-Time Reporting (ITM)**
- Pushes time/date every 1 second using a **FreeRTOS Software Timer**
- Output appears on the **ITM Data Console** via SWO
- Leaves UART free and provides very fast debug output



## 🧩 System Architecture

- **Command Task**  
  Receives UART input and sends decoded commands to the queue.

- **Menu/Logic Task**  
  Implements the state machine for LED control, RTC configuration, and reporting.

- **Print Task**  
  Handles all formatted output to UART via a print queue.

- **LED Task**  
  Executes LED animation effects without blocking the system.

- **RTC Timer Task**  
  Sends periodic timestamps when reporting mode is enabled.

All major modules follow a clean, modular structure.



## 🛠 Hardware Requirements

- **STM32F407 Discovery Board**
- USB cable for ST-Link  
- Optional: external LEDs if using off-board GPIO patterns  



## 🧪 Sample Serial Output
```
========================================
                MAIN MENU
======================================== 
[0] LED Effects 
[1] Date & Time Settings 
[2] Exit 
---------------------------------------- 
Enter your choice: 0
======================================== 
              LED EFFECTS 
======================================== 
Available Effects: 
[none] Stop All Effects 
[e1] All Blink 
[e2] Ping-Pong Sweep 
[e3] Shift Left 
[e4] Shift Right 
[e5] Edge-to-Center Wave 
[e6] Binary Counter 
[e7] Random Sparkle 
[e8] Wave Expand 
[e9] Alternate Flash 
[e10] Growing Bar 
---------------------------------------- 
Select an effect: e1

========================================
                MAIN MENU
======================================== 
[0] LED Effects 
[1] Date & Time Settings 
[2] Exit 
---------------------------------------- 
Enter your choice: 1
========================================
                    RTC
======================================== 

Current Time&Date: 01:00:58 [AM] 01-01-2000 
[0] Configure Time 
[1] Configure Date 
[2] Enable Reporting 
[3] Exit 
---------------------------------------- 
Enter your choice: 0
Enter hour (1-12): 7
Enter minutes (0-59): 22
Enter seconds (0-59): 24
Enter AM/PM: PM

Configuration successful 

Current Time&Date: 07:22:24 [PM] 01-01-2000 

========================================
                MAIN MENU
======================================== 
[0] LED Effects 
[1] Date & Time Settings 
[2] Exit 
---------------------------------------- 
Enter your choice: 
```



## ⚙️ Build & Run Instructions

1. Open the project in **STM32CubeIDE**
2. Connect your STM32F407 Discovery board
3. Build and flash via ST-Link
4. Open any serial terminal at:
   - **Baud:** 115200  
   - **Data:** 8N1
5. For real-time time/date logging:
   - Open **SWV → ITM Data Console** in CubeIDE
   - Enable SWO @ 2 MHz or appropriate frequency



## 📌 Future Enhancements

- Additional LED animations  
- On-board push-button menu navigation  
- Store user settings in Flash or Backup Registers  
- Add richer serial commands with history/autocomplete  
- Optional OLED display support  



## ⭐ Acknowledgements

Built as a clean demonstration of:
- FreeRTOS task scheduling  
- Queue-based communication  
- Non-blocking embedded design  
- RTC usage on STM32  
- SWO/ITM debugging  



Enjoy the project! Contributions, improvements, and suggestions are always welcome.
