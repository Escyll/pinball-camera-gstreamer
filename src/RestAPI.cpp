#include "RestAPI.h"
#include <QSerialPortInfo>

RestAPI::RestAPI(QObject *parent)
    : QObject{parent}
{
    m_httpServer.route("/image", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request)
                       { /*return QHttpServerResponse("application/pinball.rgb", m_imageBytes);*/
            return QHttpServerResponse("image/jpeg", m_imageBytes); });
    m_httpServer.route("/action", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request)
                       {
        auto body = request.body();
        auto jsonBody = QJsonDocument::fromJson(body).object();
        QString action = jsonBody["Action"].toString();
        QString deviceId = jsonBody["DeviceId"].toString();
        qDebug().noquote().nospace() << deviceId << ": " << action;
        if (deviceId == "flipperLeft")
        {
            if (action == "pressed")
            {
                m_serialPort.write("FlipperLeft:pressed\n");
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            }
            else if (action == "unpressed")
            {
                m_serialPort.write("FlipperLeft:unpressed\n");
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            }
        } else if (deviceId == "flipperRight")
        {
            if (action == "pressed")
            {
                m_serialPort.write("FlipperRight:pressed\n");
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            }
            else if (action == "unpressed")
            {
                m_serialPort.write("FlipperRight:unpressed\n");
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            }
        }
        return QHttpServerResponse(QHttpServerResponse::StatusCode::BadRequest); });
    m_httpServer.listen(QHostAddress::Any, 8234);
    QString port = "COM6";
    m_serialPort.setPort(QSerialPortInfo(port));
    m_serialPort.setBaudRate(9600);
    if (!m_serialPort.open(QSerialPort::OpenModeFlag::ReadWrite))
    {
        qDebug().noquote() << "Failed to open" << port;
    }
    QObject::connect(&m_serialPort, &QSerialPort::readyRead, this, [this]
                     {
                         // while(m_serialPort.canReadLine())
                         // {
                         //     auto message = m_serialPort.readLine();
                         //     message = message.trimmed();
                         //     qDebug().noquote() << "Msg:" << message;
                         // }
                     });
}

QImage RestAPI::image() const
{
    return m_image;
}

qint32 pinballAPIFormat(QImage::Format format)
{
    switch (format)
    {
    case QImage::Format_RGB888:
        return 0;
    default:
        throw std::exception();
    }
}

void RestAPI::setImage(const QImage &newImage)
{
    /*if (m_image == newImage)
        return;
    m_image = newImage.convertToFormat(QImage::Format_RGB888);
    m_imageBytes.clear();
    QDataStream stream(&m_imageBytes, QIODevice::WriteOnly);
    stream << static_cast<qint32>(m_image.width());
    stream << static_cast<qint32>(m_image.height());
    stream << pinballAPIFormat(m_image.format());
    m_imageBytes.append(QByteArray((char *)m_image.bits(), m_image.sizeInBytes()));
    emit imageChanged();*/
}

void RestAPI::setJpeg(const QByteArray& data)
{
    m_imageBytes = data;
}
