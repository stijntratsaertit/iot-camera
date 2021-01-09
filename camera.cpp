#include "camera.h"

using namespace std;

Camera::Camera()
{
    changeState("0");
    _videoLength = "5000";
    _ledPin = 6;
    pinMode(_ledPin, OUTPUT);
}

void Camera::recordAndSave(string reason)
{
    changeState("1");
    time_t time = std::time(NULL);
    _currentFile << "vid-" << time << "-" << reason;
    record();
    sleep(5);
    convertVideo();
    sleep(5);
    save();
    _currentFile.str("");
    _currentFile.clear();
    changeState("0");
}

void Camera::record()
{
    ostringstream command;
    cout << _currentFile.str() << "\n";
    command << "raspivid -o /home/stijn/iot/" << _currentFile.str() << ".h264 -t " << _videoLength;
    system(command.str().c_str());
}

void Camera::convertVideo()
{
    ostringstream command;
    command << "MP4Box -add /home/stijn/iot/" << _currentFile.str() << ".h264 /home/stijn/iot/" << _currentFile.str() << ".mp4";
    system(command.str().c_str());
}

void Camera::save()
{
    ostringstream command;
    command << "/usr/bin/python3.7 /home/stijn/iot/uploader.py " << _currentFile.str() << ".mp4";
    system(command.str().c_str());

    ostringstream deleteCommand;
    deleteCommand << "rm /home/stijn/iot/" << _currentFile.str() << ".mp4 /home/stijn/iot/" << _currentFile.str() << ".h264";
    system(deleteCommand.str().c_str());
}

void Camera::changeState(string state)
{
    ledEnabled(state == "0" ? false : true);
    _stateFile.open("/home/stijn/iot/camera_recording.txt", ios::trunc);
    _stateFile << state;
    _stateFile.close();
}

int Camera::requestToRecord()
{
    string isRequested = "";
    _requestFile.open("/home/stijn/iot/camera_request.txt", ios::in | ios::out);
    _requestFile.seekg(0);
    getline(_requestFile, isRequested);
    _requestFile.seekg(0);
    _requestFile << "0";
    _requestFile.close();
    return isRequested.compare("1");
}

void Camera::ledEnabled(bool value)
{
    digitalWrite(_ledPin, value ? HIGH : LOW);
}