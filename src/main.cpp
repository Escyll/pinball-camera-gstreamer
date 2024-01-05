#include <opencv2/opencv.hpp>

#include <QCoreApplication>
#include <QImage>

#include <gst/gst.h>

#include "FrameFetcher.h"
#include "RestAPI.h"

int main(int argc, char *argv[])
{
    std::cout << "Let's go" << std::endl;
    gst_init(&argc, &argv);
    QCoreApplication app(argc, argv);

#ifdef __linux__
    // FrameFetcher frameFetcher("v4l2src device=/dev/video0 ! image/jpeg, width=1920, height=1080, framerate=30/1, format=MJPG ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true");
    FrameFetcher frameFetcher("v4l2src device=/dev/video0 ! image/jpeg, width=1920, height=1080, framerate=30/1, format=MJPG ! jpegdec ! videoconvert ! video/x-raw, format=RGB ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true");
#elif _WIN32
    //FrameFetcher frameFetcher("mfvideosrc device-index=1 ! videoconvert ! videoscale ! video/x-raw, width=960, height=540, format=RGB ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true");
    FrameFetcher frameFetcher("mfvideosrc device-index=1 ! image/jpeg, width=1280, height=720, framerate=30/1 ! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true");
#endif
    RestAPI restApi;

    QObject::connect(&frameFetcher, &FrameFetcher::imageAvailable, &restApi, &RestAPI::setImage);
    QObject::connect(&frameFetcher, &FrameFetcher::rawAvailable, &restApi, [&restApi](const QString&, const QByteArray& data)
        {
            restApi.setJpeg(data);
        });
    QObject::connect(&frameFetcher, &FrameFetcher::imageAvailable, &frameFetcher, [](const QImage &image)
                     { cv::imshow("Frame", cv::Mat(image.size().height(), image.size().width(), CV_8UC3, (char *)image.rgbSwapped().bits())); });
    QObject::connect(&frameFetcher, &FrameFetcher::rawAvailable, &frameFetcher, [](const QString &, const QByteArray &data)
                     {
                        /*cv::_InputArray array(data.data(), data.size());
                        cv::Mat imgMat = cv::imdecode(array, cv::ImreadModes::IMREAD_UNCHANGED);
                        cv::imshow("Frame", imgMat);*/ });

    return app.exec();
}