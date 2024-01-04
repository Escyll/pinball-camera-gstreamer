#ifndef FRAMEFETCHER_H
#define FRAMEFETCHER_H

#pragma once

#include <QObject>
#include <gst/gstelement.h>

class FrameFetcher : public QObject
{
    Q_OBJECT
public:
    FrameFetcher(const char *pipeline_description);
    ~FrameFetcher();

signals:
    void rawAvailable(const QString &type, const QByteArray &data);
    void imageAvailable(const QImage &image);

private:
    GstElement *m_pipeline;
};

#endif