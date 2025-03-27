# Descirption

This project contains the hardware design files and source code to create a 4 person buzzer game from scratch.
This was created as a volunteer activity associated with the Church of Jesus Christ of Latter Day Saints to teach teenagers how to solder, 3D print, and code in C++.
Feel free to use and learn from it according the MIT License.

# How to play:

1) Plug in the handles' using the RJ12 ports.
2) Plug in the mother board to USB power supply.
3) The game will play a startup song.
4) The slide switch will toggle muting the sound.
5) The game will start in the reset state.  All LEDS on the handles should be off.
6) Ask a question.
7) Push the motherboard button to enable responses.  The handles should light up the left LED.
8) After responses are enabled, press and hold a handle button to key in a response.  The motherboard will play a sound.  In the case of a tie, a random tie breaker is applied by the motherboard.  The handle button can be pushed before the response is enabled.
9) The handle the got the reponse will light up the right LED.
10) State the answer.
11) Judge and score manually.
12) Press the mother board button to clear the response and go back to 6).

# Code 

## Diagnostics
Some short pieces of code for trying out the buttons and LEDs.  This is good for teaching others how to code.

## Game
The arduino project that implements the game using the FreeRTOS framework in Arduino.
Customize the song_XXXXX files to play your own custom music on the buzzer.
The "game.ino" is main file used by Arduino to compile the project.

# Freecad
All the 3D printable parts were designed in FreeCAD.  The .stl files are in the folders for each component.
## Case 
The container for the mother board to be 3D printed.
## Handle 
The container for the 4 handles with a button and LEDs connected to the mother board by RJ12 phone cable.

# KiCAD:
The design source files for the motherboard for the buzzer system.  
The exports folder contains the bill of materials, gerbers, and drill files to make the board. 
This directory also contains a .step and schematic printout required to make the circuits.  
This distribution does not ship the associated KiCad parts library to fully recreate the board files.

# Cost Estimate
At the time of publication, the cost to purchase components and manufacture 1 board was approximately 75 - 125 $USD depending on the minimum quantity cost for the PCB (e.g Osh Park requires a minimum order of 3 PCBs).  
There is economy of scale in shipping for parts and PCBs on larger orders.
The printing used about 300 grams of PLA for one system.
Soldering and assembly takes about 2 hours for sklled labor and about 10 hours for new beginners.

