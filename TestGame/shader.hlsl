struct VIn
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

//変換用行列
cbuffer ConstantBuffer : register(b0)
{
	matrix World;		//ワールド変換行列
	matrix View;		//ビュー変換行列
	matrix Projection;	//透視射影変換行列
}

VOut VShader(VIn input)
{
	VOut output;

	//座標変換
	float4 worldPosition = mul(input.position, World);
	float4 viewPosition = mul(worldPosition, View);
	output.position = mul(viewPosition, Projection);
	output.color = input.color;
	output.texCoord = input.texCoord;

	return output;
}

//-------------------------------

struct PIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

Texture2D textureDiffuse : register(t0);
SamplerState sampleLinear : register(s0);

float4 PShader(PIn input) : SV_TARGET
{
	float4 color = textureDiffuse.Sample(sampleLinear, input.texCoord);

	//return float4(1.0 - color.rgb, 1.0);
	//return color;
	return input.color;
}