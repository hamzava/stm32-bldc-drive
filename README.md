# ⚡ Sensorless 3-Phase BLDC Motor Driver  
### Using STM32F411RE + IR2110 Gate Drivers + IRF3205

> 📌 **Recommended Reading:**  
> Please refer to `/Docs/BLDC Project Report.pdf` first for a detailed explanation of the architecture, technical challenges encountered, and lessons learned during development.  
> This repository documents a prototype exploration and does not represent a finalized, fully validated motor drive system.

◆ This project represents an exploration into low-level complementary PWM control, high-side bootstrap driving, and discrete 3-phase inverter design using STM32 advanced timers.

---

## ◆ Project Overview

🟥 This project is a  3-phase BLDC motor driver prototype built using:

 - **STM32F411RE** (Advanced Timer – Complementary PWM)
 - **IR2110 High/Low Side Gate Drivers**
 - IRF3205 N-Channel MOSFET Half-Bridges
 - Bootstrap High-Side Supply Topology

🟥 The objective was to implement manual commutation for a sensorless BLDC motor using complementary PWM outputs from the MCU’s advanced timer (TIM1), with register-level control and control the motor using it's BEMF.

This prototype focused on:

✔ BLDC Motor Control Algorithm
✔ Complementary PWM generation  
✔ Gate driver bootstrap behavior  
✔ Register-level commutation control  
✔ Hardware–firmware synchronization  

---
## ◆ Repository Structure
/Hardware
- ├── KiCad_Project
- ├── Gerbers
- ├── Schematics_PDF
- ├── PCB_Images

/Firmware

/Videos

/Docs
- ├── Datasheets
- ├── Resources_Used > ***All the files used to learn about technologies involved in this project***
- ├── BLDC Project Report.pdf

---

## ◆ Status

Prototype exploration complete.  
Further revision planned with improved instrumentation and driver selection.
