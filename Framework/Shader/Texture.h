#ifndef __Texture_H__
#define __Texture_H__

// GLEW
#include <GL/glew.h>

#include <map>;
#include <string>;

namespace opengl_framewordk
{
	//����ͨ��ͼƬ�ļ����죬�ṩ�������
	class Texture
	{
	public:
		Texture();
		~Texture();

		static Texture* createWithFile(const char* imgFile);

	public:
		void bindTexture(){ glBindTexture(GL_TEXTURE_2D, textureID); }
		GLuint getTextureID(){ return textureID; }

	private:
		bool initWithFile(const char* imgFile);

	private:
		GLuint textureID;
	};

	//�����棬����ʱ���ͷ���������
	class TextureCache
	{
	private:
		TextureCache();
		~TextureCache();

		static TextureCache* instance;

	public:
		static TextureCache* getInstance();
		static void destoryInstance();

		Texture* getTextureFromFile(const char* imgFile);

	private:
		std::map<std::string, Texture*> textures;
	};
}

#endif