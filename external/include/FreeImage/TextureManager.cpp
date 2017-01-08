//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include "TextureManager.h"

TextureManager* TextureManager::m_inst(0);

TextureManager* TextureManager::Inst()
{
	if(!m_inst)
		m_inst = new TextureManager();

	return m_inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
}

//these should never be called
//TextureManager::TextureManager(const TextureManager& tm){}
//TextureManager& TextureManager::operator=(const TextureManager& tm){}
	
TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif

	UnloadAllTextures();
	m_inst = 0;
}

bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	//if this texture ID is in use, unload the current texture
	if(m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return true;
}

GLuint TextureManager::getOpenglTexture(const char* filename, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);

	//OpenGL's image ID to map to
	GLuint gl_texID;

	fif = FreeImage_GetFileType(filename, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	if (fif == FIF_UNKNOWN)
		return 0;

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	if (!dib)
		return 0;

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	if ((bits == 0) || (width == 0) || (height == 0))
		return 0;

	glGenTextures(1, &gl_texID);
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, bits);

	FreeImage_Unload(dib);

	return gl_texID;
}

GLBITMAP* TextureManager::FIBitmap2GLBitmap(FIBITMAP *fibmp)
{
	int i, j, k;
	int pitch = FreeImage_GetPitch(fibmp);
	unsigned char *bits = FreeImage_GetBits(fibmp);
	int bpp = FreeImage_GetBPP(fibmp);
	GLBITMAP *glbmp = (GLBITMAP *)malloc(sizeof(GLBITMAP));
	RGBQUAD *palette = NULL;

	if (!glbmp) return NULL;

	glbmp->w = FreeImage_GetWidth(fibmp);
	glbmp->h = FreeImage_GetHeight(fibmp);

	switch (bpp) {
	case 8:
		if (!(palette = FreeImage_GetPalette(fibmp))) return NULL;
		if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 3))) return NULL;
		glbmp->rgb_mode = GL_RGB;
		for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
				k = bits[i*pitch + j];
				glbmp->buf[(i*glbmp->w + j) * 3 + 0] = palette[k].rgbRed;
				glbmp->buf[(i*glbmp->w + j) * 3 + 1] = palette[k].rgbGreen;
				glbmp->buf[(i*glbmp->w + j) * 3 + 2] = palette[k].rgbBlue;
			}
		}
		break;
	case 24:
		if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 3))) return NULL;
		glbmp->rgb_mode = GL_RGB;
		for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
				glbmp->buf[(i*glbmp->w + j) * 3 + 0] = bits[i*pitch + j * 3 + 2];
				glbmp->buf[(i*glbmp->w + j) * 3 + 1] = bits[i*pitch + j * 3 + 1];
				glbmp->buf[(i*glbmp->w + j) * 3 + 2] = bits[i*pitch + j * 3 + 0];
			}
		}
		break;
	case 32:
		if (!(glbmp->buf = (unsigned char *)malloc(glbmp->w*glbmp->h * 4))) return NULL;
		glbmp->rgb_mode = GL_RGBA;
		for (i = 0; i < glbmp->h; ++i) {
			for (j = 0; j < glbmp->w; ++j) {
				glbmp->buf[(i*glbmp->w + j) * 4 + 0] = bits[i*pitch + j * 4 + 2];
				glbmp->buf[(i*glbmp->w + j) * 4 + 1] = bits[i*pitch + j * 4 + 1];
				glbmp->buf[(i*glbmp->w + j) * 4 + 2] = bits[i*pitch + j * 4 + 0];
				glbmp->buf[(i*glbmp->w + j) * 4 + 3] = bits[i*pitch + j * 4 + 3];
			}
		}
		break;
	default: return NULL;
	}

	return glbmp;
}

void TextureManager::FreeGLBitmap(GLBITMAP *glbmp)
{
	if (glbmp) {
		if (glbmp->buf) free(glbmp->buf);
		free(glbmp);
	}
}

GLuint TextureManager::loadtextureNormal(const char *filename)
{
	GLuint tex = 0;
	int tmp_bit;
	int i;
	int w, h;
	int bpp;
	unsigned char *bits = NULL;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *bitmap = NULL;
	GLBITMAP *glbmp = NULL;

	fif = FreeImage_GetFileType(filename, 0);
	if (FIF_UNKNOWN == fif) {
		fif = FreeImage_GetFIFFromFilename(filename);
		if (FIF_UNKNOWN == fif)
			return 0;
	}
	if (FreeImage_FIFSupportsReading(fif))
		bitmap = FreeImage_Load(fif, filename, 0);

	if (!bitmap)
		return 0;

	//printf("bit: %d\n", FreeImage_GetBPP(bitmap));

	glbmp = FIBitmap2GLBitmap(bitmap);
	if (!glbmp)
		return 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, glbmp->rgb_mode, glbmp->w, glbmp->h, 0, glbmp->rgb_mode, GL_UNSIGNED_BYTE, glbmp->buf);

	FreeGLBitmap(glbmp);
	FreeImage_Unload(bitmap);

	return tex;
}

void TextureManager::loadTextureWithCubeMap(const char *filename, GLuint textureID, int index)
{
	GLuint tex = 0;
	int tmp_bit;
	int i;
	int w, h;
	int bpp;
	unsigned char *bits = NULL;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *bitmap = NULL;
	GLBITMAP *glbmp = NULL;

	fif = FreeImage_GetFileType(filename, 0);
	if (FIF_UNKNOWN == fif) {
		fif = FreeImage_GetFIFFromFilename(filename);
		if (FIF_UNKNOWN == fif)
			return;
	}
	if (FreeImage_FIFSupportsReading(fif))
		bitmap = FreeImage_Load(fif, filename, 0);

	if (!bitmap)
		return;

	//printf("bit: %d\n", FreeImage_GetBPP(bitmap));

	glbmp = FIBitmap2GLBitmap(bitmap);
	if (!glbmp)
		return;

	//textureID外部已制定，它为CUBE_MAP
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, glbmp->rgb_mode, glbmp->w, glbmp->h, 0, glbmp->rgb_mode, GL_UNSIGNED_BYTE, glbmp->buf);

	FreeGLBitmap(glbmp);
	FreeImage_Unload(bitmap);
}

bool TextureManager::UnloadTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_texID.find(texID) != m_texID.end())
	{
		glDeleteTextures(1, &(m_texID[texID]));
		m_texID.erase(texID);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

bool TextureManager::BindTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, bind it's texture as current
	if(m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed
	else
		result = false;

	return result;
}

void TextureManager::UnloadAllTextures()
{
	//start at the begginning of the texture map
	std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

	//Unload the textures untill the end of the texture map is found
	while(i != m_texID.end())
		UnloadTexture(i->first);

	//clear the texture map
	m_texID.clear();
}