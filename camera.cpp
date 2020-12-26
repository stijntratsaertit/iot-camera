#include "camera.h"

using namespace std;

#include <string>
#include <iostream>
#include <fstream>

Camera::Camera()
{
    _videoLength = "5000";
}

void Camera::recordAndSave()
{
    std::time_t time = std::time(NULL);
    _currentFile << "vid-" << time;
    record();
    sleep(5);
    convertVideo();
    sleep(5);
    save();
}

void Camera::record()
{
    std::ostringstream command;
    std::cout << _currentFile.str() << "\n";
    command << "raspivid -o " << _currentFile.str() << ".h264 -t " << _videoLength;
    system(command.str().c_str());
}

void Camera::convertVideo()
{
    std::ostringstream command;
    command << "MP4Box -add " << _currentFile.str() << ".h264 " << _currentFile.str() << ".mp4";
    system(command.str().c_str());
}

void Camera::save()
{
    std::ostringstream command;
    command << "/usr/bin/python3.7 uploader.py " << _currentFile.str() << ".mp4 " << _currentFile.str() << ".h264";
    system(command.str().c_str());

    std::ostringstream deleteCommand;
    deleteCommand << "rm " << _currentFile.str() << ".mp4";
    system(deleteCommand.str().c_str());
}