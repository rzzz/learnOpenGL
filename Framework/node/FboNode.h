#ifndef __FboNode_H__
#define __FboNode_H__

#include "OpenglNode.h"

namespace opengl_framework
{
	//ͼ��ڵ㣬ʹ�ö����3��vbo��һ��ebo
	class FboNode : public OpenglNode
	{
	public:
		FboNode();
		~FboNode();

		CREATE_FUNC(FboNode);

	public:
		virtual bool init();
		virtual void draw();

		void beginDraw();
		void endDraw();

	private:
		GLuint framebuffer;
		GLuint texColorBuffer;
		GLuint rbo;

		GLuint vbos[4];

	};
}

#endif