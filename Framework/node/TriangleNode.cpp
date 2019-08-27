#include "TriangleNode.h"

namespace opengl_framework
{
	TriangleNode::TriangleNode()
	{
		//
	}

	TriangleNode::~TriangleNode()
	{
		glDeleteBuffers(2, vbos);
	}

	bool TriangleNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Position_Color_Shader);
		setShaderProgram(shaderProgram);

		vertexCount = 3;
		vertexPosition = new float[vertexCount * 3];
		vertexColor = new float[vertexCount * 3];

		vertexPosition[0] = -1.0f;
		vertexPosition[1] = -1.0f;
		vertexPosition[2] = 0.0f;

		vertexPosition[3] = 0.0f;
		vertexPosition[4] = 1.0f;
		vertexPosition[5] = 0.0f;

		vertexPosition[6] = 1.0f;
		vertexPosition[7] = -1.0f;
		vertexPosition[8] = 0.0f;

		vertexColor[0] = 0.9f;
		vertexColor[1] = 0.1f;
		vertexColor[2] = 0.1f;
		vertexColor[3] = 0.3f;
		vertexColor[4] = 0.4f;
		vertexColor[5] = 0.5f;
		vertexColor[6] = 0.1f;
		vertexColor[7] = 0.2f;
		vertexColor[8] = 0.1f;

		//vertexColor[0] = 0.9f;
		//vertexColor[1] = 0.0f;
		//vertexColor[2] = 0.0f;
		//vertexColor[3] = 0.9f;
		//vertexColor[4] = 0.0f;
		//vertexColor[5] = 0.0f;
		//vertexColor[6] = 0.9f;
		//vertexColor[7] = 0.0f;
		//vertexColor[8] = 0.0f;

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

	void TriangleNode::draw()
	{
		NODE_DRAW_SETUP();

		glm::mat4 model;
		getShaderProgram()->updateMVP(model);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glBindVertexArray(0);
	}
}