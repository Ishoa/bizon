cbuffer cb : register(c0)
{
	row_major float4x4 mWorldViewProj;
}

struct VSin
{
	float3	Pos : POSITION0;
};

struct VSout
{
	float4	Pos : SV_POSITION;
};

VSout VSMain( VSin Vin )
{
	VSout Vout = (VSout)0;
	Vout.Pos = mul(float4(Vin.Pos, 1.0f) , mWorldViewProj);
	return Vout;
}