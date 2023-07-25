cbuffer ConstBufferData : register(b0)
{
    // --行列-- //
    matrix mat;
    
	// --色（RBGA）-- //
    float4 color;
    
    float t;
}

struct VSOutput
{
    float4 svPos : SV_POSITION;
    float2 uv : TEXCOORD;
};