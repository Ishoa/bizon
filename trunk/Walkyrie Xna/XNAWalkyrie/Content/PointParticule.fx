
float4x4 World;
float4x4 View;
float4x4 Projection;

Texture Texture;
sampler TextureSampler = sampler_state 
{ 
    texture = <Texture>; 
    magfilter = LINEAR; 
    minfilter = LINEAR; 
    mipfilter=LINEAR; 
    AddressU = WRAP; 
    AddressV = WRAP;
};

struct ParticuleVertexOut
{
    float4 Position     : POSITION0;
    float1 Size         : PSIZE;
    float1 ParticuleValue      : COLOR0;
    float4 Color        : COLOR1;    
};

struct ParticulePixelIn
{
    #ifdef XBOX
        float4 TexCoord : SPRITETEXCOORD;
    #else
        float2 TexCoord : TEXCOORD0;
    #endif
    float1 ParticuleValue      : COLOR0;
    float4 Color        : COLOR1;
};

struct PixelToFrame
{
    float4 Color : COLOR0;
};

ParticuleVertexOut ParticuleVS (float4 Position : POSITION, 
                             float1 Size : PSIZE, 
                             float1 ParticuleValue : COLOR0, 
                             float4 ParticuleColor : COLOR1)
{
    ParticuleVertexOut Output = (ParticuleVertexOut)0;
    
    float4x4 preViewProjection = mul (View, Projection);
    float4x4 preWorldViewProjection = mul (World, preViewProjection); 
    Output.Position = mul(Position, preWorldViewProjection);    
    
    Output.Size = Size;
    Output.ParticuleValue = ParticuleValue;
    Output.Color.rgba = ParticuleColor.rgba;
    
    return Output;    
}

PixelToFrame ParticulePS(ParticulePixelIn PSIn)
{ 
    PixelToFrame Output = (PixelToFrame)0;    

    #ifdef XBOX
        float2 texCoord = abs(PSIn.TexCoord.zw);
    #else
        float2 texCoord = PSIn.TexCoord.xy;
    #endif

    Output.Color = tex2D(TextureSampler, texCoord);

    //Output.Color *= PSIn.Color;
    
    return Output;
}

technique ParticulePoint
{
    pass Pass0
    {   
        //PointSpriteEnable = true;
        VertexShader = compile vs_2_0 ParticuleVS();
        PixelShader  = compile ps_2_0 ParticulePS();
    }
}