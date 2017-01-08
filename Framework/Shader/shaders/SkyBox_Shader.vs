"													\n\
layout (location = 0) in vec3 position;				\n\
													\n\
out vec3 TexCoord;									\n\
													\n\
void main()											\n\
{													\n\
    gl_Position = projection * view * model * vec4(position, 1.0f);			\n\
	TexCoord = position;							\n\
}													\n\
";