#ifndef __CubeBoxNode_H__
#define __CubeBoxNode_H__

#include "OpenglNode.h"

namespace opengl_framework
{
	//���ӽڵ㣬ʹ�ö����һ��vbo
	class CubeBoxNode : public OpenglNode
	{
	public:
		CubeBoxNode();
		~CubeBoxNode();

		CREATE_FUNC(CubeBoxNode);

	public:
		virtual bool init();
		virtual void draw();

	private:
		GLuint vbo;
		glm::vec3 cubePositions[10];
	};
}

#endif