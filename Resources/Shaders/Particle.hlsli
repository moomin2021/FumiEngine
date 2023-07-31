cbuffer cbuff0 : register(b0)
{
    matrix mat;
}

// ���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
    float4 svpos    : SV_POSITION;  // �V�X�e���p���_���W
    float3 normal   : NORMAL;       // �@���x�N�g��
    float2 uv       : TEXCOORD;     // uv�l
};

// �W�I���g���V�F�[�_����s�N�Z���V�F�[�_�ւ̏o��
struct GSOutput
{
    float4 svpos    : SV_POSITION;  // �V�X�e���p���_���W
    float3 normal   : NORMAL;       // �@���x�N�g��
    float2 uv       : TEXCOORD;     // uv�l
};