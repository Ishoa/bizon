Texture2D tHeightMap : register(t0);

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
	float3	Pos		: POSITION0;
	float3	Norm	: NORMAL0;
	float3	Tan		: TANGENT0;
	float2	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
};

struct VSout
{
	float4	ScreenPos	: SV_POSITION;
	float3	Pos			: POSITION0;
	float3	Norm		: NORMAL0;
	float3	Tan			: TANGENT0;
	float2	Tex0		: TEXCOORD0;
	float2	Tex1		: TEXCOORD1;
};

VSout VSMain( VSin Vin )
{
	VSout Vout = (VSout)0;
	
	int sx, sy;
	tHeightMap.GetDimensions(sx, sy);
	float3 Pos = Vin.Pos + Vin.Norm * tHeightMap.Load(int3(Vin.Tex0.x * sx, Vin.Tex0.y * sy, 0)).r * 10.0f;
	Vout.ScreenPos  = mul(float4(Pos, 1.0f) , mWorldViewProj);
	Vout.Pos = Vin.Pos;
	Vout.Norm = Vin.Norm;
	Vout.Tan = Vin.Tan;
	Vout.Tex0 = Vin.Tex0;
	Vout.Tex1 = Vin.Tex1;
	
	return Vout;
}