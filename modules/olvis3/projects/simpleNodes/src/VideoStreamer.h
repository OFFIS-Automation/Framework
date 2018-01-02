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

#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <filter/PluginInterface.h>

#include <ports/ImagePort.h>
#include <ports/StringPort.h>
#include <ports/IntegerPort.h>

#include <gst/gst.h>

class VideoStreamer : public UserFilter
{
	public:
		VideoStreamer();

		void execute();
		void start();
		void stop();

	private:
		// Ports
		in::String m_ip;
		in::Integer m_port;

		in::String m_tune;
		in::String m_speed_preset;

        in::Image m_frameIn;

		// Member
		bool m_initialized = false;

		GMainLoop* m_loop;
		GstElement* m_pipeline;

		GstElement* m_source;
		GstElement* m_encoder;
		GstElement* m_sink;

		GstBuffer* m_buffer;
		GstClockTime m_timestamp = 0;
};

#endif // VIDEOSTREAMER_H
