#pragma once

#include <wiringPi.h>
#include <string>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

class Camera
{
public:
    Camera();
    void recordAndSave();
    int requestToRecord();

private:
    void record();
    void convertVideo();
    void save();
    void changeState(string state);
    void ledEnabled(bool value);

    ostringstream _currentFile;
    string _videoLength;
    ofstream _stateFile;
    fstream _requestFile;
    int _ledPin;
};
