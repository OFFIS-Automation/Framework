#include "GStreamerInitializer.h"

#include <string>

#include <Windows.h>

#include <gst/gst.h>
#include <gst/gstregistry.h>

GStreamerInitializer GStreamerInitializer::gStreamerInitializer;

GStreamerInitializer::GStreamerInitializer()
{
	gst_init(nullptr, nullptr);

	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	auto fileName = std::string(buffer);
	auto path = fileName.substr(0, fileName.find_last_of('\\')) + "\\plugins\\gstreamer-plugins\\";

	gst_registry_scan_path(gst_registry_get(), path.c_str());
}
