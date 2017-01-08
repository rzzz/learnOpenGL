"										\n\
in vec2 TexCoords;						\n\
										\n\
uniform sampler2D texture_diffuse1;		\n\
										\n\
out vec4 color;							\n\
										\n\
void main()								\n\
{										\n\
    color = vec4(texture(texture_diffuse1, TexCoords));		\n\
}										\n\
";