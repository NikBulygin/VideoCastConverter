#include "../videostreamer.h"



void VideoStreamer::play()
{
    if (!pipeline) return;

    GstBus *bus = gst_element_get_bus(pipeline);
    gst_bus_add_watch(bus, (GstBusFunc)on_message, this);
    gst_object_unref(bus);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_main_loop_run(loop);
}

void VideoStreamer::pause()
{
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
    }
}

void VideoStreamer::rewind(double position)
{
    if (!pipeline) {
        return;
    }

    gint64 duration;
    if (gst_element_query_duration(pipeline, GST_FORMAT_TIME, &duration))
    {
        gint64 seek_position = position * duration;
        gst_element_seek(pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, seek_position,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
    }
}

void VideoStreamer::setVolume(double volume)
{
    GstElement *volumeElement = gst_bin_get_by_name(GST_BIN(pipeline), "my_volume_element");
    if (volumeElement) {
        g_object_set(volumeElement, "volume", volume, NULL);
        gst_object_unref(volumeElement);
    }
}

int64_t VideoStreamer::getCurrentPosition()
{
    if (pipeline) {
        gint64 position = -1;
        gst_element_query_position(pipeline, GST_FORMAT_TIME, &position);
        return position / GST_MSECOND;  // nanosec to millisec
    }
    return 0;
}

int64_t VideoStreamer::getDuration()
{
    if (pipeline) {
        gint64 duration = -1;
        gst_element_query_duration(pipeline, GST_FORMAT_TIME, &duration);
        return duration / GST_MSECOND;  // nanosec to millisec
    }
    return 0;
}

