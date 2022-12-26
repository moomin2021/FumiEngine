cbuffer ConstBufferData3D : register(b0)
{
	// --行列-- //
    matrix mat;
}

cbuffer MaterialData : register(b1)
{
    float3 m_ambient : packoffset(c0); // -> アンビエント係数
    float3 m_diffuse : packoffset(c1); // -> ディフェーズ係数
    float3 m_specular : packoffset(c2); // -> スペキュラー係数
    float m_alpha : packoffset(c2.w); // -> アルファ
}

// --頂点シェーダーの出力構造体-- //
// --頂点シェーダーからピクセルシェーダーへのやり取りに使用する-- //
struct VSOutput
{
	// --座標-- //
    float4 svpos : SV_POSITION;

	// --法線-- //
    float3 normal : NORMAL;

	// --uv値-- //
    float2 uv : TEXCOORD;
};