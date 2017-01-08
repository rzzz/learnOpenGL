#include "Shader.h"

namespace opengl_framewordk
{
	const char * srcPosition_Color_vert =
#include "Position_Color.vs"
	const char * srcPosition_Color_frag =
#include "Position_Color.frag"

	const char * srcPosition_Color_Texture_vert =
#include "Position_Color_Texture.vs"
	const char * srcPosition_Color_Texture_frag =
#include "Position_Color_Texture.frag"

	const char * srcPosition_Texture_vert =
#include "Position_Texture.vs"
	const char * srcPosition_Texture_frag =
#include "Position_Texture.frag"

	const char * srcModel3d_Shader_vert =
#include "Model3d_Shader.vs"
	const char * srcModel3d_Shader_frag =
#include "Model3d_Shader.frag"

	const char * srcSkyBox_Shader_vert =
#include "SkyBox_Shader.vs"
	const char * srcSkyBox_Shader_frag =
#include "SkyBox_Shader.frag"

}