//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#ifndef TextureManager_H
#define TextureManager_H

#include <GL/glew.h>

#include <windows.h>
#include <gl/gl.h>

#include "FreeImage.h"
#include <map>

typedef struct
{
	int				w;
	int             h;
	unsigned char   *buf;
	GLuint          rgb_mode;
}GLBITMAP;

class TextureManager
{
public:
	static TextureManager* Inst();
	virtual ~TextureManager();

	//load a texture an make it the current texture
	//if texID is already in use, it will be unloaded and replaced with this texture
	bool LoadTexture(const char* filename,	//where to load the file from
		const unsigned int texID,			//arbitrary id you will reference the texture by
											//does not have to be generated with glGenTextures
		GLenum image_format = GL_RGB,		//format the image is in
		GLint internal_format = GL_RGB,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);					//border size

	//单纯创建纹理，不缓存
	GLuint getOpenglTexture(const char* filename,
		GLenum image_format = GL_RGB,
		GLint internal_format = GL_RGB,
		GLint level = 0,
		GLint border = 0);

	//加载纹理可能会有颜色错误，使用此方式自动识别，自动加载
	GLuint loadtextureNormal(const char *filename);
	GLBITMAP* FIBitmap2GLBitmap(FIBITMAP *fibmp);
	void FreeGLBitmap(GLBITMAP *glbmp);
	void loadTextureWithCubeMap(const char *filename, GLuint textureID, int index);

	//free the memory for a texture
	bool UnloadTexture(const unsigned int texID);

	//set the current texture
	bool BindTexture(const unsigned int texID);

	//free all texture memory
	void UnloadAllTextures();

protected:
	TextureManager();
	TextureManager(const TextureManager& tm);
	TextureManager& operator=(const TextureManager& tm);

	static TextureManager* m_inst;
	std::map<unsigned int, GLuint> m_texID;
};

#endif