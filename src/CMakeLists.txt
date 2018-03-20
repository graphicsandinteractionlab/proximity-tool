
find_package(OpenCV REQUIRED)
find_package(realsense2 REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${realsense_INCLUDE_DIR}
	)


add_executable(proximity-tool
	tool-main.cpp
	)


target_link_libraries(proximity-tool
	${OpenCV_LIBS}
	${realsense2_LIBRARY}
	)


message(STATUS "RealSense SDK: ${realsense_INCLUDE_DIR}")
