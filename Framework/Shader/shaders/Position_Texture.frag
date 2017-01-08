"										\n\
in vec2 TexCoord;						\n\
										\n\
uniform vec3 lightColor;				\n\
uniform sampler2D ourTexture;			\n\
										\n\
out vec4 color;							\n\
										\n\
void main()								\n\
{										\n\
    color = texture(ourTexture, TexCoord)* vec4(lightColor, 1.0f);	\n\
}										\n\
";