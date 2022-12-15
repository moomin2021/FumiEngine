#include "Model.hlsli"

Texture2D<float4> tex : register(t0); // -> 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // ------> 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = float4(tex.Sample(smp, input.uv));

	// --�E�����@�����̃��C�g
    float3 light = normalize(float3(1, -1, 1));

	// --diffuce(�g�U���ˌ�)��[0, 1]�͈̔͂�Clamp����
    float diffuse = saturate(dot(-light, input.normal));

	// �A���r�G���g����0.3�Ƃ��Čv�Z
    float brightness = diffuse + 0.3f;

	// --�P�x��RGB�ɑ�����ďo��
    return float4(texcolor.rgb * brightness, texcolor.a) * float4(1.0f, 1.0f, 1.0f, 1.0f);
}

//float4 main(VSOutput input) : SV_TARGET
//{
//    float3 light = normalize(float3(1, -1, 1));// -> �E�����A�����̃��C�g
//    float light_diffuse = saturate(dot(-light, input.normal));
//    float3 shade_color;
//    shade_color = m_ambient;// -> �A���r�G���g��
//    shade_color += m_diffuse * light_diffuse;// -> �f�B�t�F�[�Y��
//    float4 texcolor = tex.Sample(smp, input.uv);
//    return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//}