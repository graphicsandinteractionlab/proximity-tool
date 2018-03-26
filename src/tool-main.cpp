/**
  * Proximity Tool
  *
  * a small unix style tool to get the readings of a OpenNI2 enabled device send over OSC
  *
  * (c) Copyrights 2018 - Hartmut Seichter
  *
  */

#include "interface_rgbd.hpp"

#include "arrrgh.hpp"

#include <memory>
#include <vector>
#include <iostream>
#include <thread>

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"


#if defined(USE_OPENCV)
    // include OpenCV header file
    #include <opencv2/opencv.hpp>
#endif

inline static unsigned short
depthValueRaw(const void* ptr,int x, int y,int stride)
{
    const unsigned short* ptr_uint16 = reinterpret_cast<const unsigned short*>(ptr);
    return *(ptr_uint16 + (y * stride + x));
}

inline static float
depthValue(const void* ptr,int x, int y,int stride)
{
    return static_cast<float>(depthValueRaw(ptr,x,y,stride)) * 0.1f;
}


int main(int argc,char **argv) {

	arrrgh::parser parser("proximity-tool", "detects proximity");

	const auto& useAscii = parser.add< bool >("ascii",
		"Use ASCII instead of that UNICORN thing or whatever it is.",
		'a',
		arrrgh::Optional,
		true /* defaults to true */
		);


	parser.parse(argc, argv);

    bool running = true;

    std::string receiver = (argc > 1) ? argv[1] : "localhost";
    int port = (argc > 2) ? std::atoi(argv[2]) : 4455;

    //
    UdpTransmitSocket transmitSocket( IpEndpointName( receiver.c_str(), port ) );

    std::unique_ptr<RGBD> dev;

    dev.reset(RGBD::create(RGBD::kRGBDDeviceOpenNI2,0));

    if (dev == nullptr) {
        std::cerr << "cannot find device" << std::endl;
        return -1;
    }

    if (dev->open() != 0) {
        std::cerr << "cannot open device" << std::endl;
        return -1;
    }

    std::string RGB_WINDOW = "RGB";
    std::string DEPTH_WINDOW = "Depth";

    while(running) {

        //        dev->update();

        int w,h;

        const void *ptr = dev->getRGB(w,h);

        //        std::cout << w << "x" << h << std::endl;
#if defined (USE_OPENCV)
        if (ptr != nullptr) {


            // Create color image
            cv::Mat rgb( h,
                         w,
                         CV_8UC3,
                         (uchar*)ptr);

            // Display RGB and Depth Window
            cv::namedWindow(RGB_WINDOW, cv::WINDOW_AUTOSIZE );
            cv::imshow(RGB_WINDOW, rgb);

        }
#endif


        const void *depthPtr = dev->getDepth(w,h);

        //        std::cout << w << "x" << h << std::endl;

        if (depthPtr != nullptr) {


#if defined (USE_OPENCV)
            // Create color image
            cv::Mat depthRaw( h,
                              w,
                              CV_16UC1,
                              (uchar*)depthPtr);


            cv::Mat depthFloat;
            depthRaw.convertTo( depthFloat, CV_32FC1, 0.1);


            // Display RGB and Depth Window
            cv::namedWindow(DEPTH_WINDOW, cv::WINDOW_AUTOSIZE );
            cv::imshow(DEPTH_WINDOW, depthFloat);


            float d = depthFloat.at<float>(240,320);
            std::cout << "d  " << depthFloat.at<float>(240,320)
                      << " d2 " << depthValueRaw(depthPtr,320,240,w) << std::endl;
#endif
            float d = depthValue(depthPtr,320,240,w);

            std::cout << "depth " << d << std::endl;

            std::vector<char> buffer(1024);
            osc::OutboundPacketStream p( buffer.data(), buffer.size() );

            p << osc::BeginBundleImmediate
              << osc::BeginMessage( "/depth" )
              << d << osc::EndMessage
              << osc::EndBundle;

            transmitSocket.Send( p.Data(), p.Size() );

            // sleep for 10ms
            std::this_thread::sleep_for(std::chrono::duration<double,std::milli>(10.0));

        }


#if defined(USE_OPENCV)
        int c = cv::waitKey(1);

        switch (c) {
        case 27:
            running = false;
            break;

        }
#endif

    };


#if defined(USE_OPENCV)
    cv::destroyAllWindows();
#endif


    dev->close();

    return 0;
}
