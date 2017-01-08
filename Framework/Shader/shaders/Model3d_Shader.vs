"												\n\
layout (location = 0) in vec3 position;			\n\
layout (location = 1) in vec3 normal;			\n\
layout (location = 2) in vec2 texCoords;		\n\
												\n\
out vec3 TexCoords;								\n\
												\n\
void main()										\n\
{												\n\
    gl_Position = projection * view * model * vec4(position, 1.0f);			\n\
    TexCoords = texCoords;							\n\
}												\n\
";