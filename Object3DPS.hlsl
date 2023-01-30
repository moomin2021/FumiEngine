#include "Object3D.hlsli"

Texture2D<float4> tex : register(t0); // -> 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // ------> 0番スロットに設定されたサンプラー

//float4 main(VSOutput input) : SV_TARGET
//{
//    float4 texcolor = float4(tex.Sample(smp, input.uv));

//	// --右下億　向きのライト
//    float3 light = normalize(float3(1, -1, 1));

//	// --diffuce(拡散反射光)を[0, 1]の範囲にClampする
//    float diffuse = saturate(dot(-light, input.normal));

//	// アンビエント項を0.3として計算
//    float brightness = diffuse + 0.3f;

//	// --輝度をRGBに代入して出力
//    return float4(texcolor.rgb * brightness, texcolor.a) * float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

float4 main(VSOutput input) : SV_TARGET
{
    // テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
    
    // シェーディングによる色
    float4 shadecolor;
    
    // 光沢度
    const float shininess = 4.0f;
    
    // 頂点から視点へのベクトル
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
    
    float3 lightv = { 1.0f, -1.0f, 1.0f };
    float3 lightcolor = { 1.0f, 1.0f, 1.0f };
    
    // ライトに向かうベクトルと法線の内積
    float3 dotlightnormal = dot(lightv, input.normal);
    
    // 反射光ベクトル
    float3 reflect = normalize(-lightv + 2.0f * dotlightnormal * input.normal);

    // 環境反射光
    float3 ambient = m_ambient;
    
    // 拡散反射光
    float3 diffuse = dotlightnormal * m_diffuse;
    
    // 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
    
    // 全て加算する
    shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
    shadecolor.a = m_alpha;
    
    // シェーディングによる色で描画
    return shadecolor * texcolor;
}