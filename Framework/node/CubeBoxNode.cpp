#include "CubeBoxNode.h"
#include "FreeImage/TextureManager.h"
#include "../Director.h"

namespace opengl_framework
{
	CubeBoxNode::CubeBoxNode()
	{
		//
	}

	CubeBoxNode::~CubeBoxNode()
	{
		glDeleteBuffers(1, &vbo);
	}

	bool CubeBoxNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Position_Texture_Shader);
		setShaderProgram(shaderProgram);

		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		};
		// World space positions of our cubes
		cubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
		cubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
		cubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
		cubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
		cubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
		cubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
		cubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
		cubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
		cubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
		cubePositions[9] = glm::vec3(-0.8f, 0.8f, 0.9f);

		setTexture(TextureCache::getInstance()->getTextureFromFile("img/container.jpg"));

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindVertexArray(0);

		return true;
	}

	void CubeBoxNode::draw()
	{
		NODE_DRAW_SETUP();

		glActiveTexture(GL_TEXTURE0);
		getTexture()->bindTexture();
		getShaderProgram()->setUniformTexture("ourTexture", 0);

		glBindVertexArray(vao);
		//int i = 9;
		for (GLuint i = 0; i < 10; i++)
		{
			// Calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 0.2f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			getShaderProgram()->updateMVP(model);

			glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
		}

		glBindVertexArray(0);
	}
}