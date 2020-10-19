FROM ubuntu:20.04

ENV TZ=Asia/Shanghai
RUN echo "Asia/Shanghai" > /etc/timezone
RUN apt-get update&&apt-get install tzdata && dpkg-reconfigure -f noninteractive tzdata && apt-get install -y gdb && apt-get install -y libcurl4-gnutls-dev \
  && apt-get install -y vim && apt-get install -y iputils-ping && apt-get install -y cmake
RUN apt-get update && apt-get install -y build-essential

RUN apt-get update && apt-get install -y ffmpeg
RUN apt-get install -y wget && apt-get install -y g++ && apt-get install -y unzip
RUN wget https://github.com/opencv/opencv/archive/3.4.5.zip

RUN apt-get install -y pkg-config
RUN unzip 3.4.5.zip\
  && cd opencv-3.4.5\
  && mkdir build\
  && cd build\
  && cmake ..\
  && make -j4\
  && make install

RUN apt-get install -y libavformat-dev\
  && apt-get install -y libavdevice-dev\
  && apt-get install -y libavcodec-dev\
  && apt-get install -y libswscale-dev\
  && apt-get install -y libavutil-dev

COPY ./test.cpp ./CMakeLists.txt /ffmpeg-demo/

EXPOSE 9000

WORKDIR /ffmpeg-demo/
#ENTRYPOINT ["/bin/bash"]
RUN mkdir build && mkdir build/image && cd build && cmake .. && make