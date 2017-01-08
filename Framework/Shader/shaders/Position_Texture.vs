"													\n\
layout (location = 0) in vec3 position;				\n\
layout (location = 2) in vec2 texCoord;				\n\
													\n\
out vec2 TexCoord;									\n\
													\n\
void main()											\n\
{													\n\
    gl_Position = projection * view * model * vec4(position, 1.0f);			\n\
	TexCoord = vec2(texCoord.x, texCoord.y);		\n\
}													\n\
";