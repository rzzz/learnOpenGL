#include "ImageNode.h"
#include "FreeImage/TextureManager.h"

namespace opengl_framework
{
	ImageNode::ImageNode()
	{
		//
	}

	ImageNode::~ImageNode()
	{
		glDeleteBuffers(4, vbos);
	}

	bool ImageNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Position_Color_Texture_Shader);
		setShaderProgram(shaderProgram);

		vertexCount = 4;
		vertexPosition = new float[vertexCount * 3];
		vertexColor = new float[vertexCount * 3];
		vertexUV = new float[vertexCount * 2];

		vertexPosition[0] = 0.8f;	//右上，z设正值，想让右边翘起来，更接近眼睛
		vertexPosition[1] = 0.8f;
		vertexPosition[2] = 0.2f;

		vertexPosition[3] = 0.8f;
		vertexPosition[4] = -0.8f;
		vertexPosition[5] = 0.2f;

		vertexPosition[6] = -0.8f;	//左下，z设负值，想让左边更远离眼睛
		vertexPosition[7] = -0.8f;
		vertexPosition[8] = -0.2f;

		vertexPosition[9] = -0.8f;
		vertexPosition[10] = 0.8f;
		vertexPosition[11] = -0.2f;

		for (int i = 0; i < vertexCount * 3; i++)
		{
			vertexColor[i] = 1.0f;
		}

		vertexUV[0] = 1.0f;
		vertexUV[1] = 1.0f;
		vertexUV[2] = 1.0f;
		vertexUV[3] = 0.0f;
		vertexUV[4] = 0.0f;
		vertexUV[5] = 0.0f;
		vertexUV[6] = 0.0f;
		vertexUV[7] = 1.0f;

		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};

		setTexture(TextureCache::getInstance()->getTextureFromFile("img/grass.png"));

		glGenVertexArrays(1, &vao);
		glGenBuffers(4, vbos);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertexCount * 3, vertexPosition, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);		

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertexCount * 3, vertexColor, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertexCount * 2, vertexUV, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);		//在此之前，可以解绑vbo，但是不可以解绑ebo

		return true;
	}

	void ImageNode::draw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		NODE_DRAW_SETUP();

		glActiveTexture(GL_TEXTURE0);
		getTexture()->bindTexture();
		getShaderProgram()->setUniformTexture("ourTexture", 0);

		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::rotate(trans, 0.2f, glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.9, 0.9, 0.9));
		getShaderProgram()->updateMVP(trans);
		
		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}
}