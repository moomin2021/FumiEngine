#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION)
{
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mat, pos);
    return output;
}