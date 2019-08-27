#ifndef __FboNode_H__
#define __FboNode_H__

#include "OpenglNode.h"

namespace opengl_framework
{
	//图像节点，使用额外的3个vbo与一个ebo
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