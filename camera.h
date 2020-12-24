#pragma once

#include <wiringPi.h>
#include <string>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <CkHttp.h>

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
    CkHttp _http;
    const char* _bucketName;
    const char* _path;
    const char* _contentType;
};
