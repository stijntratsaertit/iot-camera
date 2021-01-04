#include "camera.h"

using namespace std;

Camera::Camera()
{
    changeState("0");
    _videoLength = "5000";
}

void Camera::recordAndSave()
{
    time_t time = std::time(NULL);
    _currentFile << "vid-" << time;
    record();
    sleep(5);
    convertVideo();
    sleep(5);
    save();
}

void Camera::record()
{
    changeState("1");
    ostringstream command;
    cout << _currentFile.str() << "\n";
    command << "raspivid -o " << _currentFile.str() << ".h264 -t " << _videoLength;
    system(command.str().c_str());
    changeState("0");
}

void Camera::convertVideo()
{
    ostringstream command;
    command << "MP4Box -add " << _currentFile.str() << ".h264 " << _currentFile.str() << ".mp4";
    system(command.str().c_str());
}

void Camera::save()
{
    ostringstream command;
    command << "/usr/bin/python3.7 uploader.py " << _currentFile.str() << ".mp4 " << _currentFile.str() << ".h264";
    system(command.str().c_str());

    ostringstream deleteCommand;
    deleteCommand << "rm " << _currentFile.str() << ".mp4";
    system(deleteCommand.str().c_str());
}

void Camera::changeState(string state)
{
    _stateFile.open("camera_recording.txt", ios::trunc);
    _stateFile << state;
    _stateFile.close();
}

int Camera::requestToRecord()
{
    int  isRequested;
    _requestFile.open("camera_request.txt", ios::in | ios::out);
    _requestFile >> isRequested;
    _requestFile.seekg(0);
    _requestFile << "0";
    _requestFile.close();
    return isRequested;
}