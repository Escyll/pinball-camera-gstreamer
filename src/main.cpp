#include <opencv2/opencv.hpp>

#include <QCoreApplication>
#include <QImage>

#include "FrameFetcher.h"
#include <gst/gst.h>

int main(int argc, char *argv[])
{
    gst_init(&argc, &argv);
    QCoreApplication app(argc, argv);
    FrameFetcher frameFetcher("v4l2src device=/dev/video0 ! image/jpeg, width=1920, height=1080, framerate=30/1, format=MJPG ! jpegdec ! videoconvert ! video/x-raw, format=RGB ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true");
    QObject::connect(&frameFetcher, &FrameFetcher::frameAvailable, &frameFetcher, [](const QImage &image)
                     { cv::imshow("Frame", cv::Mat(image.size().height(), image.size().width(), CV_8UC3, (char *)image.rgbSwapped().bits())); });

    return app.exec();
}