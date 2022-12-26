cbuffer ConstBufferData3D : register(b0)
{
	// --�s��-- //
    matrix mat;
}

cbuffer MaterialData : register(b1)
{
    float3 m_ambient : packoffset(c0); // -> �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); // -> �f�B�t�F�[�Y�W��
    float3 m_specular : packoffset(c2); // -> �X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); // -> �A���t�@
}

// --���_�V�F�[�_�[�̏o�͍\����-- //
// --���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����-- //
struct VSOutput
{
	// --���W-- //
    float4 svpos : SV_POSITION;

	// --�@��-- //
    float3 normal : NORMAL;

	// --uv�l-- //
    float2 uv : TEXCOORD;
};