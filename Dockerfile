FROM jrottenberg/ffmpeg:4.1-ubuntu

ENV TZ=Asia/Shanghai
RUN echo "Asia/Shanghai" > /etc/timezone
RUN apt-get update&&apt-get install tzdata && dpkg-reconfigure -f noninteractive tzdata && apt-get install -y gdb && apt-get install -y libcurl4-gnutls-dev \
  && apt-get install -y vim && apt-get install -y iputils-ping && apt-get install -y cmake
RUN apt-get update && apt-get install -y build-essential

COPY ./rtsp.cpp ./CMakeLists.txt /ffmpeg-demo/

EXPOSE 9000

WORKDIR /ffmpeg-demo/
RUN mkdir build && mkdir build/image && cd build && cmake .. && make