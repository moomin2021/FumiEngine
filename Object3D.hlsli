cbuffer ConstBufferData3D : register(b0)
{
    matrix viewproj;// --> �r���[�v���W�F�N�V�����s��
    matrix world;// -----> ���[���h�s��
    float3 cameraPos;// -> �J�������W(���[���h���W)
}

cbuffer MaterialData : register(b1)
{
    float3 m_ambient : packoffset(c0); // --> �A���r�G���g�W��
    float3 m_diffuse : packoffset(c1); // --> �f�B�t�F�[�Y�W��
    float3 m_specular : packoffset(c2); // -> �X�y�L�����[�W��
    float m_alpha : packoffset(c2.w); // ---> �A���t�@
}

// --���_�V�F�[�_�[�̏o�͍\����-- //
// --���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����-- //
struct VSOutput
{
    float4 svPos : SV_POSITION;// -> �V�X�e���p���_���W
    float4 worldPos : POSITION;// -> ���[���h���W
    float3 normal : NORMAL;// -----> �@��
    float2 uv : TEXCOORD;// -------> UV�l
};