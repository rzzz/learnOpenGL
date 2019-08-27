#ifndef __RectangleNode_H__
#define __RectangleNode_H__

#include "OpenglNode.h"

namespace opengl_framework
{
	//矩形节点，使用额外的2个vbo
	class RectangleNode : public OpenglNode
	{
	public:
		RectangleNode();
		~RectangleNode();

		CREATE_FUNC(RectangleNode);

	public:
		virtual bool init();
		virtual void draw();

	private:
		GLuint vbos[2];
	};
}

#endif