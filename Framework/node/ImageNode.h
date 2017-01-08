#ifndef __ImageNode_H__
#define __ImageNode_H__

#include "OpenglNode.h"

namespace opengl_framewordk
{
	//图像节点，使用额外的3个vbo与一个ebo
	class ImageNode : public OpenglNode
	{
	public:
		ImageNode();
		~ImageNode();

		CREATE_FUNC(ImageNode);

	public:
		virtual bool init();
		virtual void draw();

	private:
		GLuint vbos[4];
	};
}

#endif