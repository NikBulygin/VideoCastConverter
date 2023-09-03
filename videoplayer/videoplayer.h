#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QWidget>
#include <QVideoWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QSlider>
#include <QShowEvent>
#include <QTimer>
#include <QMediaPlayer>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaDevices>
#include <QCameraDevice>

#include "../videostreamer/videostreamer.h"

class VideoPlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit VideoPlayer(
        const char *inputFile ,
        VideoStreamer* streamer = nullptr,
        QWidget *parent = nullptr);

    void start_video()
    {
        streamer->play();
    }

private:
    VideoStreamer* streamer;
    QVideoWidget* video_widget;
    QCamera* camera = nullptr;
    QMediaCaptureSession* captureSession = nullptr;
    QVideoWidget* videoWidget = nullptr;
    QMediaPlayer* player = nullptr;


    const char *input_file;

    QWidget* control_menu;
    QPushButton* pause_button;
    QSlider* slider_video;
    QSlider* slider_audio_volume;

    bool isPlaying = true;

    void setUI();
    QCameraDevice set_camera_input_device();

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void onPlayPauseButtonClicked();
    void onVideoSliderMoved(int position);
    void onVolumeSliderMoved(int volume);
    void updateVideoPosition();


};


#endif // VIDEOPLAYER_H
