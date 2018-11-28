#include "value.fx"


struct ColorPS_OUTPUT
{
	float4 vtxCol : SV_Target;
};

//==============//
// Color Shader //
//==============//
struct ColorVtx_INPUT
{
	float3 vtxPos : POSITION;
	float4 vtxCol : COLOR;
};

struct ColorVtx_OUTPUT
{
	float4 vtxPos : SV_POSITION;
	float4 vtxCol : COLOR;
};

ColorVtx_OUTPUT VS_ColorVtx(ColorVtx_INPUT _input)
{
	ColorVtx_OUTPUT output = (ColorVtx_OUTPUT) 0.f;

	float4 vWorldPos = mul(float4(_input.vtxPos, 1.f), g_matWorld);
	float4 vViewPos = mul(vWorldPos, g_matView);
	float4 vProjPos = mul(vViewPos, g_matProj);

	output.vtxPos = vProjPos;
	output.vtxCol = _input.vtxCol;

	return output;
}

ColorPS_OUTPUT PS_ColorVtx(ColorVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	output.vtxCol = _input.vtxCol;
	return output;
}


//=============
// Texture Shader
//=============


struct TexVtx_INPUT
{
	float3 vtxPos : POSITION;
	float2 vtxUV : TEXCOORD;
};

struct TexVtx_OUTPUT
{
	float4 vtxPos : SV_POSITION;
	float2 vtxUV : TEXCOORD;
};



TexVtx_OUTPUT VS_TexVtx(TexVtx_INPUT _input)
{
	TexVtx_OUTPUT output = (TexVtx_OUTPUT) 0.f;

	float4 vWorldPos = mul(float4(_input.vtxPos, 1.f), g_matWorld);
	float4 vViewPos = mul(vWorldPos, g_matView);
	float4 vProjPos = mul(vViewPos, g_matProj);

	output.vtxPos = vProjPos;
	output.vtxUV = _input.vtxUV;

	return output;
}

ColorPS_OUTPUT PS_TexVtx(TexVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	float4 vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);

    if(g_int_0)
    {
        vCol.rgb = (vCol.r + vCol.g + vCol.b) / 3.f;
    }

    output.vtxCol = vCol;

	return output;
}

/////////////////////
// Collider Shader

