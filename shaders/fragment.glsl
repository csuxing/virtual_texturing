#version 330 core
in vec2 TexCoord;

uniform isampler2D pageTable;
uniform sampler2D physicalTexture;

out vec4 FragColor;

void main() {
    // ��ȡҳ����Ϣ
    ivec4 pageInfo = texture(pageTable, TexCoord);
    
    if (pageInfo.w == 1) {
        // ����������������
        vec2 physicalPos = vec2(pageInfo.xy);
        vec2 localPos = fract(TexCoord * textureSize(pageTable, 0));
        vec2 finalCoord = (physicalPos + localPos) / textureSize(physicalTexture, 0);
        
        // ������������
        FragColor = texture(physicalTexture, finalCoord);
    } else {
        // ҳ��δ���أ���ʾռλɫ
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
}