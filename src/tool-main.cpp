// include the librealsense C++ header file
//#include <librealsense2/rs.hpp>

//#include <librealsense/rs.hpp>

#include "interface_rgbd.hpp"



// include OpenCV header file
#include <opencv2/opencv.hpp>


int main() {




    bool running = true;

    while(running) {

        rs::intrinsics   _depth_intrin;
        rs::intrinsics  _color_intrin;

        _depth_intrin       = camera->get_stream_intrinsics( rs::stream::depth );
        _color_intrin       = camera->get_stream_intrinsics( rs::stream::color );


        // Create depth image
        cv::Mat depth16( _depth_intrin.height,
                         _depth_intrin.width,
                         CV_16U,
                         (uchar *)camera->get_frame_data( rs::stream::depth ) );

        // Create color image
        cv::Mat rgb( _color_intrin.height,
                     _color_intrin.width,
                     CV_8UC3,
                     (uchar *)camera->get_frame_data( rs::stream::color ) );

        // < 800
        cv::Mat depth8u = depth16;
        depth8u.convertTo( depth8u, CV_8UC1, 255.0/1000 );

        // Display RGB and Depth Window
        cv::namedWindow(RGB_WINDOW, cv::WINDOW_AUTOSIZE );
        cv::imshow(RGB_WINDOW, rgb);


        cv::namedWindow(DEPTH_WINDOW, cv::WINDOW_AUTOSIZE );
        cv::imshow(DEPTH_WINDOW, depth8u);

        int c = cv::waitKey(1);

        switch (c) {
        case 27:
            running = false;
            break;

        }

        if( camera->is_streaming( ) ) {
            camera->wait_for_frames();
        }
    };

    if (camera) {
        camera->stop();
    }

    cv::destroyAllWindows();


    return 0;
}
