cbuffer ConstBufferData : register(b0)
{
    // --�s��-- //
    matrix mat;
    
	// --�F�iRBGA�j-- //
    float4 color;
    
    float t;
}

struct VSOutput
{
    float4 svPos : SV_POSITION;
    float2 uv : TEXCOORD;
};