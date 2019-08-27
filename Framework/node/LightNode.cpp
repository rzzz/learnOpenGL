#include "LightNode.h"
#include "FreeImage/TextureManager.h"
#include "../Director.h"

namespace opengl_framework
{
	LightNode::LightNode()
	{
		//
	}

	LightNode::~LightNode()
	{
		glDeleteBuffers(1, &vbo);
	}

	bool LightNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Position_Color_Shader);
		setShaderProgram(shaderProgram);

		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f
		};

		for (int i = 0; i < 218; i++)
		{
			if (i % 6 == 3 || i % 6 == 4 || i % 6 == 5)
			{
				switch (i % 6)
				{
				case 3:
					vertices[i] = lightColor.x;
					break;
				case 4:
					vertices[i] = lightColor.y;
					break;
				case 5:
					vertices[i] = lightColor.z;
					break;
				}
			}
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindVertexArray(0);

		return true;
	}

	void LightNode::draw()
	{
		NODE_DRAW_SETUP();

		glBindVertexArray(vao);

		glm::mat4 model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		getShaderProgram()->updateMVP(model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}
}