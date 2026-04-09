#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 direction;  // 模糊方向 (1.0, 0.0) 或 (0.0, 1.0)
uniform vec2 resolution;

void main()
{
    vec2 texelSize = 1.0 / resolution;
    vec3 result = vec3(0.0);
    
    // 13-tap 高斯权重
    float weights[7] = float[](
        0.204968,   // 中心
        0.180376,   // ±1
        0.120246,   // ±2
        0.060626,   // ±3
        0.023264,   // ±4
        0.006764,   // ±5
        0.001498    // ±6
    );
    
    // 中心采样
    result += texture(texture0, fragTexCoord).rgb * weights[0];
    
    // 对称采样
    for(int i = 1; i < 7; i++)
    {
        vec2 offset = direction * texelSize * float(i) * 2.0;
        result += texture(texture0, fragTexCoord + offset).rgb * weights[i];
        result += texture(texture0, fragTexCoord - offset).rgb * weights[i];
    }
    
    finalColor = vec4(result, 1.0) * fragColor;
}
