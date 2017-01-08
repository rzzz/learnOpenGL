#ifndef __TriangleNode_H__
#define __TriangleNode_H__

#include "OpenglNode.h"

namespace opengl_framewordk
{
	//三角形，使用额外的两个vbo
	class TriangleNode : public OpenglNode
	{
	public:
		TriangleNode();
		~TriangleNode();

		CREATE_FUNC(TriangleNode);

	public:
		virtual bool init();
		virtual void draw();

	private:
		GLuint vbos[2];
	};
}

#endif