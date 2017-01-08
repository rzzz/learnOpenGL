#ifndef __Application_H__
#define __Application_H__

#include <windows.h>

namespace opengl_framewordk
{
	//Ӧ�ó�������ʵ��onApplicationCreate����ѭ��run
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