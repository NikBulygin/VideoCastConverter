#include <QApplication>

#include <gst/gst.h>
#include <signal.h>

#include "videostreamer/videostreamer.h"
#include "videoplayer/videoplayer.h"


void showHelp(char *appName)
{
    g_print("Usage:\n");
    g_print("  %s <input_file> [output_file]          Play video file. If output_file is provided, stream to it.\n", appName);
    g_print("  %s -g|--gui <input_file>               Start GUI video player with input file.\n", appName);
    g_print("  %s -h|--help                          Show this help message.\n", appName);
}

int handleGUIOption(int argc, char** argv)
{
    QApplication app(argc, argv);
    gst_init (&argc, &argv);
    VideoStreamer streamer(true);
    VideoPlayer videoplayer(argv[2], &streamer);
    videoplayer.show();
    return app.exec();
}

int handleCommandLine(int argc, char **argv)
{
    gst_init (&argc, &argv);
    VideoStreamer streamer;
    if (argc == 2) {
        streamer.create_pipeline(argv[1]);
    } else if (argc == 3)
    {
        streamer.create_pipeline(argv[1], argv[2]);
    } else
    {
        g_printerr("Usage: %s <input_file> [output_file]\n", argv[0]);
        showHelp(argv[0]);
        return -1;
    }
    streamer.play();
    return 0;
}


int main(int argc, char *argv[])
{
    if (argc > 1 && (strcmp(argv[1], "-h") == 0 ||
                     strcmp(argv[1], "--help") == 0))
    {
        showHelp(argv[0]);
        return 0;
    }

    if (argc > 1 && (strcmp(argv[1], "-g") == 0 ||
                     strcmp(argv[1], "--gui") == 0))
    {
        if (argc != 3) {
            g_printerr("Usage when using GUI option: %s -g|--gui <input_file>\n", argv[0]);
            return -1;
        }
        return handleGUIOption(argc, argv);
    } else
    {
        return handleCommandLine(argc, argv);
    }
}


