#pragma once

#include <wiringPi.h>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class Camera
{
public:
    Camera();
    void recordAndSave(string reason);
    bool requestToRecord();

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
