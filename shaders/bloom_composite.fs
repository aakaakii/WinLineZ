#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;      // 原始场景
uniform sampler2D texture1;      // 模糊后的亮部
uniform float bloomIntensity;

void main()
{
    vec3 sceneColor = texture(texture0, fragTexCoord).rgb;
    vec3 bloomColor = texture(texture1, fragTexCoord).rgb;
    
    // 线性混合
    vec3 color = sceneColor + bloomColor * bloomIntensity;
    
    // ACES 色调映射
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    color = (color * (a * color + b)) / (color * (c * color + d) + e);
    
    // 简单的 gamma 校正
    color = pow(color, vec3(1.0 / 2.2));
    
    finalColor = vec4(color, 1.0) * fragColor;
}
