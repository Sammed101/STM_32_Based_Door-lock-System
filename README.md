# STM32 Keypad Door Lock System with LCD & Relay

## 📘 Overview
This project is a **4x4 keypad-based door lock system** built using the **STM32F103C6 (Blue Pill)** microcontroller.  
Users enter a password on the keypad, which is verified by the MCU. If correct, it **unlocks the door** by activating a relay (or solenoid) connected to **PC13**, and feedback is shown on an **I2C 16x2 LCD**.

---

## ⚙️ Features
- 🔢 **4x4 Keypad** for password input  
- 🔐 **Password verification with masked display**  
- 💡 **LCD display** shows status messages (e.g., “Access Granted”)  
- ⚡ **Relay or solenoid control** through PC13  
- 🕒 **Debounce delay** for stable key input  
- 🧱 **Simple, clean HAL-based STM32 project structure**

---

## 🧰 Hardware Requirements
| Component | Description |
|------------|-------------|
| **STM32F103C6 / Blue Pill** | Main microcontroller |
| **4x4 Matrix Keypad** | Password input |
| **I2C LCD (16x2)** | Display feedback |
| **Relay (5V)** | To control lock or solenoid |
| **1N4007 Diode** | Flyback protection |
| **BC547 / IRLZ44N Transistor** | To drive relay or solenoid |
| **5V Power Supply** | System power |

---

## 🔌 Pin Configuration

### 🧮 Keypad (4x4 Matrix)
| Function | STM32 Pin | Description |
|-----------|------------|-------------|
| Row 1 | PB0 | `Rows_Pin` |
| Row 2 | PB1 | `RowsB1_Pin` |
| Row 3 | PB10 | `RowsB12_Pin` |
| Row 4 | PB11 | `RowsB13_Pin` |
| Column 1 | PA0 | `Columns_Pin` |
| Column 2 | PA1 | `ColumnsA2_Pin` |
| Column 3 | PA2 | `ColumnsA3_Pin` |
| Column 4 | PA3 | `ColumnsA4_Pin` |

### 💡 LCD (I2C)
| Function | STM32 Pin | Description |
|-----------|------------|-------------|
| SDA | PB7 | I2C Data |
| SCL | PB6 | I2C Clock |
| VCC | 5V | Power |
| GND | GND | Ground |

### 🔐 Relay / Solenoid Control
| Component | STM32 Pin | Description |
|------------|------------|-------------|
| Relay / Transistor | **PC13** | Output signal to activate door lock |
| Diode | Across relay coil | Protects from back EMF |
| Solenoid | Controlled via relay or MOSFET | Door mechanism |

> 💡 The relay or solenoid activates when the correct password is entered and turns off automatically after a delay.

---

## 🧩 Software Used
- **STM32CubeIDE** – Development environment  
- **STM32CubeMX** – HAL configuration and pin setup  
- **ST-Link Utility** – Flash firmware to STM32  

---

## 📁 Folder Structure
```
STM32_Keypad_Door_Lock/
├── Core/
│   ├── Inc/
│   └── Src/
├── Drivers/
│   ├── CMSIS/
│   └── STM32F1xx_HAL_Driver/
├── STM32_Keypad_Door_Lock.ioc
├── README.md
└── .project / .cproject
```

---

## 🚀 How to Use
1. Open the project in **STM32CubeIDE**.  
2. Verify pin configuration (especially PC13 for relay).  
3. Build and flash firmware using **ST-Link**.  
4. Power the setup with **5V**.  
5. Enter the default password `1234`.  
   - ✅ Correct → “Access Granted” on LCD & relay turns ON.  
   - ❌ Wrong → “Wrong Password” message.  
6. The relay (or solenoid) automatically turns OFF after a short delay.

---

## 🧠 Future Enhancements
- Change password feature via keypad  
- Save password in internal flash memory  
- Add buzzer for feedback or alarms  
- IoT/Wi-Fi integration for remote control  
- Add RTC for time-based auto-lock

---

## 👨‍💻 Author
**Sammed Ramdhave**  
📧 [sammedramdhave07@gmail.com](mailto:sammedramdhave07@gmail.com)  
🔗 [LinkedIn](https://www.linkedin.com/in/sammed-ramdhave)
