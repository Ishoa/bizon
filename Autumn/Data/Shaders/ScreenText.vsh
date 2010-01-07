struct VSin
{
	float2	Pos : POSITION0;
	float4	Color : COLOR0;
	float2	TexCoord : TEXCOORD0;
};

struct VSout
{
	float4	Pos : SV_POSITION;
	float4	Color : COLOR0;
	float2	TexCoord : TEXCOORD0;
};

VSout VSMain( VSin Vin )
{
	VSout Vout = (VSout)0;
	Vout.Pos = float4(Vin.Pos, 0.0f, 1.0f);
	Vout.Color = Vin.Color;
	Vout.TexCoord = Vin.TexCoord;
	return Vout;
}