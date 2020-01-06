# Code Interpreter to remotely control a flight simulator
Created as a university project in Advanced Programming.

Link to Github - https://github.com/NoamBarak/flight_simulator - Daniela added as contributor.

### Introduction
In this project we are creating a program that will remotely help 
the plane in the flight simulator called flight-gear make a
 "hands off" takeoff.
  
The program connects to the flight-gear remote server and 
fetches data from it, then interprets the data 
to determine the right actions necessary
for a successful takeoff. Followed by implementing these actions 
in the simulator itself by creating a server that sends 
actions for the plane to perform.  
The program will accept a .txt file with instructions and 
commands to create a takeoff with specific specs.

-------------

### Usage
Having a successful takeoff without having to manually man
the plane's controls.  

![](Flying.gif)

-------------

### Launch

In order to launch the program follow the next steps:
1) Download and install [flight-gear](https://www.flightgear.org/download/main-program/).
2) In flight-gear additional settings add the following: 
````
 --telnet=socket,in,10,127.0.0.1,PORT1,tcp --httpd=8080
 --generic=socket,out,10,127.0.0.1,PORT2,tcp,generic_small
 ````
 
 3) Plant the file 'generic_small.xml' in `flightgear/Protocol`.
 4) Start the program with a .txt file containing the commands.
 5) Open flight-gear and click `Fly!`.
 
 -------------

### Project status
Currently working with .txt files containing the commands and specs for takeoff.   
Awaiting next assingment for more features  :wink:.	
