cbuffer ConstBufferData3D : register(b0)
{
    matrix viewproj;// --> ビュープロジェクション行列
    matrix world;// -----> ワールド行列
    float3 cameraPos;// -> カメラ座標(ワールド座標)
}

cbuffer MaterialData : register(b1)
{
    float3 m_ambient : packoffset(c0); // --> アンビエント係数
    float3 m_diffuse : packoffset(c1); // --> ディフェーズ係数
    float3 m_specular : packoffset(c2); // -> スペキュラー係数
    float m_alpha : packoffset(c2.w); // ---> アルファ
}

// --頂点シェーダーの出力構造体-- //
// --頂点シェーダーからピクセルシェーダーへのやり取りに使用する-- //
struct VSOutput
{
    float4 svPos : SV_POSITION;// -> システム用頂点座標
    float4 worldPos : POSITION;// -> ワールド座標
    float3 normal : NORMAL;// -----> 法線
    float2 uv : TEXCOORD;// -------> UV値
};