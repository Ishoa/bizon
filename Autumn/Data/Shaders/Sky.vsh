cbuffer cb : register(b0)
{
	row_major float4x4 mWorldViewProj;
	row_major float4x4 mWorld;
	float4 vLightDir;
	float4 vLightColor;
	float4 vCamera;
}

struct VSin
{
	float2	Pos		: POSITION0;
};

struct VSout
{
	float4	ScreenPos	: SV_POSITION;
};

VSout VSMain( VSin Vin )
{
	VSout Vout = (VSout)0;
	Vout.ScreenPos  = float4(Vin.Pos.xy, 1, 1);
	
	return Vout;
}