#ifndef __OpenglNode_H__
#define __OpenglNode_H__

#include "../Shader/ShaderProgram.h"
#include "../Shader/Texture.h"

namespace opengl_framewordk
{

	//解除vao绑定，解除纹理绑定，使用其着色器
#define NODE_DRAW_SETUP() \
	do { \
	glBindVertexArray(0); \
	glBindBuffer(GL_ARRAY_BUFFER, 0); \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); \
	glBindTexture(GL_TEXTURE_2D, 0); \
	getShaderProgram()->use(); \
	} while (0)

	//创建节点类型，并调用init
#define CREATE_FUNC(__TYPE__) \
	static __TYPE__* create() \
	{ \
		__TYPE__ *pRet = new __TYPE__(); \
		if (pRet && pRet->init()) \
		{ \
			return pRet; \
		} \
		else \
		{ \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
		} \
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////

	//节点基类：着色器，纹理，vao
	class OpenglNode
	{
	public:
		OpenglNode();
		~OpenglNode();

	public:
		virtual bool init();
		virtual void draw();

		CREATE_FUNC(OpenglNode);

	public:
		void setShaderProgram(ShaderProgram* shader){ this->shader = shader; }
		ShaderProgram* getShaderProgram(){ return shader; }

		void setTexture(Texture* texture){ this->texture = texture; }
		Texture* getTexture(){ return texture; }

		int getZOrder(){ return zOrder; }
		void setZOrder(int zo){ zOrder = zo; }

	protected:
		ShaderProgram* shader;
		Texture* texture;

		int vertexCount;
		float* vertexPosition;
		float* vertexColor;
		float* vertexUV;

		GLuint vao;		//统一使用vao：这样不用记录vbo，ebo
		int zOrder;

	};
}

#endif