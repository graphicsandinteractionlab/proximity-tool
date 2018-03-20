

set(BUILD_GRAPHICAL_EXAMPLES FALSE CACHE INTERNAL "")
set(BUILD_UNIT_TESTS FALSE CACHE INTERNAL "")
set(BUILD_EXAMPLES FALSE CACHE INTERNAL "")

add_subdirectory(thirdparty/librealsense1)



find_package(OpenCV REQUIRED)
find_package(OpenNI2)

set(interface_rgbd_libs)
set(interface_rgbd_incs)
set(interface_rgbd_defs)
set(interface_rgbd_srcs)

option(INTERFACE_RGBD_ENABLE_OPENNI2 "Enable Kinect/OpenNI2 Interface" OFF)
if (OPENNI2_FOUND AND INTERFACE_RGBD_ENABLE_OPENNI2)

	list(APPEND interface_rgbd_libs ${OPENNI2_LIBRARY})
	list(APPEND interface_rgbd_incs ${OPENNI2_INCLUDES})
	list(APPEND interface_rgbd_srcs interface_openni2.hpp interface_openni2.cpp)

	set(INTERFACE_RGBD_HAVE_OPENNI2 TRUE)
endif()


configure_file(interface_config.hpp.cmake
	${CMAKE_BINARY_DIR}/include/interface_config.hpp
	)

add_executable(proximity-tool
	${CMAKE_BINARY_DIR}/include/interface_config.hpp
	tool-main.cpp
	interface_rgbd.hpp
	interface_rgbd.cpp
	interface_config.hpp.cmake
	${interface_rgbd_srcs}
	)

message(STATUS "OpenNI2 ${OPENNI2_FOUND}")

target_link_libraries(proximity-tool
	${OpenCV_LIBS}
	${interface_rgbd_libs}
	)

target_include_directories(
	proximity-tool PUBLIC
	${CMAKE_BINARY_DIR}/include
	${OpenCV_INCLUDE_DIRS}
	${interface_rgbd_incs}
#	thirdparty/librealsense1/include
    )


#message(STATUS "RealSense SDK: ${realsense_INCLUDE_DIR}")
