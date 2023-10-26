#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 texcoord3D;
uniform mat4 VPMatrix;

out vec3 texcoord;
out vec3 objectSpaceCoord;
void main()
{
    
    gl_Position = VPMatrix*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texcoord = texcoord3D;
    objectSpaceCoord = aPos;
}