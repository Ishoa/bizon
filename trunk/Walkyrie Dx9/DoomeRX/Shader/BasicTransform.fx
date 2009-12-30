// variables globales
float4x4 MGlobViewProj;

// structure d’entrée du VS
struct VSin {
	float3	pos : POSITION;
	float4	col : COLOR;
	float2	tx0 : TEXCOORD0;
};

// structure de sortie du VS
struct VSout {
	float4	pos : POSITION;
	float4	col : COLOR;
	float2	tx0 : TEXCOORD0;
};

// fonction principale
VSout main(VSin In) {
	VSout  Out;
	Out.pos = mul(float4(In.pos,1),MGlobViewProj);
	Out.col = In.col;
	Out.tx0 = In.tx0;
	return Out;
}
