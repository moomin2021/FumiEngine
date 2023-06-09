cbuffer ConstBufferData : register(b0)
{
    // --行列-- //
    matrix mat;
    
	// --色（RBGA）-- //
    float4 color;
}

// --頂点ッシェーダーの出力構造体-- //
struct VSOutput
{
	// --システム用頂点座標-- //
    float4 svpos : SV_POSITION;

	// --uv値-- //
    float2 uv : TEXCOORD;
};