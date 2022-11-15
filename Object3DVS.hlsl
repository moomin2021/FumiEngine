#include "Object3D.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// ピクセルシェーダーに渡す値
    VSOutput output;
	
    output.svpos = mul(mat, pos);
    output.normal = mul(mat, normal);
    output.uv = uv;
    return output;
}