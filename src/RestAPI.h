#ifndef RESTAPI_H
#define RESTAPI_H

#include <QImage>
#include <QObject>
#include <QSerialPort>
#include <QtHttpServer>

class RestAPI : public QObject
{
    Q_OBJECT
public:
    explicit RestAPI(QObject *parent = nullptr);

    QImage image() const;
    void setImage(const QImage &newImage);

    void setJpeg(const QByteArray& data);

signals:
    void imageChanged();

private:
    QImage m_image;
    QByteArray m_imageBytes;
    QHttpServer m_httpServer;
    QSerialPort m_serialPort;
};

#endif // RESTAPI_H
