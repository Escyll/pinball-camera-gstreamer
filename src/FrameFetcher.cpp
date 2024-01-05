#include <gst/gst.h>
#include <gst/app/app.h>

#include <QImage>

#include "FrameFetcher.h"

GstFlowReturn new_preroll(GstAppSink * /*appsink*/, gpointer /*data*/)
{
    return GST_FLOW_OK;
}

GstFlowReturn new_sample(GstAppSink *appsink, gpointer data)
{
    FrameFetcher *myFrameFetcher = static_cast<FrameFetcher *>(data);
    static int framecount = 0;

    GstSample *sample = gst_app_sink_pull_sample(appsink);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstStructure *structure = gst_caps_get_structure(caps, 0);
    auto name = gst_structure_get_name(structure);

    if (!(framecount % 30))
    {
        g_print(".");
    }

    if (!framecount)
    {
        g_print("caps: %s\n", gst_caps_to_string(caps));
    }
    framecount++;

    GstMapInfo map;
    gst_buffer_map(buffer, &map, GST_MAP_READ);
    if (name == std::string("image/jpeg"))
    {
        emit myFrameFetcher->rawAvailable(name, QByteArray::fromRawData((char *)map.data, map.size));
    }
    else
    {
        const int width = g_value_get_int(gst_structure_get_value(structure, "width"));
        const int height = g_value_get_int(gst_structure_get_value(structure, "height"));
        emit myFrameFetcher->imageAvailable(QImage(map.data, width, height, QImage::Format_RGB888));
    }

    gst_buffer_unmap(buffer, &map);
    gst_sample_unref(sample);

    return GST_FLOW_OK;
}

static gboolean my_bus_callback(GstBus *bus, GstMessage *message, gpointer /*data*/)
{
    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ERROR:
    {
        GError *err;
        gchar *debug;

        gst_message_parse_error(message, &err, &debug);
        g_print("Error: %s\n", err->message);
        g_error_free(err);
        g_free(debug);
        break;
    }
    case GST_MESSAGE_EOS:
        break;
    default:
        break;
    }
    return true;
}

FrameFetcher::FrameFetcher(const char *pipeline_description)
{
    GError *error = nullptr;
    m_pipeline = gst_parse_launch(pipeline_description, &error);

    if (error)
    {
        g_print("could not construct pipeline: %s\n", error->message);
        g_error_free(error);
        exit(-1);
    }

    GstElement *sink = gst_bin_get_by_name(GST_BIN(m_pipeline), "sink");

    gst_app_sink_set_emit_signals((GstAppSink *)sink, true);
    gst_app_sink_set_drop((GstAppSink *)sink, true);
    gst_app_sink_set_max_buffers((GstAppSink *)sink, 1);
    GstAppSinkCallbacks callbacks = {nullptr, new_preroll, new_sample};
    gst_app_sink_set_callbacks(GST_APP_SINK(sink), &callbacks, this, nullptr);

    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
    gst_bus_add_watch(bus, my_bus_callback, nullptr);
    gst_object_unref(bus);

    gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
}

FrameFetcher::~FrameFetcher()
{
    gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(m_pipeline));
}