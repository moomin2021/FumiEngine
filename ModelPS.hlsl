#include "Model.hlsli"

Texture2D<float4> tex : register(t0); // -> 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // ------> 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = float4(tex.Sample(smp, input.uv));

	// --右下億　向きのライト
    float3 light = normalize(float3(1, -1, 1));

	// --diffuce(拡散反射光)を[0, 1]の範囲にClampする
    float diffuse = saturate(dot(-light, input.normal));

	// アンビエント項を0.3として計算
    float brightness = diffuse + 0.3f;

	// --輝度をRGBに代入して出力
    return float4(texcolor.rgb * brightness, texcolor.a) * float4(1.0f, 1.0f, 1.0f, 1.0f);
}

//float4 main(VSOutput input) : SV_TARGET
//{
//    float3 light = normalize(float3(1, -1, 1));// -> 右下奥、向きのライト
//    float light_diffuse = saturate(dot(-light, input.normal));
//    float3 shade_color;
//    shade_color = m_ambient;// -> アンビエント項
//    shade_color += m_diffuse * light_diffuse;// -> ディフェーズ項
//    float4 texcolor = tex.Sample(smp, input.uv);
//    return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//}