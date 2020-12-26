#pragma once

#include <wiringPi.h>
#include <string>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sstream>

using namespace std;

class Camera
{
public:
    Camera();
    void recordAndSave();

private:
    void record();
    void convertVideo();
    void save();

    std::ostringstream _currentFile;
    std::string _videoLength;
};
