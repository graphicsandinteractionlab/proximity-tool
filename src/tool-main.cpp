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


int main(int argc,const char **argv) {

    // some information about this tool
    arrrgh::parser parser("proximity-tool", "detects proximity using OpenNI2 devices - default osc output is to localhost:4455");

    // command line parameters
    const auto& receiver_address = parser.add< std::string >( "receiver", "IP or hostname of receiver", 'r', arrrgh::Optional, "localhost" );
    const auto& receiver_port = parser.add< int >( "port", "port number", 'p', arrrgh::Optional, 4455 );
    const auto& need_help = parser.add< bool >( "help", "print command line arguments", 'h', arrrgh::Optional, false );
    const auto& print_verbose = parser.add< bool >( "verbose", "verbose debug output", 'v', arrrgh::Optional, false );
    const auto& idle_sleep = parser.add< float >( "idle", "idle time in ms", 'i', arrrgh::Optional, 10.0f );
//    const auto& grid_size = parser.add< int >( "grid", "grid size", 'g', arrrgh::Optional, 9 );


    // try to parse
    try {

        parser.parse(argc, argv);

    } catch (const std::exception &e) {

        std::cerr << "Error parsing arguments: " << e.what() << std::endl;
        parser.show_usage( std::cerr );

        return 1;
    }

    // show help and quit
    if (need_help.value()) {

        parser.show_usage(std::cout);
        return 0;
    }


    bool running = true;

    //
    UdpTransmitSocket transmitSocket( IpEndpointName( receiver_address.value().c_str(), receiver_port.value() ) );

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

    std::vector<float> depthValues(9);

    while(running) {

        //        dev->update();

        int w,h;

//        const void *ptr = dev->getRGB(w,h);
        const void *depthPtr = dev->getDepth(w,h);

        if (depthPtr != nullptr) {


            if (depthValues.size() == 9) {

                depthValues[0] = depthValue(depthPtr,160,120,w);
                depthValues[1] = depthValue(depthPtr,320,120,w);
                depthValues[2] = depthValue(depthPtr,480,120,w);

                depthValues[3] = depthValue(depthPtr,160,240,w);
                depthValues[4] = depthValue(depthPtr,320,240,w);
                depthValues[5] = depthValue(depthPtr,480,240,w);

                depthValues[6] = depthValue(depthPtr,160,360,w);
                depthValues[7] = depthValue(depthPtr,320,360,w);
                depthValues[8] = depthValue(depthPtr,480,360,w);

            } else {

                depthValues[0] = depthValue(depthPtr,320,240,w);

            }

            if (print_verbose.value()) {
                std::cout << "depth ";
                for (auto v : depthValues) {
                    std::cout << v;
                }
                std::cout << std::endl;
            }

            std::vector<char> buffer(1024);
            osc::OutboundPacketStream p( buffer.data(), buffer.size() );

            p << osc::BeginBundleImmediate
              << osc::BeginMessage( "/depth" );

            for (auto v : depthValues) {
                p << v;
            }

            p << osc::EndMessage
              << osc::EndBundle;

            transmitSocket.Send( p.Data(), p.Size() );

            // sleep for 10ms
            std::this_thread::sleep_for(std::chrono::duration<float,std::milli>(idle_sleep.value()));

        }
    };

    dev->close();

    return 0;
}
