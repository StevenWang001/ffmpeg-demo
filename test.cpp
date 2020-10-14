#include <cstdio>
#include <cstdlib>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}

int main(int argc, char **argv) {
    // Register everything
    av_register_all();

    // Open the initial context variables that are needed
    AVFormatContext *format_ctx = avformat_alloc_context();

    avformat_network_init();

    //open RTSP
    //rtmp://172.18.64.55:1935/live/34020000002000000001@34020000001310000003
    //rtsp://admin:admin123@172.18.64.151:554/cam/realmonitor?channel=1&subtype=0
    if (avformat_open_input(&format_ctx, "rtsp://admin:admin123@172.18.64.151:554/cam/realmonitor?channel=1&subtype=0",
                            NULL, NULL) != 0) {
        return EXIT_FAILURE;
    }

    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
        return EXIT_FAILURE;
    }

    //search video stream
    int video_stream_index;
    for (int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            video_stream_index = i;
    }

    //start reading packets from stream and write them to file
    //av_read_play(format_ctx);    //play RTSP

    // Get the codec
    AVCodec *codec = NULL;
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        exit(1);
    }

    // Add this to allocate the context by codec
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);

    avcodec_get_context_defaults3(codec_ctx, codec);

    if (avcodec_open2(codec_ctx, codec, NULL) < 0)
        exit(1);

    AVFrame *picture = av_frame_alloc();
    AVPacket *packet = av_packet_alloc();
    av_init_packet(packet);
    while (av_read_frame(format_ctx, packet) >= 0) { //read frames
        std::cout << "1 Frame: " << std::endl;
        if (packet->stream_index == video_stream_index) {    //packet is video
            std::cout << "2 Is Video" << std::endl;
            std::cout << "4 decoding" << std::endl;
            int check = 0;
            int result = avcodec_decode_video2(codec_ctx, picture, &check, packet);
            std::cout << "Bytes decoded " << result << " check " << check
                      << std::endl;
        }
        av_packet_unref(packet);
        av_init_packet(packet);
    }
    av_frame_free(&picture);
    av_packet_free(&packet);
    avcodec_close(codec_ctx);
    avformat_close_input(&format_ctx);

    return (EXIT_SUCCESS);
}
