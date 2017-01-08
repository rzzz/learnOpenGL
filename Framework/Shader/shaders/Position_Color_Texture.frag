"										\n\
in vec3 ourColor;						\n\
in vec2 TexCoord;						\n\
										\n\
uniform vec3 lightColor;				\n\
uniform sampler2D ourTexture;			\n\
										\n\
out vec4 color;							\n\
										\n\
void main()								\n\
{										\n\
	vec4 texColor = texture(ourTexture, TexCoord);			\n\
    color = texColor * vec4(ourColor * lightColor, 1.0f);	\n\
}										\n\
";