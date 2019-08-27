#include "../Framework/Application.h"

class App : public opengl_framework::Application
{
public:
	App();
	~App();

	virtual bool onApplicationCreate();
	virtual bool onApplicationCreateFinish();
};