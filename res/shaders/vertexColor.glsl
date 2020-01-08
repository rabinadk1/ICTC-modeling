#shader vertex
#version 430 core
layout (location = 0) in vec4 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = aPos;
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}

#shader fragment
#version 430 core
out vec4 FragColor;
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
