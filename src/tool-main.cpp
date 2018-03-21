
#include "interface_rgbd.hpp"
// include OpenCV header file

#include <opencv2/opencv.hpp>

#include <memory>


int main() {

    bool running = true;

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


        const void *depthPtr = dev->getDepth(w,h);

//        std::cout << w << "x" << h << std::endl;

        if (ptr != nullptr) {


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

            std::cout << "d " << depthFloat.at<float>(240,320) << std::endl;

        }


        int c = cv::waitKey(1);

        switch (c) {
        case 27:
            running = false;
            break;

        }
    };

    cv::destroyAllWindows();

    dev->close();

    return 0;
}
