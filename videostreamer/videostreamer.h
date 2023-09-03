#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <gst/video/videooverlay.h>
#include <gst/gst.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>  // for GDK_WINDOW_XID
#endif
#ifdef GDK_WINDOWING_WIN32
#include <gdk/gdkwin32.h>  // for GDK_WINDOW_HWND
#endif

#include <iostream>
#include <string>
#include <csignal>
#include <cstdint>
#include <sstream>
#include <fcntl.h>



class VideoStreamer
{

private:
    bool output_to_virtual_device = false;
    std::string v4l2_device;
    std::string pulseaudio_device;
    GstElement *pipeline;
    GMainLoop *loop;

    GstElement* get_pipeline(const char *inputFile, const char *outputFile = nullptr);

    GstElement* build_to_screen_pipeline(const char *inputFile);
    GstElement* build_to_v4l2sink_pipeline(const char *inputFile);
    GstElement* build_to_autosink_pipeline(const char *inputFile);
    GstElement* build_to_file_pipeline(const char *inputFile, const char *outputFile);
    GstElement* build_transcoding_pipeline(const char *inputFile, const char *outputFile);

    static gboolean on_message(GstBus *bus, GstMessage *message, VideoStreamer *player);
    bool is_writable_v4l2_device(const std::string& device_path);

public:
    explicit VideoStreamer(bool output_to_vsink = false);
    ~VideoStreamer();

    void create_pipeline(const char *inputFile, const char *outputFile = nullptr);
    void play();
    void pause();
    void rewind(double position);
    void setVolume(double volume);

    int64_t getCurrentPosition();
    int64_t getDuration();

    std::string determine_v4l2sink_device();
    std::string determine_pulseaudio_sink();
    std::string determine_audio_device();

    std::string getV4L2Device() const { return v4l2_device; }
    std::string getPulseaudioDevice() const { return pulseaudio_device; }
};

#endif // VIDEOSTREAMER_H
