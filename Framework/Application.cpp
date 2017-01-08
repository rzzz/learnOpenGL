#include "Application.h"
#include "Director.h"

namespace opengl_framewordk
{
	Application::Application()
	{
		//
	}

	Application::~Application()
	{

	}

	void Application::setAnimationInterval(double interval)
	{
		LARGE_INTEGER nFreq;
		QueryPerformanceFrequency(&nFreq);
		m_nAnimationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
	}

	void Application::run()
	{
		setAnimationInterval(1.0 / 60);

		if (!onApplicationCreate())
		{
			return;
		}

		if (!Director::getInstance()->isInitReady())
		{
			if (!Director::getInstance()->initOpenGLWindow())
			{
				return;
			}
		}

		onApplicationCreateFinish();

//#define DIRECTOR_LOOP

#ifdef DIRECTOR_LOOP
		Director::getInstance()->drawScene();
		Director::destoryInstance();
#else
		LARGE_INTEGER nFreq;
		LARGE_INTEGER nLast;
		LARGE_INTEGER nNow;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nLast);

		while (1)
		{
			// Get current time tick.
			QueryPerformanceCounter(&nNow);

			// If it's the time to draw next frame, draw it, else sleep a while.
			if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
			{				
				bool isOK = Director::getInstance()->drawScene((double)(nNow.QuadPart - nLast.QuadPart)/(double)nFreq.QuadPart);
				nLast.QuadPart = nNow.QuadPart;

				if (!isOK)
				{
					Director::destoryInstance();
					return;
				}
			}
			else
			{
				Sleep(0);
			}
		}
#endif


		//////////////////////////////////////////////////////////////////////////////////////
	}
}