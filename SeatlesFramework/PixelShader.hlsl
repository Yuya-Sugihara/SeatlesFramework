#include "BasicType.hlsli"

float4 pixelShader(Output input) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv));
}