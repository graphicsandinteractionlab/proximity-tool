// include the librealsense C++ header file
//#include <librealsense2/rs.hpp>

#include <librealsense/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>

#include <exception>

//// Convert rs2::frame to cv::Mat
//cv::Mat frame_to_mat(const rs2::frame& f)
//{
//    using namespace cv;
//    using namespace rs2;

//    auto vf = f.as<video_frame>();
//    const int w = vf.get_width();
//    const int h = vf.get_height();

//    if (f.get_profile().format() == RS2_FORMAT_BGR8)
//    {
//        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
//    }
//    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
//    {
//        auto r = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
//        cv::cvtColor(r, r, CV_BGR2RGB);
//        return r;
//    }
//    else if (f.get_profile().format() == RS2_FORMAT_Z16)
//    {
//        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
//    }
//    else if (f.get_profile().format() == RS2_FORMAT_Y8)
//    {
//        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);;
//    }

//    throw std::runtime_error("Frame format is not supported yet!");
//}

//// Converts depth frame to a matrix of doubles with distances in meters
//cv::Mat depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f)
//{
//    using namespace cv;
//    using namespace rs2;

//    cv::Mat dm = frame_to_mat(f);
//    dm.convertTo(dm, CV_64F);
//    auto depth_scale = pipe.get_active_profile()
//        .get_device()
//        .first<depth_sensor>()
//        .get_depth_scale();
//    dm = dm * depth_scale;
//    return dm;
//}

int main() {

    rs::context context;

    rs::device *camera = context.get_device(0);

    if (camera == nullptr) {
        std::cerr << "no camera" << std::endl;
    }

    // Window size and frame rate
    int const INPUT_WIDTH      = 640;
    int const INPUT_HEIGHT     = 480;
    int const FRAMERATE        = 30;

    const std::string RGB_WINDOW = "RGB";
    const std::string DEPTH_WINDOW = "Depth";


    camera->enable_stream( rs::stream::color, INPUT_WIDTH, INPUT_HEIGHT, rs::format::rgb8, FRAMERATE );
    camera->enable_stream( rs::stream::depth, INPUT_WIDTH, INPUT_HEIGHT, rs::format::z16, FRAMERATE );
    camera->start( );

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
