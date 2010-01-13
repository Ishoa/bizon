cbuffer cb : register(b0)
{
	row_major float4x4 mWorldViewProj;
	float4 vLightDir;
	float4 vLightColor;
	float4 vCamera;
}

struct VSin
{
	float3	Pos		: POSITION0;
	float3	Norm	: NORMAL0;
	float3	Tan		: TANGENT0;
	float2	Tex		: TEXCOORD0;
};

struct VSout
{
	float4	ScreenPos	: SV_POSITION;
	float3	Pos			: POSITION0;
	float3	Norm		: NORMAL0;
	float3	Tan			: TANGENT0;
	float2	Tex			: TEXCOORD0;
};

VSout VSMain( VSin Vin )
{
	VSout Vout = (VSout)0;
	Vout.ScreenPos  = mul(float4(Vin.Pos, 1.0f) , mWorldViewProj);
	Vout.Pos = Vin.Pos;
	Vout.Norm = Vin.Norm;
	Vout.Tan = Vin.Tan;
	Vout.Tex = Vin.Tex;
	return Vout;
}