#include "../videostreamer.h"


bool VideoStreamer::is_writable_v4l2_device(const std::string& device_path)
{
    int fd = open(device_path.c_str(), O_WRONLY);
    if (fd == -1) {
        return false;
    }
    close(fd);
    return true;
}


std::string VideoStreamer::determine_v4l2sink_device()
{
    FILE *fp = popen("v4l2-ctl --list-devices", "r");
    if (!fp) {
        std::cerr << "Failed to run v4l2-ctl command." << std::endl;
        return "";
    }

    char buffer[256];
    std::string output;
    while (fgets(buffer, sizeof(buffer), fp) != nullptr)
    {
        output += buffer;
    }
    pclose(fp);

    std::size_t loopback_pos = output.find("Dummy video device");
    if (loopback_pos == std::string::npos) {
        return "";
    }

    std::size_t dev_pos = output.find("/dev/", loopback_pos);
    while (dev_pos != std::string::npos)
    {
        std::size_t dev_end_pos = output.find("\n", dev_pos);
        if (dev_end_pos == std::string::npos)
        {
            return "";
        }

        std::string potential_device = output.substr(dev_pos, dev_end_pos - dev_pos);
        if (is_writable_v4l2_device(potential_device))
        {
            return potential_device;
        }

        dev_pos = output.find("/dev/", dev_end_pos);
    }

}


std::string VideoStreamer::determine_pulseaudio_sink()
{
    FILE *fp = popen("pactl list sinks short", "r");
    if (!fp)
    {
        std::cerr << "Failed to run pactl command." << std::endl;
        return "";
    }

    char buffer[256];
    std::string output;
    while (fgets(buffer, sizeof(buffer), fp) != nullptr)
    {
        output += buffer;
    }
    pclose(fp);

    std::size_t loopback_pos = output.find("auto_null");
    if (loopback_pos == std::string::npos)
    {
        return "";
    }

    std::size_t id_end_pos = output.find("\t", loopback_pos);
    if (id_end_pos == std::string::npos)
    {
        return "";
    }

    return output.substr(0, id_end_pos);
}
