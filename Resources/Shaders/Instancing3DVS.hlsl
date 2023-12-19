#include "Instancing3D.hlsli"

StructuredBuffer<matrix> matrices : register(t1);

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instanceId : SV_InstanceID)
{
	// 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(matrices[instanceId], float4(normal, 0)));
    float4 wpos = mul(matrices[instanceId], pos);
    
    VSOutput output; // ピクセルシェーダーに渡す値
    output.svPos = mul(mul(viewproj, matrices[instanceId]), pos);
    output.worldPos = wpos;
    output.normal = wnormal.xyz;
    output.color = color;
    output.uv = uv;
    
    return output;
}