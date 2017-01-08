"												\n\
layout (location = 0) in vec3 position;			\n\
layout (location = 1) in vec3 color;			\n\
												\n\
out vec3 ourColor;								\n\
												\n\
void main()										\n\
{												\n\
    gl_Position = projection * view * model * vec4(position, 1.0f);			\n\
    ourColor = color;							\n\
}												\n\
";