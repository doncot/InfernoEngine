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

//�ϊ��p�s��
cbuffer ConstantBuffer : register(b0)
{
	matrix World;		//���[���h�ϊ��s��
	matrix View;		//�r���[�ϊ��s��
	matrix Projection;	//�����ˉe�ϊ��s��
}

VOut VShader(VIn input)
{
	VOut output;

	//���W�ϊ�
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