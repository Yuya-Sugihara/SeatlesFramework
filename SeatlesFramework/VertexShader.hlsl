
struct Output
{
	float4 pos:POSITION;
	float4 svpos:SV_POSITION;
};

Output vertexShader(float4 pos : POSITION)
{
	//return pos;
	Output output;
	output.pos = pos;
	output.svpos = pos;
	return output;
}