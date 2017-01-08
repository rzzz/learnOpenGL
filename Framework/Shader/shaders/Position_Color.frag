"										\n\
in vec3 ourColor;						\n\
										\n\
uniform vec3 lightColor;				\n\
										\n\
out vec4 color;							\n\
										\n\
void main()								\n\
{										\n\
    color = vec4(ourColor * lightColor, 1.0f);		\n\
}										\n\
";