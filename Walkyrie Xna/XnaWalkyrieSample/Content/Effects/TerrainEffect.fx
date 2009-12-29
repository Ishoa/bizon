float4x4 WorldViewProjection;

texture TerreTexture;
texture DensiteTerreTexture;

texture HerbeTexture;

sampler2D TerreMap = sampler_state
{
	Texture = <TerreTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

sampler2D DensiteTerreMap = sampler_state
{
	Texture = <DensiteTerreTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

sampler2D HerbeMap = sampler_state
{
	Texture = <HerbeTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

// TODO: add effect parameters here.

struct VertexShaderInput
{
    float4 Position : POSITION;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
    // TODO: add input channels such as texture
    // coordinates and vertex colors here.
};

struct VertexShaderOutput
{
    float4 Position : POSITION;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
    // TODO: add vertex shader outputs such as colors and texture
    // coordinates here. These values will automatically be interpolated
    // over the triangle, and provided as input to your pixel shader.
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.Position = mul(input.Position, WorldViewProjection);
    
	output.texCoord0 = input.texCoord0;
	output.texCoord1 = input.texCoord1;
	
    // TODO: add your vertex shader code here.

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
    // TODO: add your pixel shader code here.
    
    float4 colorTerre = tex2D(TerreMap,input.texCoord0) * tex2D(DensiteTerreMap,input.texCoord1).a;   
    float4 colorHerbe = tex2D(HerbeMap,input.texCoord0) * (1.0f - tex2D(DensiteTerreMap,input.texCoord1).a);
    
    float4 finalcolor = colorTerre + colorHerbe;
    return float4(finalcolor.rgb,1);
}

technique Technique1
{
    pass Pass1
    {
        // TODO: set renderstates here.

        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunction();

    }
}
