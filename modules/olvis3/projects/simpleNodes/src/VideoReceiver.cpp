// OFFIS Automation Framework
// Copyright (C) 2013-2018 OFFIS e.V.
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

#include <iostream>
#include <thread>

#include <opencv2/imgproc/imgproc.hpp>

#include <gst/app/gstappsink.h>

#include "VideoReceiver.h"
#include "GStreamerInitializer.h"

REGISTER_FILTER(VideoReceiver);

VideoReceiver::VideoReceiver()
{
	// Ports
	setName(QObject::tr("Video Receiver"));
	setDesc(QObject::tr("Receives data from the network"));
	setGroup("input");

	m_ip.setName(QObject::tr("Source Address"));
	m_ip.setDefault("0.0.0.0");
	m_ip.setVisibility(AdvancedPortVisibility);
	addInputPort(m_ip);

	m_port.setName(QObject::tr("Source Port"));
	m_port.setDefault(12000);
	m_port.setVisibility(AdvancedPortVisibility);
	addInputPort(m_port);

	m_frameOut.setName(QObject::tr("Image Out"));
	m_frameOut.setDesc(QObject::tr("Image output"));
	addOutputPort(m_frameOut);

	// Gstreamer

	m_loop = g_main_loop_new(nullptr, false);
	m_source = gst_element_factory_make("udpsrc", "videosource");

	g_object_set(G_OBJECT(m_source),
		"caps", gst_caps_new_simple("application/x-rtp", nullptr),
		nullptr);

	m_pipeline = gst_pipeline_new("pipeline");
	auto depayloader = gst_element_factory_make("rtph264depay", "depayloader");
	auto decoder = gst_element_factory_make("avdec_h264", "decoder");
	auto converter = gst_element_factory_make ("videoconvert", "converter");
	m_sink = gst_element_factory_make("appsink", "cvsink");

	g_object_set(G_OBJECT(m_sink),
		"emit-signals", true,
		"max-buffers", 2,
		"drop", true,
		"caps", gst_caps_new_simple ("video/x-raw",
			 "format", G_TYPE_STRING, "BGR",
			 nullptr),
		nullptr);

	g_signal_connect(m_sink, "new-sample",
		G_CALLBACK(static_cast<GstFlowReturn(*)(GstElement *, decltype(this))>(
			[](GstElement *, auto* receiver){return receiver->onNewFrame();})), this);

	gst_bin_add_many(GST_BIN(m_pipeline), m_source, depayloader, decoder, converter, m_sink,
		nullptr);
	gst_element_link_many(m_source, depayloader, decoder, converter, m_sink, nullptr);
}

void VideoReceiver::start()
{
	g_object_set(G_OBJECT(m_source),
		"address", m_ip.getValue().toStdString().c_str(),
		"port", m_port.getValue(),
		nullptr);

	gst_element_set_state(m_pipeline, GST_STATE_PLAYING);

	std::thread(g_main_loop_run, m_loop).detach();
}

void VideoReceiver::stop()
{
	gst_element_set_state(m_pipeline, GST_STATE_NULL);

	g_main_loop_quit(m_loop);
}

GstFlowReturn VideoReceiver::onNewFrame()
{
	auto sample = gst_app_sink_pull_sample(GST_APP_SINK(m_sink));

	if (!sample)
		return GST_FLOW_ERROR;

	gint width, height;

	auto caps = gst_caps_get_structure(gst_sample_get_caps(sample), 0);

	gst_structure_get_int(caps, "width", &width);
	gst_structure_get_int(caps, "height", &height);

	auto buffer = gst_sample_get_buffer(sample);

	GstMapInfo info;

	gst_buffer_map(buffer, &info, GST_MAP_READ);
	const auto frame = cv::Mat(cv::Size(width, height), CV_8UC3, info.data);
	gst_buffer_unmap(buffer, &info);

	m_frameOut.send(frame);

	gst_sample_unref(sample);

	return GST_FLOW_OK;
}
