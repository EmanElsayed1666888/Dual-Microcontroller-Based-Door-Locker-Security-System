# Dual-Microcontroller-Based-Door-Locker-Security-System
# 🔐 Dual Microcontroller-Based Door Locker Security System

## 📌 Project Overview
This project is a **secure and efficient smart door control system** built using **two ATmega32 microcontrollers** and powered by **FreeRTOS**.  
It implements **password authentication**, **motorized door control**, **motion detection**, and **buzzer alerts** for enhanced security.  

The project was developed as part of the **Standard Embedded Diploma (Final Project)**.

---

## 🎯 Objectives
- Develop a **dual microcontroller system** for secure door access.  
- Implement **password-based authentication** with EEPROM storage.  
- Enhance functionality with **sensors, buzzer alerts, and motorized control**.  
- Demonstrate **modular embedded software design** using layered architecture.

---

## ⚡ Features
- 🔑 **Password Authentication** stored securely in external EEPROM  
- 📟 **LCD & Keypad Interface** for user interaction (HMI_ECU)  
- 🔄 **UART Communication** between HMI_ECU & Control_ECU  
- ⚙️ **Motorized Door Control** via H-Bridge & PWM  
- 👁️ **PIR Motion Sensor** for intelligent door handling  
- 🚨 **Buzzer Alerts & Lockout** after multiple failed attempts  
- 🔄 **Password Change Option** for enhanced security  

---

## 🛠️ Hardware Components
- **ATmega32 Microcontrollers** (HMI_ECU & Control_ECU)  
- **16x2 LCD Display** (8-bit mode)  
- **4x4 Keypad**  
- **External EEPROM (I2C)**  
- **DC Motor + H-Bridge Motor Driver**  
- **PIR Motion Sensor**  
- **Buzzer**  

---

## 📂 System Architecture
- **HMI_ECU** → LCD, Keypad, UART Communication  
- **Control_ECU** → EEPROM, Motor, PIR Sensor, Buzzer, Security Logic  

Communication between the two ECUs is handled via **UART**.  

---

## 📜 Drivers Implemented
- GPIO Driver  
- UART Driver  
- I2C (TWI) Driver  
- LCD Driver  
- Keypad Driver  
- Timer Driver (Normal & Compare modes with interrupts & callbacks)  
- PWM Driver (Timer0)  
- PIR Sensor Driver  
- Buzzer Driver  
- DC Motor Driver  
- EEPROM Driver  

---

## ⚙️ System Flow
1. User sets a **5-digit password** (stored in EEPROM).  
2. User enters password → compared with stored password.  
3. ✅ If correct: Motor unlocks door, PIR sensor monitors entry, then locks door.  
4. ❌ If incorrect (3 times): System triggers **buzzer alarm** & **lockout** for 1 minute.  
5. User can change password after verification.  

---


---

## 📂 Repository Structure
