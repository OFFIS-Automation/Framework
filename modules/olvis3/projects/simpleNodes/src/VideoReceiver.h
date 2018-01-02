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

#ifndef VIDEORECEIVER_H
#define VIDEORECEIVER_H

#include <gst/gst.h>

#include <filter/PluginInterface.h>

#include <ports/ImagePort.h>
#include <ports/StringPort.h>
#include <ports/IntegerPort.h>

class VideoReceiver : public UserFilter
{
	public:
		VideoReceiver();

		void start();
		void stop();
		// FIXME: Empty execute, not firing the output port, causes the Processor to think it is
		// finished. Works only with a "stop behavior" of "never stop".
		void execute() {};

	private:
		// Ports
		in::String m_ip;
		in::Integer m_port;

        out::Image m_frameOut;

		// Member
		GstFlowReturn onNewFrame();

		GMainLoop* m_loop;
		GstElement* m_pipeline;

		GstElement* m_source;
		GstElement* m_sink;
};

#endif // VIDEORECEIVER_H
