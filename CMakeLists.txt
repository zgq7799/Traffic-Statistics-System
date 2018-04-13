project(Traffic_Statistics)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(./src SRC_LIST)
include_directories(./include ./include/package_bgs)
add_subdirectory(include)
find_package( OpenCV REQUIRED )

SET (CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11")

file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${CURRENT_HEADERS})

set(OpenCV_LIBS
    libopencv_core.so
#    libopencv_face.so
    libopencv_highgui.so
    libopencv_imgcodecs.so
    libopencv_imgproc.so
#    libopencv_calib3d.so
    libopencv_features2d.so
#    libopencv_flann.so
    libopencv_imgproc.so
#    libopencv_ml.so
#    libopencv_objdetect.so
#    libopencv_photo.so
#    libopencv_shape.so
#    libopencv_stitching.so
#    libopencv_superres.so
    libopencv_video.so
    libopencv_videoio.so
#    libopencv_videostab.so
    libopencv_tracking.so
    )
target_link_libraries( Traffic_Statistics ${OpenCV_LIBS} -ltiff /home/zgq/Desktop/bgslibrary-master/build/libbgs.a)
