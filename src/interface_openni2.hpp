/*
 * Interface RGBD - (c) 2018 Hartmut Seichter
 *
 */
#ifndef INTERFACE_RGBD_OPENNI2
#define INTERFACE_RGBD_OPENNI2

#include "interface_rgbd.hpp"

#include <OpenNI.h>
#include <thread>

class OpenNIDevice : public RGBD {

    openni::Device device;
    openni::VideoStream depth_stream;
    openni::VideoStream color_stream;

    // RGB frame
    openni::VideoFrameRef colorFrame;
    openni::VideoFrameRef depthFrame;

	std::thread capture_thread;

    // RGBD interface
public:

    OpenNIDevice();

    int open();
    bool available() const { return true; }
    int update();
    void close();

    virtual ~OpenNIDevice() {}

    // RGBD interface
public:
    const void *getRGB(int &w, int &h);
    const void *getDepth(int &w, int &h);
};


#endif
