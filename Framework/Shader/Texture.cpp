#include "Texture.h"
#include "FreeImage/TextureManager.h"

using namespace std;

namespace opengl_framewordk
{

	Texture::Texture()
		:textureID(0)
	{
		//
	}

	Texture::~Texture()
	{
		if (textureID != 0)
		{
			glDeleteTextures(1, &textureID);
		}
	}

	Texture* Texture::createWithFile(const char* imgFile)
	{
		auto tx = new Texture();
		if (tx->initWithFile(imgFile))
		{
			return tx;
		}

		delete tx;
		return nullptr;
	}

	bool Texture::initWithFile(const char* imgFile)
	{
		textureID = TextureManager::Inst()->loadtextureNormal(imgFile);

		if (textureID != 0)
		{
			// Set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);// loadtextureNormal÷–“—∞Û∂®

			return true;
		}

		return false;
	}


	TextureCache* TextureCache::instance = nullptr;
	TextureCache::TextureCache()
		:textures()
	{

	}

	TextureCache::~TextureCache()
	{
		for (map<string, Texture*>::iterator itr = textures.begin(); itr != textures.end(); itr++)
		{
			delete itr->second;
		}
		textures.clear();
	}

	TextureCache* TextureCache::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new TextureCache();
		}

		return instance;
	}

	void TextureCache::destoryInstance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

	Texture* TextureCache::getTextureFromFile(const char* imgFile)
	{
		auto tx = textures.find(imgFile);
		if (tx != textures.end())
		{
			return tx->second;
		}

		auto texture = Texture::createWithFile(imgFile);
		if (texture != nullptr)
		{
			textures.insert(make_pair(imgFile, texture));
		}

		return texture;
	}

}