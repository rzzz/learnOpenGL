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
	//��ɫ�����򣺴��ַ������죬�����ɫ�����룻�ṩ����ֵ������ֵ��ͳһMVP����
	class ShaderProgram
	{
	public:
		// ��������ȡ������Shader
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
		// ����ID 
		GLuint programID;
	};

	//��ɫ�����棬������ɫ����������ʱ����������ɫ������
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