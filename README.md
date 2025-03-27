# 4-Person Buzzer Game  

## Description  

This project includes the hardware design files and source code needed to build a four-person buzzer game from scratch.  

It was created as a volunteer activity associated with [The Church of Jesus Christ of Latter-day Saints](https://churchofjesuschrist.org "Church of Jesus Christ") to teach teenagers how to solder, 3D print, and program in C++.  

You are welcome to use and learn from this project under the **MIT License**.  

---

## How to Play  

1. **Connect the handles** using the RJ12 ports.  
2. **Power on the motherboard** by connecting it to a USB power supply.  
3. The game will play a **startup song**.  
4. The **slide switch toggles sound muting**.  
5. The game starts in the **reset state**—all handle LEDs should be **off**.  
6. **Ask a question.**  
7. Press the **motherboard button** to enable responses—handles will light up the **left LED**.  
8. Players **press and hold a handle button** to respond.  
   - The motherboard will play a sound.  
   - If two players press at the same time, the motherboard applies a **random tiebreaker**.  
   - Players can press the button **before** responses are enabled.  
9. The winning handle will light up the **right LED**.  
10. The player states their **answer**.  
11. **Judge and score manually**.  
12. Press the **motherboard button** to reset and go back to step **6**.  

---

## Code  

### Diagnostics  

Short test scripts for **buttons and LEDs**—useful for teaching coding basics.  

### Game  

The **Arduino project** implementing the game using the **FreeRTOS framework**.  

- Customize the `song_XXXXX` files to add **custom buzzer sounds**.  
- The main file for **Arduino compilation** is `game.ino`.  

---

## 3D Printed Components  

Designed in **FreeCAD**. `.stl` files for 3D printing are available in the corresponding component folders.  

### Case  

A **3D-printable enclosure** for the motherboard.  

### Handle  

A **3D-printable enclosure** for the four handles, each equipped with a button and LEDs. The handles connect to the motherboard via **RJ12 phone cables**.  

---

## PCB Design  

Designed in **KiCad**.  

- The **exports folder** contains:  
  - **Bill of materials (BOM)**  
  - **Gerber and drill files** for PCB manufacturing  
- Also includes:  
  - A `.step` model and **schematic printout** for circuit reference  
- **Note**: This repository does **not** include the full KiCad parts library required for board file recreation.  

---

## Cost Estimate  

At the time of publication:  

- **PCB and component costs**: $75 - $125 USD per board (varies based on order size; e.g., **Osh Park requires a minimum order of 3 PCBs**).  
- **3D printing material**: ~300g of **PLA** per system.  
- **Assembly time**:  
  - **Skilled labor**: ~2 hours  
  - **Beginners**: ~10 hours  

