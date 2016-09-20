#ifndef GSTREAMERINITIALIZER_H
#define GSTREAMERINITIALIZER_H

// Singleton to initialize gstreamer

class GStreamerInitializer
{
	private:
		GStreamerInitializer();

		static GStreamerInitializer gStreamerInitializer;
};

#endif // GSTREAMERINITIALIZER_H
