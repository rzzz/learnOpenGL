#ifndef __Application_H__
#define __Application_H__

#include <windows.h>

namespace opengl_framewordk
{
	//应用程序，子类实现onApplicationCreate，主循环run
	class Application
	{
	public:
		Application();
		~Application();

	public:
		virtual bool onApplicationCreate(){ return true; };
		virtual bool onApplicationCreateFinish(){ return true; };

		void run();

		void setAnimationInterval(double interval);

	private:
		LARGE_INTEGER m_nAnimationInterval;
	};
}

#endif