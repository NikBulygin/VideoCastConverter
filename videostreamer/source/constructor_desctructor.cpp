#include "../videostreamer.h"

VideoStreamer::VideoStreamer(bool output_to_vsink) : output_to_virtual_device(output_to_vsink)
{
    gst_init(nullptr, nullptr);
    pipeline = nullptr;
    loop = g_main_loop_new(nullptr, FALSE);
}

VideoStreamer::~VideoStreamer()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
    gst_deinit();
    if (loop) {
        g_main_loop_unref(loop);
    }
}



gboolean VideoStreamer::on_message(GstBus *bus, GstMessage *message, VideoStreamer *player)
{
    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_EOS:
        g_main_loop_quit(player->loop);
        break;
    case GST_MESSAGE_ERROR:
        gchar *debug;
        GError *err;

        gst_message_parse_error(message, &err, &debug);
        g_free(debug);

        std::cerr << "Error: " << err->message << std::endl;
        g_error_free(err);

        g_main_loop_quit(player->loop);
        break;
    default:
        break;
    }
    return true;
}
