#include "FboNode.h"
#include "FreeImage/TextureManager.h"
#include "../Director.h"
#include <iostream>

namespace opengl_framewordk
{
	FboNode::FboNode()
	{
		//
	}

	FboNode::~FboNode()
	{
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteTextures(1, &texColorBuffer);
		glDeleteRenderbuffers(1, &rbo);

		glDeleteBuffers(4, vbos);
	}

	bool FboNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Position_Color_Texture_Shader);
		setShaderProgram(shaderProgram);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		
		// Generate texture		
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Director::getInstance()->getViewPortSizeWidth(), Director::getInstance()->getViewPortSizeHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Director::getInstance()->getViewPortSizeWidth(), Director::getInstance()->getViewPortSizeHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//use texture
		vertexCount = 4;
		vertexPosition = new float[vertexCount * 3];
		vertexColor = new float[vertexCount * 3];
		vertexUV = new float[vertexCount * 2];

		vertexPosition[0] = -0.5f;
		vertexPosition[1] = 1.0f;
		vertexPosition[2] = 0.0f;

		vertexPosition[3] = -0.5f;
		vertexPosition[4] = 0.5f;
		vertexPosition[5] = 0.0f;

		vertexPosition[6] = -1.0f;
		vertexPosition[7] = 0.5f;
		vertexPosition[8] = 0.0f;

		vertexPosition[9] = -1.0f;
		vertexPosition[10] = 1.0f;
		vertexPosition[11] = 0.0f;

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

		//纹理自行绑定

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

	void FboNode::beginDraw()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FboNode::endDraw()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FboNode::draw()
	{
		glDisable(GL_DEPTH_TEST);

		NODE_DRAW_SETUP();

		glActiveTexture(GL_TEXTURE0);
		//getTexture()->bindTexture();
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		getShaderProgram()->setUniformTexture("ourTexture", 0);

		glm::mat4 trans;
		getShaderProgram()->updateMVP(trans);

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}
}