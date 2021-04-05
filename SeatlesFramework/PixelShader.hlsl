
struct Input
{
	float4 pos:POSITION;
	float4 svpos:SV_POSITION;
};

float4 pixelShader(Input input) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	return float4((float2(1,1) + input.pos.xy * 0.5f),1,1);
}