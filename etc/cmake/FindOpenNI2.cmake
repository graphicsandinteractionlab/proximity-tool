if(WIN32)
    if(NOT (MSVC64 OR MINGW64))
        find_file(OPENNI2_INCLUDES "OpenNI.h" PATHS "$ENV{OPEN_NI_INSTALL_PATH}/Include" DOC "OpenNI2 c++ interface header")
        find_library(OPENNI2_LIBRARY "OpenNI2" PATHS $ENV{OPENNI2_LIB} DOC "OpenNI2 library")
    else()
        find_file(OPENNI2_INCLUDES "OpenNI.h" PATHS $ENV{OPENNI2_INCLUDE64} "$ENV{OPEN_NI_INSTALL_PATH64}Include" DOC "OpenNI2 c++ interface header")
        find_library(OPENNI2_LIBRARY "OpenNI2" PATHS $ENV{OPENNI2_LIB64} DOC "OpenNI2 library")
    endif()
elseif(UNIX OR APPLE)
	find_file(OPENNI2_INCLUDES
		"OpenNI.h" PATHS "/usr/include/ni2" "/usr/include/openni2" $ENV{OPENNI2_INCLUDE}
		DOC "OpenNI2 c++ interface header"
		)
    find_library(OPENNI2_LIBRARY "OpenNI2" PATHS "/usr/lib" $ENV{OPENNI2_REDIST} DOC "OpenNI2 library")
endif()

if(OPENNI2_LIBRARY AND OPENNI2_INCLUDES)
    set(OPENNI2_FOUND TRUE)
endif()

mark_as_advanced(FORCE OPENNI2_LIBRARY)
mark_as_advanced(FORCE OPENNI2_INCLUDES)
