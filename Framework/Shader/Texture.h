#ifndef __Texture_H__
#define __Texture_H__

// GLEW
#include <GL/glew.h>

#include <map>;
#include <string>;

namespace opengl_framewordk
{
	//纹理，通过图片文件构造，提供本纹理绑定
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

	//纹理缓存，析构时，释放所有纹理
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