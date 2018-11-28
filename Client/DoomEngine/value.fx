
//레지스터 변수
cbuffer TransformBuffer : register(b0)
{
    matrix g_matWorld;
    matrix g_matView;
    matrix g_matProj;
};

cbuffer INT : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
};

cbuffer FLOAT : register(b2)
{
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
};

cbuffer VEC4 : register(b3)
{
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
};

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);

SamplerState g_sam_0 : register(s0);

cbuffer ANIM_UV : register(b12)
{
    float4 g_vec4_uv;
    float4 g_vec_anim;
}

Texture2D g_tex_Anim : register(t16);
