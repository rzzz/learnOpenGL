#include "OpenglNode.h"

namespace opengl_framewordk
{
	OpenglNode::OpenglNode()
		: vertexCount(0), vertexPosition(nullptr), vertexColor(nullptr), vertexUV(nullptr), shader(nullptr), texture(nullptr), zOrder(0), vao(0)
	{
		//init();	//���캯�����֮ǰ�����ᷢ����̬
	}

	OpenglNode::~OpenglNode()
	{
		if (vertexPosition) delete vertexPosition;
		if (vertexColor) delete vertexColor;
		if (vertexUV) delete vertexUV;

		//texture manage by TextureCache

		if (vao)
		{
			glDeleteVertexArrays(1, &vao);
		}
	}

	bool OpenglNode::init()
	{
		//�������㣬��vao
		return true;
	}

	void OpenglNode::draw()
	{
		//��ͼ
	}
}