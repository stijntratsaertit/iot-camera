echo "Build started"
g++ main.cpp gas-sensor.cpp camera.cpp movement-sensor.cpp light-sensor.cpp -o main -lwiringPi
echo "Build success"