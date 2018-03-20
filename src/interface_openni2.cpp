#include "interface_openni2.hpp"

#include <iostream>
#include <stdexcept>

struct CaptureThread {

    OpenNIDevice& _device;

    CaptureThread(OpenNIDevice& dev) : _device(dev) {
    }

    void operator()() {

        while (!_device.getWantsClose()) {
            _device.update();
        }
    }
};

OpenNIDevice::OpenNIDevice()
{
}

int OpenNIDevice::open() {

    using namespace openni;


    Status status = STATUS_OK;

    // Initialize OpenNI
    status = OpenNI::initialize();

    if (status != STATUS_OK) {

        std::cerr << "OpenNI: Initialize failed: '" << OpenNI::getExtendedError() << "'" << std::endl;

        OpenNI::shutdown();

        return 1;
    }

    std::cout << "Initialized ... " << std::endl;

    // Check if a camera is connected
    Array<openni::DeviceInfo> deviceList;
    OpenNI::enumerateDevices(&deviceList);
    int nr_devices = deviceList.getSize();

    if(nr_devices < 1) {
        std::cout << "OpenNI: No devices found" << std::endl;
        OpenNI::shutdown();
        return 1;
    }

    std::cout << "Devices " << nr_devices << std::endl;

    // Open device
    status = device.open(ANY_DEVICE);

    if (status != STATUS_OK) {
        std::cerr << "OpenNI: Could not open device: " << OpenNI::getExtendedError() << std::endl;
        OpenNI::shutdown();
        return 1;
    }

    std::cout << "device "
              << device.getDeviceInfo().getVendor()
              << " "
              << device.getDeviceInfo().getName()
              << " "
              << device.getDeviceInfo().getUri()
              << std::endl;

    // Create depth stream
    if (device.getSensorInfo(SENSOR_DEPTH) != NULL) {
        status = depth_stream.create(device, SENSOR_DEPTH);
        if (status != STATUS_OK) {

            std::cerr << "OpenNI: Could not create depth stream " <<  OpenNI::getExtendedError() << std::endl;

            OpenNI::shutdown();

            return 1;
        }
    }

    // Create color stream
    if (device.getSensorInfo(SENSOR_COLOR) != NULL) {
        status = color_stream.create(device, SENSOR_COLOR);
        if (status != STATUS_OK) {
            std::cerr << "OpenNI: Could not create color stream" << OpenNI::getExtendedError() << std::endl;
            OpenNI::shutdown();
            return 1;
        }
    }

    // Choose what depth format we want from the camera
    VideoMode depth_mode;
    depth_mode.setPixelFormat(PIXEL_FORMAT_DEPTH_100_UM);
    depth_mode.setResolution(640, 480);
    depth_mode.setFps(30);
    status = depth_stream.setVideoMode(depth_mode);
    if (status != STATUS_OK) {
        std::cerr << "OpenNI: Could not set depth video mode:" << OpenNI::getExtendedError() << std::endl;
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;

    // Choose what color format we want from the camera
    VideoMode color_mode;
    color_mode.setPixelFormat(PIXEL_FORMAT_RGB888);
    color_mode.setResolution(640, 480);
    color_mode.setFps(30);
    status = color_stream.setVideoMode(color_mode);
    if (status != STATUS_OK) {
        std::cerr  << "OpenNI: Could not set color video mode:" << OpenNI::getExtendedError() << std::endl;
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;

    // Enable registration mode
    status = device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);

    if (status != STATUS_OK) {
        printf("OpenNI: Could not enable registration mode:\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;

    // Enable color-to-depth synchronization
    status = device.setDepthColorSyncEnabled(true);

    if (status != STATUS_OK) {
        printf("OpenNI: Could not enable color sync:\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;


    // Disable depth mirroring (we want to see the perspective of the camera)
    status = depth_stream.setMirroringEnabled(false);

    if (status != STATUS_OK) {
        printf("OpenNI: Could enable mirroring on depth stream\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;

    // Disable color mirroring (we want to see the perspective of the camera)
    status = color_stream.setMirroringEnabled(false);

    if (status != STATUS_OK) {
        printf("OpenNI: Could enable mirroring on color stream\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    // Use allocator to have OpenNI write directly into our depth buffers
//    status = depth_stream.setFrameBuffersAllocator(depthAlloc);

    if (status != STATUS_OK) {
        printf("OpenNI: Could not set depth frame buffer allocator\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    // Use allocator to have OpenNI write directly into our color buffer
//    status = color_stream.setFrameBuffersAllocator(colorAlloc);

    if (status != STATUS_OK) {
        printf("OpenNI: Could not set color frame buffer allocator\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    // Start depth
    status = depth_stream.start();

    if (status != STATUS_OK) {
        std::cerr << "OpenNI: cannot start depth stream " << OpenNI::getExtendedError() << std::endl;
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;


    // Start color
    status = color_stream.start();

    if (status != STATUS_OK) {
        printf("OpenNI: Could not start the color stream\n%s\n", OpenNI::getExtendedError());
        OpenNI::shutdown();
        return 1;
    }

    std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;

    // create capture thread
    capture_thread = std::thread(CaptureThread(*this));

    return 0;
}


int OpenNIDevice::update() {

    openni::Status status = openni::STATUS_OK;

    // Next depth frame
    status = depth_stream.readFrame(&depthFrame);
    if (status != openni::STATUS_OK) {
        std::cerr << "OpenNI: readFrame (Depth) failed " << openni::OpenNI::getExtendedError() << std::endl;
    };

    status = color_stream.readFrame(&colorFrame);
    if (status != openni::STATUS_OK) {
        std::cerr << "OpenNI: readFrame (Color) failed " << openni::OpenNI::getExtendedError() << std::endl;
    }

    return 0;
}

void OpenNIDevice::close() {

    this->setWantsClose(true);

    capture_thread.join();



    depth_stream.destroy();
    color_stream.destroy();
    device.close();
//    openni::OpenNI::shutdown();
}

const void *OpenNIDevice::getRGB(int &w, int &h)
{
    if (colorFrame.isValid()) {
        w = colorFrame.getWidth();
        h = colorFrame.getHeight();

        return colorFrame.getData();
    }

    return nullptr;
}


const void *OpenNIDevice::getDepth(int &w, int &h)
{
    if (depthFrame.isValid()) {
        w = depthFrame.getWidth();
        h = depthFrame.getHeight();

        return depthFrame.getData();
    }

    return nullptr;
}
