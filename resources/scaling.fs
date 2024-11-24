#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float textureScale;

out vec4 finalColor;

void main()
{
    vec2 scaledTexCoord = fragTexCoord * textureScale;
    vec4 texelColor = texture(texture0, scaledTexCoord);
    finalColor = texelColor * fragColor;
}
