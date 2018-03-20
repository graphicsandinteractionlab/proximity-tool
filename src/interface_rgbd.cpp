
#include "interface_rgbd.hpp"

#include "interface_config.hpp"


#if defined(KFUSION_INTERFACE_HAVE_FREENECT)
    #include "interface_kinect.hpp"
#endif

#if defined(INTERFACE_RGBD_HAVE_OPENNI2)
    #include "interface_openni2.hpp"
#endif

#if defined(KFUSION_INTERFACE_HAVE_LIBREALSENSE)
    #include "interface_librealsense.hpp"
#endif

#if defined(KFUSION_INTERFACE_HAVE_MSKINECT1)
	#include "interface_mskinect.hpp"
#endif

long RGBD::getNumFrames() const
{
    return _numFrames;
}

bool RGBD::getWantsClose() const
{
    return _wantsClose;
}

void RGBD::setWantsClose(bool wantsClose)
{
    _wantsClose = wantsClose;
}

RGBD *RGBD::create(RGBD::RGBDDevice device, const char *flags) {

    switch (device) {
#if defined(KFUSION_INTERFACE_HAVE_FREENECT)
    case RGBD::kRGBDDeviceKinect:
        return new KinectDevice();
        break;
#endif

#if defined(INTERFACE_RGBD_HAVE_OPENNI2)
    case RGBD::kRGBDDeviceOpenNI2:
        return new OpenNIDevice();
        break;
#endif

#if defined(KFUSION_INTERFACE_HAVE_LIBREALSENSE)
    case RGBD::kRGBDRealSense:
        return new RealSenseDevice();
        break;
#endif

#if defined(KFUSION_INTERFACE_HAVE_MSKINECT1)
	case RGBD::kRGBDMSKinect1:
		return new MSKinectDevice1();
		break;
#endif

    default:
        break;
    }

    return 0L;
}
