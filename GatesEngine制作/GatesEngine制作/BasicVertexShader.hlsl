#include"BasicShaderHeader.hlsli"
VSOutput VSmain( float4 pos : POSITION )
{
    VSOutput output;
	output.svpos = mul(mat,pos);
	return output;
}