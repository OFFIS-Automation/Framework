// OFFIS Automation Framework
// Copyright (C) 2013-2016 OFFIS e.V.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <thread>

#include <gst/app/gstappsrc.h>

#include "VideoStreamer.h"

REGISTER_FILTER(VideoStreamer);

VideoStreamer::VideoStreamer()
{
	setName(QObject::tr("Video Streamer"));
	setDesc(QObject::tr("Streams incoming data to the network"));
	setGroup("output");

	m_ip.setName(QObject::tr("Destination Address"));
	m_ip.setDefault("134.106.47.255");
	m_ip.setVisibility(AdvancedPortVisibility);
	addInputPort(m_ip);

	m_port.setName(QObject::tr("Destination Port"));
	m_port.setDefault(12000);
	m_port.setVisibility(AdvancedPortVisibility);
	addInputPort(m_port);

	m_tune.setName(QObject::tr("H264 Tune"));
	m_tune.setDefault("zerolatency");
	m_tune.setDesc("stillimage, fastdecode, zerolatency");
	m_tune.setVisibility(ExpertPortVisibility);
	addInputPort(m_tune);

	m_speed_preset.setName(QObject::tr("H264 Speed-Preset"));
	m_speed_preset.setDefault("superfast");
	m_speed_preset.setDesc("None, ultrafast, superfast, veryfast, faster, fast, medium, slow, "
		"slower, veryslow, placebo");
	m_speed_preset.setVisibility(ExpertPortVisibility);
	addInputPort(m_speed_preset);

	m_frameIn.setName(QObject::tr("Image In"));
	m_frameIn.setDesc(QObject::tr("Image input"));
	addInputPort(m_frameIn);

	// Gstreamer

	gst_init(nullptr, nullptr);

	m_loop = g_main_loop_new(nullptr, false);
	m_source = gst_element_factory_make("appsrc", "cvsource");

	g_object_set(G_OBJECT(m_source),
		"stream-type", GST_APP_STREAM_TYPE_STREAM,
		"format", GST_FORMAT_TIME,
		nullptr);

	m_pipeline = gst_pipeline_new("pipeline");
	auto converter = gst_element_factory_make ("videoconvert", "converter");
	m_encoder = gst_element_factory_make("x264enc", "encoder");
	auto payloader = gst_element_factory_make("rtph264pay", "payloader");

	g_object_set(G_OBJECT(payloader),
		"config-interval", 1,
		nullptr);

	m_sink = gst_element_factory_make("udpsink", "videosink");

	g_object_set(G_OBJECT(m_sink),
		"sync", false,
		nullptr);

	gst_bin_add_many(GST_BIN(m_pipeline), m_source, converter, m_encoder, payloader, m_sink,
		nullptr);
	gst_element_link_many(m_source, converter, m_encoder, payloader, m_sink, nullptr);
}

void VideoStreamer::start()
{
	gst_util_set_object_arg(G_OBJECT(m_encoder), "tune", m_tune.getValue().toStdString().c_str());
	gst_util_set_object_arg(G_OBJECT(m_encoder), "speed-preset",
		m_speed_preset.getValue().toStdString().c_str());

	g_object_set(G_OBJECT(m_sink),
		"host", m_ip.getValue().toStdString().c_str(),
		"port", m_port.getValue(),
		nullptr);

	gst_element_set_state(m_pipeline, GST_STATE_PLAYING);

	std::thread(g_main_loop_run, m_loop).detach();
}

void VideoStreamer::stop()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);

	g_main_loop_quit(m_loop);

	gst_buffer_unref(m_buffer);
	m_initialized = false;
}

void VideoStreamer::execute()
{
	const cv::Mat frame = m_frameIn;

	if (!m_initialized)
	{
		const auto width = frame.cols;
		const auto height = frame.rows;
		const auto channels = frame.channels();
		const auto size = width * height * channels;

		m_buffer = gst_buffer_new_allocate(nullptr, size, nullptr);

		g_object_set(G_OBJECT(m_source),
			"caps", gst_caps_new_simple ("video/x-raw",
				 "format", G_TYPE_STRING, "BGR",
				 "width", G_TYPE_INT, width,
				 "height", G_TYPE_INT, height,
				 "framerate", GST_TYPE_FRACTION, 0, 1,
				 nullptr),
			nullptr);

		m_initialized = true;
	}

	gst_buffer_fill(m_buffer, 0, frame.data, gst_buffer_get_size(m_buffer));

	GST_BUFFER_PTS(m_buffer) = m_timestamp;
	GST_BUFFER_DURATION(m_buffer) = gst_util_uint64_scale_int(1, GST_SECOND, 2);

	m_timestamp += GST_BUFFER_DURATION(m_buffer);

	GstFlowReturn returnValue;
	g_signal_emit_by_name(m_source, "push-buffer", m_buffer, &returnValue);
}
