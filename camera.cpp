#include "camera.h"

using namespace std;

Camera::Camera()
{
    _http.put_AwsAccessKey(std::getenv("AWSAK"));
    _http.put_AwsSecretKey(std::getenv("AWSSK"));
    _bucketName = "camera-footage";
    _path = "/";
    _contentType = "mp4";
}

void Camera::recordAndSave()
{
    std::time_t time = std::time(NULL);
    _currentFile << "vid-" << time;
    record();
    sleep(30);
    convertVideo();
    sleep(10);
    save();
}

void Camera::record()
{
    std::ostringstream command;
    std::cout << _currentFile.str() << "\n";
    command << "raspivid -o " << _currentFile.str() << ".h264 -t 60000";
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
    bool success = _http.S3_UploadFile(_path, _contentType, _bucketName, _currentFile.str().c_str());

    if (success != true)
    {
        std::cout << _http.lastErrorText() << "\r\n";
    }
    else
    {
        std::cout << "File uploaded."
                  << "\r\n";
    }
}