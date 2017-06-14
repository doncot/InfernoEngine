struct PIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

Texture2D textureDiffuse : register(t0);
SamplerState sampleLinear : register(s0);

float4 main(PIn input) : SV_TARGET
{
	float4 color = textureDiffuse.Sample(sampleLinear, input.texCoord);

	return float4(1.0 - color.rgb, 1.0);
}