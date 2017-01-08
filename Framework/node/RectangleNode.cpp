#include "RectangleNode.h"

namespace opengl_framewordk
{
	RectangleNode::RectangleNode()
	{
		//
	}

	RectangleNode::~RectangleNode()
	{
		glDeleteBuffers(2, vbos);
	}

	bool RectangleNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Position_Color_Shader);
		setShaderProgram(shaderProgram);

		vertexCount = 4;
		vertexPosition = new float[vertexCount * 3];
		vertexColor = new float[vertexCount * 3];

		vertexPosition[0] = -0.99999f;
		vertexPosition[1] = -0.99999f;
		vertexPosition[2] = 0.0f;

		vertexPosition[3] = -0.99999f;
		vertexPosition[4] =  0.99999f;
		vertexPosition[5] = 0.0f;

		vertexPosition[6] = 0.99999f;
		vertexPosition[7] = 0.99999f;
		vertexPosition[8] = 0.0f;

		vertexPosition[9] =  0.99999f;
		vertexPosition[10] = -0.99999f;
		vertexPosition[11] = 0.0f;

		for (int i = 0; i < vertexCount * 3; i++)
		{
			vertexColor[i] = 0.9f;
		}
		
		glGenVertexArrays(1, &vao);
		glGenBuffers(2, vbos);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertexCount * 3, vertexPosition, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertexCount * 3, vertexColor, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);		

		glBindVertexArray(0);

		return true;
	}

	void RectangleNode::draw()
	{
		NODE_DRAW_SETUP();

		glm::mat4 model;
		getShaderProgram()->updateMVP(model);

		glBindVertexArray(vao);

		glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

		glBindVertexArray(0);
	}
}