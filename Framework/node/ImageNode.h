#ifndef __ImageNode_H__
#define __ImageNode_H__

#include "OpenglNode.h"

namespace opengl_framewordk
{
	//ͼ��ڵ㣬ʹ�ö����3��vbo��һ��ebo
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