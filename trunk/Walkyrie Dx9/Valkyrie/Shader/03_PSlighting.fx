/////////////////////////////
// Shader : rendu de Phong //
/////////////////////////////

/////////////////////////////
//      Vertex Shader      //
/////////////////////////////

// variables globales
float4x4 MGlobViewProj;  // matrice de transformation
float4   LightPos;       // position de la source de lumiere

// structure d’entrée du VS
struct VSin {
	float4	position : POSITION;
	float4	normal   : NORMAL;
	float4	color    : COLOR;
	float2	coordtx  : TEXCOORD0; // coordonnees de texture 2D
};

// structure de sortie du VS
struct VSout {
	float4	transpos : POSITION;  // position transformée
	float4	color    : COLOR;
	float2	coordtx  : TEXCOORD0; // coordonnees de texture 2D
	float3  normal   : TEXCOORD1; // normale
	float3  lightdir : TEXCOORD2; // direction de la normale
	float3  position : TEXCOORD3; // position	
};

// fonction principale
VSout VSmain(VSin In) {
	VSout  Out=(VSout)0;

	// transformation
	Out.transpos = mul(In.position,MGlobViewProj);

	// donnees passees directement au PS
	Out.color   = In.color;
	Out.coordtx = In.coordtx;

	// direction de la source de lumiere
	float4  LDir;
	LDir = normalize(LightPos - In.position);
	
	// geometrie passee au PS pour interpolation
	Out.normal   = In.normal;
	Out.lightdir = LDir;
	Out.position = In.position;

	return Out;
}

/////////////////////////////
//       Pixel Shader      //
/////////////////////////////

// variables globales
float4    LightColor;
float     ns;
float3    ObsPos;

sampler2D  damier; // texture de l'etage 0

// structure d'entree du PS
struct PSin {
	float4	color    : COLOR;
	float2	coordtx  : TEXCOORD0; // coordonnees de texture 2D
	float3  normal   : TEXCOORD1; // normale
	float3  lightdir : TEXCOORD2; // direction de la normale
	float3  position : TEXCOORD3; // position	
};

// structure de sortie du PS
struct PSout {
	float4	color    : COLOR;
};

// fonction principale
PSout PSmain(PSin In) {
	PSout    Out=(PSout)0;
	float4   N,L,V,R;
	float4   color,Ld,Ls;

	// calcul de la geometrie
	N = normalize( float4(In.normal,0) );  // interpolé, à normaliser
	L = normalize( float4(In.lightdir,0) );  // interpolé, à normaliser
	V = normalize( float4(ObsPos - In.position,0) );
	R = - reflect(L,N);

	// couleur et texture
	color = In.color * tex2D(damier,In.coordtx);
	
	// calcul de la sortie
	Ld = dot(N,L);               // contribution diffuse
	Ls = pow( max(dot(R,V),0) , ns );   // contribution speculaire
	Out.color = LightColor * ( color * Ld + Ls );
	
	return Out;	
}