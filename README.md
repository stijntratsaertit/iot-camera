### RPI
This is the C++ Raspberry Pi code for group 17.

### Requirements
You will need the following:
    - C++ compiler
    - wiringPi library (```bash sudo apt-get install wiringpi```)
    - Python3

### Run
To run the program:
    - Install python dependencies with ```sudo pip3 install -r requirements.txt```
    - Compile the program by running ```g++ main.cpp camera.cpp movement-sensor.cpp light-sensor.cpp -o main -lwiringPi```
    - Make sure it has executable rights and run it with ```sudo ./main```