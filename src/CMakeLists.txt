

set(BUILD_GRAPHICAL_EXAMPLES FALSE CACHE INTERNAL "")
set(BUILD_UNIT_TESTS FALSE CACHE INTERNAL "")
set(BUILD_EXAMPLES FALSE CACHE INTERNAL "")

add_subdirectory(thirdparty/librealsense1)



find_package(OpenCV REQUIRED)
find_package(OpenNI2 REQUIRED)


add_executable(proximity-tool
	tool-main.cpp
	)


target_link_libraries(proximity-tool
	${OpenCV_LIBS}
#	realsense
    )

target_include_directories(
	proximity-tool PUBLIC
	${OpenCV_INCLUDE_DIRS}
#	thirdparty/librealsense1/include
    )


#message(STATUS "RealSense SDK: ${realsense_INCLUDE_DIR}")
