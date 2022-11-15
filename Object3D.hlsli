cbuffer ConstBufferData3D : register(b0)
{
    // --色（RBGA）-- //
    float4 color;

	// --行列-- //
    matrix mat;
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