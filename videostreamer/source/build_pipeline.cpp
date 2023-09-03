#include "../videostreamer.h"


GstElement* VideoStreamer::build_to_screen_pipeline(const char *inputFile)
{
    std::ostringstream pipeline_str;
    pipeline_str << "filesrc location="
                 << inputFile
                 << " ! decodebin ! autovideosink";

    return gst_parse_launch(pipeline_str.str().c_str(), nullptr);
}

GstElement* VideoStreamer::build_to_v4l2sink_pipeline(const char *inputFile)
{
    std::ostringstream pipeline_str;
    pipeline_str << "filesrc location=" << inputFile
                 << " ! decodebin "
                 << " ! videoconvert "
                 << " ! v4l2sink device=" << determine_v4l2sink_device();

    return gst_parse_launch(pipeline_str.str().c_str(), nullptr);
}

GstElement* VideoStreamer::build_transcoding_pipeline(const char *inputFile, const char *outputFile)
{
    std::ostringstream pipeline_str;
    pipeline_str << "filesrc location=" << inputFile
                 << " ! decodebin name=decoder "
                 << "decoder. ! audioconvert ! volume name=my_volume_element ! vorbisenc ! queue ! mux. "
                 << "decoder. ! videoconvert ! theoraenc ! queue ! mux. "
                 << "oggmux name=mux ! filesink location=" << outputFile;

    return gst_parse_launch(pipeline_str.str().c_str(), nullptr);
}



GstElement* VideoStreamer::get_pipeline(const char *inputFile, const char *outputFile)
{
    if (outputFile)
    {
        return build_transcoding_pipeline(inputFile, outputFile);
    } else if (output_to_virtual_device)
    {
        return build_to_v4l2sink_pipeline(inputFile);
    } else
    {
        return build_to_screen_pipeline(inputFile);
    }
}

void VideoStreamer::create_pipeline(const char *inputFile, const char *outputFile)
{
    if (pipeline)
    {
        gst_object_unref(pipeline);
    }
    pipeline = get_pipeline(inputFile, outputFile);
    if (!pipeline)
    {
        std::cerr << "Failed to create pipeline." << std::endl;
    }
}
