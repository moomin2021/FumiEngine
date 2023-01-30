#include "Object3D.hlsli"

Texture2D<float4> tex : register(t0); // -> 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // ------> 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//float4 main(VSOutput input) : SV_TARGET
//{
//    float4 texcolor = float4(tex.Sample(smp, input.uv));

//	// --�E�����@�����̃��C�g
//    float3 light = normalize(float3(1, -1, 1));

//	// --diffuce(�g�U���ˌ�)��[0, 1]�͈̔͂�Clamp����
//    float diffuse = saturate(dot(-light, input.normal));

//	// �A���r�G���g����0.3�Ƃ��Čv�Z
//    float brightness = diffuse + 0.3f;

//	// --�P�x��RGB�ɑ�����ďo��
//    return float4(texcolor.rgb * brightness, texcolor.a) * float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

float4 main(VSOutput input) : SV_TARGET
{
    // �e�N�X�`���}�b�s���O
    float4 texcolor = tex.Sample(smp, input.uv);
    
    // �V�F�[�f�B���O�ɂ��F
    float4 shadecolor;
    
    // ����x
    const float shininess = 4.0f;
    
    // ���_���王�_�ւ̃x�N�g��
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    
    float3 lightv = { 1.0f, -1.0f, 1.0f };
    float3 lightcolor = { 1.0f, 1.0f, 1.0f };
    
    // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
    float3 dotlightnormal = dot(lightv, input.normal);
    
    // ���ˌ��x�N�g��
    float3 reflect = normalize(-lightv + 2.0f * dotlightnormal * input.normal);

    // �����ˌ�
    float3 ambient = m_ambient;
    
    // �g�U���ˌ�
    float3 diffuse = dotlightnormal * m_diffuse;
    
    // ���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
    
    // �S�ĉ��Z����
    shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
    shadecolor.a = m_alpha;
    
    // �V�F�[�f�B���O�ɂ��F�ŕ`��
    return shadecolor * texcolor;
}