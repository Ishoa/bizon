
//Input variables

texture baseTexture;
float4x4 worldViewProj;

sampler baseSampler = 
sampler_state
{
    Texture = < baseTexture >;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    ADDRESSU = CLAMP;
    ADDRESSV = CLAMP;
};

struct VS_INPUT
{
    float4 ObjectPos: POSITION;
    float4 Color: COLOR0;
    float2 TextureCoords: TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 ScreenPos:   POSITION;
   float4 Color: COLOR0;
   float2 TextureCoords: TEXCOORD0;
};

struct PS_OUTPUT 
{
   float4 Color:   COLOR;
};



VS_OUTPUT SimpleVS(VS_INPUT In)
{
   VS_OUTPUT Out;

    //Move to screen space
    Out.ScreenPos = mul(In.ObjectPos,worldViewProj);
    Out.Color = In.Color;
    Out.TextureCoords = In.TextureCoords;

    return Out;
}

PS_OUTPUT SimplePS(VS_OUTPUT In)
{
    PS_OUTPUT Out;
    
    //Out.Color = tex2D(baseSampler, In.TextureCoords);
    
    Out.Color = tex2D(baseSampler, In.TextureCoords) * In.Color;
    return Out;
}


technique Simple
{
   pass Single_Pass
   {
        SrcBlend = SrcAlpha; 
        DestBlend = InvSrcAlpha; 

        VertexShader = compile vs_2_0 SimpleVS();
        PixelShader = compile ps_2_0 SimplePS();
   }
}