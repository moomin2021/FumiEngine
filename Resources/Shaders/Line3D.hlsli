cbuffer cbuff0 : register(b0)
{
    matrix mat;
}

struct VSOutput
{
    float4 svPos : SV_POSITION;
};

struct GSOutput
{
    float4 svPos : SV_POSITION;
};