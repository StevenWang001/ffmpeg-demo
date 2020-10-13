cmake_minimum_required(VERSION 2.8)
set(name "ffmpeg-demo")
set(exename "RTSP")

set(CMAKE_CXX_STANDARD 11)

project(${name})

# ffmpeg lib
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

include_directories(${PROJECT_SOURCE_DIR}/)

# add complate file
aux_source_directory(. SRC_DIR)

add_executable(${exename} ${SRC_DIR} rtsp.cpp)

#pthreaed
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET ${exename} PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET ${exename} PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(${exename} "${CMAKE_THREAD_LIBS_INIT}")
endif()

target_include_directories(${exename} PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR})
target_link_libraries(${exename} PRIVATE ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${SWSCALE_LIBRARY})
