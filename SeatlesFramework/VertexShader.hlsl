#include "BasicType.hlsli"

Output vertexShader(float4 pos : POSITION,float2 uv:TEXCOORD)
{
	Output output;
	//output.svpos = mul(mat, pos);
	output.svpos = pos;
	output.uv = uv;

	return output;
}