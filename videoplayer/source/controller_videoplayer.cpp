#include "../videoplayer.h"


void VideoPlayer::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

    streamer->create_pipeline(input_file);

    camera->setCameraDevice(set_camera_input_device());

    captureSession->setVideoOutput(video_widget);
    captureSession->setCamera(camera);


    camera->start();
    onPlayPauseButtonClicked();
}



QCameraDevice VideoPlayer::set_camera_input_device()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    QCameraDevice selectedCamera;

    for (const QCameraDevice &camera : cameras)
    {
        if (camera.id() == QByteArray::fromStdString(streamer->getV4L2Device()))
        {
            selectedCamera = camera;
            break;
        }
    }

    return selectedCamera;
}


void VideoPlayer::onPlayPauseButtonClicked()
{
    if (isPlaying) {
        streamer->pause();
        pause_button->setText("Play");
        isPlaying = false;
    } else {
        streamer->play();
        pause_button->setText("Pause");
        isPlaying = true;
    }
}

void VideoPlayer::onVideoSliderMoved(int position)
{
    double ratio = static_cast<double>(position) / static_cast<double>(slider_video->maximum());
    streamer->rewind(ratio);
}

void VideoPlayer::onVolumeSliderMoved(int volume)
{
    double volumeRatio = static_cast<double>(volume) / static_cast<double>(slider_audio_volume->maximum());
    streamer->setVolume(volumeRatio);
}

void VideoPlayer::updateVideoPosition()
{
    int64_t currentPosition = streamer->getCurrentPosition();
    int64_t duration = streamer->getDuration();
    if(duration)
        slider_video->setValue(static_cast<int>(currentPosition * slider_video->maximum() / duration));
    else
        slider_video->setValue(0);
}
