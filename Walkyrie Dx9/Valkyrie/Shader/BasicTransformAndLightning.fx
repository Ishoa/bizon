// variables globales
float4x4 MGlobViewProj;  // matrice de transformation
float4   LPos;           // position de la source de lumiere

// structure d’entrée du VS
struct VSin {
	float3	position : POSITION;
	float3	normal   : NORMAL;
	float4	color    : COLOR;
	float2	tx0      : TEXCOORD0;
};

// structure de sortie du VS
struct VSout {
	float4	position : POSITION;
	float4	color    : COLOR;
	float2	tx0      : TEXCOORD0;
};

// fonction principale
VSout main(VSin In) {
	VSout  Out=(VSout)0;
	// transformation
	Out.position = mul(float4(In.position,1),MGlobViewProj);
	// calcul de l'eclairage diffus
	float3  LDir;
	LDir = normalize(LPos - In.position);
	Out.color = In.color * dot(LDir,In.normal);
	// copie des coordonnees de texture
	Out.tx0 = In.tx0;
	return Out;
}
