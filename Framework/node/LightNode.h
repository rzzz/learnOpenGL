#ifndef __LightNode_H__
#define __LightNode_H__

#include "OpenglNode.h"

namespace opengl_framework
{
	class LightNode : public OpenglNode
	{
	public:
		LightNode();
		~LightNode();

		CREATE_FUNC(LightNode);

	public:
		virtual bool init();
		virtual void draw();

	private:
		GLuint vbo;
	};
}

#endif