float4 VS_Collider(float3 _vPos : POSITION) : SV_POSITION
{
    float4 vWorldPos = mul(float4(_vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
    return vProjPos;
}

ColorPS_OUTPUT PS_Collider(float4 _vPos : SV_POSITION)
{
    
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;
    if(g_int_0)
        output.vtxCol = float4(1.f, 0.f, 0.f, 1.f);

    else
        output.vtxCol = float4(0.f, 1.f, 0.f, 1.f);

    return output;
}

//Standard 2D shader
///////////////////////

TexVtx_OUTPUT VS_Std_2D(TexVtx_INPUT _input)
{
    TexVtx_OUTPUT output = (TexVtx_OUTPUT) 0.f;

    float4 vWorldPos = mul(float4(_input.vtxPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);

    output.vtxPos = vProjPos;
    output.vtxUV = _input.vtxUV;

    return output;
}

ColorPS_OUTPUT PS_Std_2D(TexVtx_OUTPUT _input)
{
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

    float4 vCol = (float4) 0.f;

    if (g_vec_anim.x)
    {
        vCol = g_tex_Anim.Sample(g_sam_0
        , float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
        , g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
    }

    output.vtxCol = vCol;

    if (0.f == output.vtxCol.a)
        clip(-1);

    return output;
}

//=============
//			UI
//=============

//=============
//			UIBar
//=============
TexVtx_OUTPUT VS_UIBar(TexVtx_INPUT _input)
{
	TexVtx_OUTPUT output = (TexVtx_OUTPUT) 0.f;

	float4 vWorldPos = mul(float4(_input.vtxPos, 1.f), g_matWorld);
	float4 vViewPos = mul(vWorldPos, g_matView);
	float4 vProjPos = mul(vViewPos, g_matProj);

	output.vtxPos = vProjPos;
	output.vtxUV = _input.vtxUV;

	return output;
}

ColorPS_OUTPUT PS_UIBar(TexVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	float4 vCol = g_tex_1.Sample(g_sam_0, _input.vtxUV);

	/*g_vec4_0;
	g_vec4_1;
	g_vec4_2;
	*/
	if (g_int_0)//°¡·ÎÀÓ?
	{
		if (_input.vtxUV.y > g_float_0)
		{
			vCol.rgb /= 2.f;
			vCol.a = 1.f;
		}
		else if (_input.vtxUV.y > g_float_1)
		{
			vCol.r *= 1.5f;
			vCol.g /= 4.f;
			vCol.b /= 4.f;
		}
		else
		{
			vCol.r /= 3.f;
			vCol.g *= 1.2f;
			vCol.b *= (1 - _input.vtxUV.y) / 0.8f;
		}
	}
	else
	{
		if (_input.vtxUV.x < g_float_0)
		{
			vCol *= g_vec4_0;
			//vCol.rgb *= 0.5f;
			//vCol.a = 1.f;
		}
		else if (_input.vtxUV.x < g_float_1)
		{
			vCol *= g_vec4_1;
			//vCol.r *= 1.5f;
			//vCol.g *= 0.5f;
			//vCol.b *= 0.5f;
		}
		else
		{
			vCol.r *= g_vec4_2.r;
			vCol.g *= g_vec4_2.g;
			vCol.b *= (_input.vtxUV.x) * g_vec4_2.b;
		}
	}

	output.vtxCol = vCol;

	return output;
}

//=============
//			UIFont
//=============
TexVtx_OUTPUT VS_UIFont(TexVtx_INPUT _input)
{
	TexVtx_OUTPUT output = (TexVtx_OUTPUT) 0.f;

	float4 vWorldPos = mul(float4(_input.vtxPos, 1.f), g_matWorld);
	float4 vViewPos = mul(vWorldPos, g_matView);
	float4 vProjPos = mul(vViewPos, g_matProj);

	output.vtxPos = vProjPos;
	output.vtxUV = _input.vtxUV;

	return output;
}

ColorPS_OUTPUT PS_UIFont(TexVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	float2 vUV = (float2)0.f;

	vUV.x = g_vec4_0.x + (g_vec4_0.z*_input.vtxUV.x);
	vUV.y = g_vec4_0.y + (g_vec4_0.w*_input.vtxUV.y);

	float4 vCol = g_tex_0.Sample(g_sam_0, vUV);

	output.vtxCol = vCol;

	return output;
}

//=========================
//			PS GRAY Shader
//=========================

ColorPS_OUTPUT PS_Std_2D_Gray(TexVtx_OUTPUT _input)
{
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

    float4 vCol = (float4) 0.f;

    if (g_vec_anim.x)
    {
        vCol = g_tex_Anim.Sample(g_sam_0
        , float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
        , g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
    } 
    vCol.rgb = (vCol.r + vCol.g + vCol.b) / 3.f;

    output.vtxCol = vCol;

    if (0.f == output.vtxCol.a)
        clip(-1);

    return output;
}

//============
//			RedStd2d
//============
ColorPS_OUTPUT PS_Std_2D_Red(TexVtx_OUTPUT _input)
{
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

    float4 vCol = (float4) 0.f;

    if (g_vec_anim.x)
    {
        vCol = g_tex_Anim.Sample(g_sam_0
        , float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
        , g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
    }
    
    vCol.r = 1;
    vCol.b = 0;
   
     if (vCol.g > 0 && vCol.g < 0.15f)
        vCol.g = 0;
    else
        vCol.g = 1;

    output.vtxCol = vCol;

    if (0.f == output.vtxCol.a)
        clip(-1);

    return output;
}

//============
//			BlackStd2d
//============

ColorPS_OUTPUT PS_Std_2D_Black(TexVtx_OUTPUT _input)
{
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

    float4 vCol = (float4) 0.f;

    if (g_vec_anim.x)
    {
        vCol = g_tex_Anim.Sample(g_sam_0
        , float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
        , g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
    }
    
    vCol.rgb = 0;
    
    if(vCol.a != 0)
    {
        vCol.a = 0.7;
    }

    output.vtxCol = vCol;

    if (0.f == output.vtxCol.a)
        clip(-1);

    return output;
}

//============
//			BlueStd2d
//============

ColorPS_OUTPUT PS_Std_2D_Blue(TexVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	float4 vCol = (float4) 0.f;

	if (g_vec_anim.x)
	{
		vCol = g_tex_Anim.Sample(g_sam_0
			, float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
				, g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
	}
	else
	{
		vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
	}

	vCol.b =1.f;
	vCol.r = 0.5f;

	if (vCol.g > 0 && vCol.g < 0.25f)
		vCol.g = 0.f;
	else if (vCol.g > 0.25f&&vCol.g < 0.5f)
	{
		vCol.g = 0.3f;
		vCol.r = 0.5f;
	}
	else
	{
		vCol.g = 1.f;
		vCol.r = 0.75f;
	}
	output.vtxCol = vCol;

    if (0.f == output.vtxCol.a)
        clip(-1);

	return output;
}

ColorPS_OUTPUT PS_Std_2D_Effect(TexVtx_OUTPUT _input)
{
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

    float4 vCol = (float4) 0.f;

    if (g_vec_anim.x)
    {
        vCol = g_tex_Anim.Sample(g_sam_0
        , float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
        , g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
    }
    output.vtxCol = vCol;

    if (0.f == output.vtxCol.a)
        clip(-1);

    output.vtxCol.a = (vCol.r + vCol.g + vCol.b) / 2;

    return output;
}

ColorPS_OUTPUT PS_Std_2D_Reverse(TexVtx_OUTPUT _input)
{
    ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

    float4 vCol = (float4) 0.f;

    if (g_vec_anim.x)
    {
        vCol = g_tex_Anim.Sample(g_sam_0
        , float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
        , g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
    }
    else
    {
        vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
    }

    output.vtxCol.r = 1 - vCol.r;
    output.vtxCol.g = 1 - vCol.g;
    output.vtxCol.b = 1 - vCol.b;
    output.vtxCol.a = vCol.a;

    if (0.f == output.vtxCol.a)
        clip(-1);

    return output;
}

ColorPS_OUTPUT PS_Std_2D_AllRed(TexVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	float4 vCol = (float4) 0.f;

	if (g_vec_anim.x)
	{
		vCol = g_tex_Anim.Sample(g_sam_0
			, float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
				, g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
	}
	else
	{
		vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
	}

	output.vtxCol.r = vCol.r*2.f;
	output.vtxCol.g = vCol.g*0.5f;
	output.vtxCol.b = vCol.b*0.5f;
	output.vtxCol.a = vCol.a;

	if (0.f == output.vtxCol.a)
		clip(-1);

	return output;
}

ColorPS_OUTPUT PS_Std_2D_AllBlack(TexVtx_OUTPUT _input)
{
	ColorPS_OUTPUT output = (ColorPS_OUTPUT) 0.f;

	float4 vCol = (float4) 0.f;

	if (g_vec_anim.x)
	{
		vCol = g_tex_Anim.Sample(g_sam_0
			, float2(g_vec4_uv.x + (g_vec4_uv.z * _input.vtxUV.x)
				, g_vec4_uv.y + (g_vec4_uv.w * _input.vtxUV.y)));
	}
	else
	{
		vCol = g_tex_0.Sample(g_sam_0, _input.vtxUV);
	}

	vCol.rgb = 0.f;
	vCol.a = 1.f;
	output.vtxCol = vCol;

	if (0.f == output.vtxCol.a)
		clip(-1);

	return output;
}