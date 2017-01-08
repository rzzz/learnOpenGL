"										\n\
in vec3 TexCoord;						\n\
										\n\
uniform samplerCube skybox;				\n\
										\n\
out vec4 color;							\n\
										\n\
void main()								\n\
{										\n\
    color = texture(skybox, TexCoord);	\n\
}										\n\
";