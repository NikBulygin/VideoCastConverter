#include "../videoplayer.h"

VideoPlayer::VideoPlayer(
    const char *inputFile ,
    VideoStreamer* streamer,
    QWidget *parent) :
    input_file(inputFile),
    streamer(streamer),
    QMainWindow(parent),
    video_widget(new QVideoWidget(this)),
    camera(new QCamera(this)),
    captureSession(new QMediaCaptureSession(this)),
    player(new QMediaPlayer(this)),
    control_menu(new QWidget(this)),
    pause_button(new QPushButton(control_menu)),
    slider_video(new QSlider(Qt::Horizontal, control_menu)),
    slider_audio_volume(new QSlider (Qt::Horizontal, control_menu))

{
    if(!this->streamer)
        this->streamer = new VideoStreamer();

    setUI();

    connect(pause_button, &QPushButton::clicked, this, &VideoPlayer::onPlayPauseButtonClicked);
    connect(slider_video, &QSlider::sliderMoved, this, &VideoPlayer::onVideoSliderMoved);
    connect(slider_audio_volume, &QSlider::valueChanged, this, &VideoPlayer::onVolumeSliderMoved);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &VideoPlayer::updateVideoPosition);
    timer->start(1000);

}
