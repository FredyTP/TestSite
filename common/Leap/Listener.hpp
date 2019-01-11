#pragma once


#include <Leap.h>
namespace Leap
{

	class SampleListener : public Leap::Listener {

	public:

		virtual void onInit(const Controller&);
		virtual void onConnect(const Controller&);
		virtual void onDisconnect(const Controller&);
		virtual void onExit(const Controller&);
		virtual void onFrame(const Controller&);
		virtual void onFocusGained(const Controller&);
		virtual void onFocusLost(const Controller&);
		virtual void onDeviceChange(const Controller&);
		virtual void onServiceConnect(const Controller&);
		virtual void onServiceDisconnect(const Controller&);
		virtual void onServiceChange(const Controller&);
		virtual void onDeviceFailure(const Controller&);
		virtual void onLogMessage(const Controller&, MessageSeverity severity, int64_t timestamp, const char* msg);
	};
}
