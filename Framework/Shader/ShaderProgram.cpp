#include "ShaderProgram.h"
#include "shaders\Shader.h"
#include "../Director.h"
#include "../CameraView.h"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

namespace opengl_framewordk
{
	ShaderProgram::ShaderProgram(const string &vertexSourcePath, const string &fragmentSourcePath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(ifstream::badbit);
		fShaderFile.exceptions(ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexSourcePath);
			fShaderFile.open(fragmentSourcePath);
			stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		auto vShaderCode = vertexCode.c_str();
		auto fShaderCode = fragmentCode.c_str();

		initWithSorce(vShaderCode, fShaderCode);

		//delete vShaderCode;	//can not do that
		//delete fShaderCode;
	}

	ShaderProgram::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource, int nouser)
	{
		initWithSorce(vertexShaderSource, fragmentShaderSource);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteShader(programID);
	}

	void ShaderProgram::initWithSorce(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		// Build and compile our shader program
		// Vertex shader
		const GLchar *Vsources[] = {
			"#version 330 core\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;	\n"
			"uniform mat4 projection;\n"
			"//custom INCLUDES END\n\n",
			vertexShaderSource
		};
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		int n = sizeof(Vsources) / sizeof(*Vsources);
		glShaderSource(vertexShader, n, Vsources, NULL);
		glCompileShader(vertexShader);
		// Check for compile time errors
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Fragment shader
		const GLchar *Fsources[] = {
			"#version 330 core\n"
			"//custom INCLUDES END\n\n",
			fragmentShaderSource
		};
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, sizeof(Fsources) / sizeof(*Fsources), Fsources, NULL);
		glCompileShader(fragmentShader);
		// Check for compile time errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Link shaders
		programID = glCreateProgram();
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);
		// Check for linking errors
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		updateBultinProgram();
	}

	void ShaderProgram::updateBultinProgram()
	{
		//ÉèÖÃattribute
		//ÉèÖÃuniform
	}

	void ShaderProgram::use()
	{
		glUseProgram(this->programID);
	}

	void ShaderProgram::setUniformTexture(const char* locationName, GLenum textureUnit)
	{
		glUniform1i(glGetUniformLocation(programID, locationName), textureUnit);
	}

	void ShaderProgram::setUniformMatrix(const char* locationName, float* matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(programID, locationName), 1, GL_FALSE, matrix);
	}

	void ShaderProgram::updateMVP(glm::mat4 &model, bool forceSetSkyBox)
	{
		// Create camera transformation
		glm::mat4 view;
		glm::mat4 projection;

		auto camera = Director::getInstance()->getCamera();
		if (camera)
		{
			view = camera->GetViewMatrix();

			float v = camera->Zoom;
			projection = glm::perspective(glm::radians(v), (float)Director::getInstance()->getViewPortSizeWidth() / (float)Director::getInstance()->getViewPortSizeHeight(), 0.1f, 1000.0f);
		}

		if (forceSetSkyBox && camera)
		{
			view = glm::mat4(glm::mat3(camera->GetViewMatrix()));	// Remove any translation component of the view matrix
		}
		
		GLint lightLoc = glGetUniformLocation(programID, "lightColor");
		glUniform3f(lightLoc, lightColor.x, lightColor.y, lightColor.z);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(programID, "model");
		GLint viewLoc = glGetUniformLocation(programID, "view");
		GLint projLoc = glGetUniformLocation(programID, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	ShaderProgramCache* ShaderProgramCache::instance = nullptr;
	ShaderProgramCache::ShaderProgramCache()
		:shaders()
	{
		init();
	}

	ShaderProgramCache::~ShaderProgramCache()
	{
		for (map<int, ShaderProgram*>::iterator itr = shaders.begin(); itr != shaders.end(); itr++)
		{
			delete itr->second;
		}

		shaders.clear();
	}

	ShaderProgramCache* ShaderProgramCache::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new ShaderProgramCache();
		}

		return instance;
	}
	
	void ShaderProgramCache::destroyInstacne()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void ShaderProgramCache::init()
	{
		//
	}

	ShaderProgram* ShaderProgramCache::getShaderProgram(int type)
	{
		auto itr = shaders.find(type);
		if (itr != shaders.end())
		{
			return itr->second;
		}

		ShaderProgram* shader = nullptr;
		switch (type)
		{
		case Position_Color_Shader:
			shader = new ShaderProgram(srcPosition_Color_vert, srcPosition_Color_frag, 0);
			break;
		case Position_Color_Texture_Shader:
			shader = new ShaderProgram(srcPosition_Color_Texture_vert, srcPosition_Color_Texture_frag, 0);
			break;
		case Position_Texture_Shader:
			shader = new ShaderProgram(srcPosition_Texture_vert, srcPosition_Texture_frag, 0);
			break;
		case Model3d_Shader:
			shader = new ShaderProgram(srcModel3d_Shader_vert, srcModel3d_Shader_frag, 0);
			break;
		case SkyBox_Shader:
			shader = new ShaderProgram(srcSkyBox_Shader_vert, srcSkyBox_Shader_frag, 0);
			break;
		default:
			break;
		}

		shaders[type] = shader;
		return shader;
	}

}