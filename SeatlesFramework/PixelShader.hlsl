#include "BasicType.hlsli"

float4 pixelShader(Output input) : SV_TARGET
{
	return float4(input.uv,1,1);
}