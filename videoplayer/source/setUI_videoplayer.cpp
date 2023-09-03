#include "../videoplayer.h"



void VideoPlayer::setUI()
{
    QGridLayout* layout = new QGridLayout();

    video_widget->setStyleSheet("background-color: red  ;");


    layout->addWidget(video_widget,
                      0, 0,
                      95, 100);

    layout->addWidget(pause_button,
                      95, 0,
                      5, 5);

    layout->addWidget(slider_video,
                      95, 5,
                      5, 90);

    layout->addWidget(slider_audio_volume,
                      95, 95,
                      5, 5);

    control_menu->setLayout(layout);


    this->setCentralWidget(control_menu);


}
