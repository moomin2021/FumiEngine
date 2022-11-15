cbuffer ConstBufferData3D : register(b0)
{
    // --�F�iRBGA�j-- //
    float4 color;

	// --�s��-- //
    matrix mat;
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