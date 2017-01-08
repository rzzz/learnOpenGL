#ifndef __SHADER_H__
#define __SHADER_H__

// GLEW
#include <GL/glew.h>

#include <string> 
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace opengl_framewordk
{
	//着色器程序：从字符串构造，完成着色器编译；提供纹理传值，矩阵传值，统一MVP传递
	class ShaderProgram
	{
	public:
		// 构造器读取并创建Shader
		ShaderProgram(const std::string &vertexSourcePath, const std::string &fragmentSourcePath);
		ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource, int nouser);
		~ShaderProgram();

	public:
		void use();
		GLuint getShaderID(){ return programID; };

		void setUniformTexture(const char* locationName, GLenum textureUnit);
		void setUniformMatrix(const char* locationName, float* matrix);

		GLint getUniformLocation(const char* locationName){ return glGetUniformLocation(programID, locationName); }

		void updateMVP(glm::mat4 &model, bool forceSetSkyBox=false);

	private:
		void initWithSorce(const char* vertexShaderSource, const char* fragmentShaderSource);
		void updateBultinProgram();

	private:
		// 程序ID 
		GLuint programID;
	};

	//着色器缓存，管理着色器程序，析构时清理所有着色器程序
	class ShaderProgramCache
	{
	public:
		enum
		{
			Position_Color_Shader,
			Position_Color_Texture_Shader,
			Position_Texture_Shader,
			Model3d_Shader,
			SkyBox_Shader,
		};
	private:
		ShaderProgramCache();
		~ShaderProgramCache();

		static ShaderProgramCache* instance;

	public:
		static ShaderProgramCache* getInstance();
		static void destroyInstacne();

	public:
		void init();

		ShaderProgram* getShaderProgram(int type);

	private:
		std::map<int, ShaderProgram*> shaders;
	};

}

#endif