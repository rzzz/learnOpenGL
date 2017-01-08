#ifndef __SkyBox_H__
#define __SkyBox_H__

#include "OpenglNode.h"

#include <vector>
using namespace std;

namespace opengl_framewordk
{
	class SkyBox : public OpenglNode
	{
	public:
		SkyBox();
		~SkyBox();

		CREATE_FUNC(SkyBox);

	public:
		virtual bool init();
		virtual void draw();

		GLuint loadCubemap(vector<const GLchar*> faces);

	private:
		GLuint vbo;
		GLuint cubemapTexture;
	};
}

#endif