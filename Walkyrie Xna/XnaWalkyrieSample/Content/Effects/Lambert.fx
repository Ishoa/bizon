//-----------------------------------------------------------------------------
// Copyright (c) 2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

float4x4 world;
float4x4 view;
float4x4 projection;
float4x4 lightViewProjection;
float4x4 textureScaleBias;

float3 lightDir;
float4 lightColor;
float4 materialAmbient;
float4 materialDiffuse;

float depthBias;
float texelSize;

//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------

texture colorMap;
sampler colorMapSampler = sampler_state
{
	Texture = <colorMap>;
    MinFilter = Anisotropic;
	MagFilter = Linear;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

texture shadowMap;
sampler shadowMapSampler = sampler_state
{
    Texture = <shadowMap>;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
    AddressU = Border;				// border sampling in U
    AddressV = Border;				// border sampling in V
    BorderColor = float4(1,0,0,0);	// outside of border should be black

};

//-----------------------------------------------------------------------------
// Vertex shaders.
//-----------------------------------------------------------------------------

void VS_Lambert(in  float4 inPosition  : POSITION,
	            in  float2 inTexCoord  : TEXCOORD,
	            in  float3 inNormal    : NORMAL,
			    out float4 outPosition : POSITION,
			    out float2 outTexCoord : TEXCOORD0,
				out float3 outNormal   : TEXCOORD1,
				out float3 outLightDir : TEXCOORD2)
{
	float4x4 worldViewProjection = mul(mul(world, view), projection);
	
	outPosition = mul(inPosition, worldViewProjection);
	outTexCoord = inTexCoord;
	outNormal = mul(inNormal, (float3x3)world);
	outLightDir = -lightDir;
}

void VS_LambertWithShadows(in  float4 inPosition        : POSITION,
	                       in  float2 inTexCoord        : TEXCOORD,
	                       in  float3 inNormal          : NORMAL,
			               out float4 outPosition       : POSITION,
			               out float4 outLightSpacePos  : TEXCOORD0,
			               out float2 outShadowTexCoord : TEXCOORD1,
			               out float2 outTexCoord       : TEXCOORD2,
				           out float3 outNormal         : TEXCOORD3,
				           out float3 outLightDir       : TEXCOORD4)
{
	float4x4 worldViewProjection = mul(mul(world, view), projection);
	float4 lightSpacePos = mul(mul(inPosition, world), lightViewProjection);
	float4 shadowCoord = mul(lightSpacePos, textureScaleBias);
					
	outPosition = mul(inPosition, worldViewProjection);
	outLightSpacePos = lightSpacePos;
	outShadowTexCoord = shadowCoord.xy / shadowCoord.w;
	outTexCoord = inTexCoord;
	outNormal = mul(inNormal, (float3x3)world);
	outLightDir = -lightDir;
}

//-----------------------------------------------------------------------------
// Pixel shaders.
//-----------------------------------------------------------------------------

float PS_ShadowMapLookup(sampler shadowMap, float2 texCoord, float depth)
{
	return (tex2D(shadowMap, texCoord).r + depthBias < depth) ? 0.0f : 1.0f;
}

float PS_ShadowMapLookup(sampler shadowMap, float2 texCoord, float2 offset, float depth)
{
	return (tex2D(shadowMap, texCoord + offset * texelSize).r + depthBias < depth) ? 0.0f : 1.0f;
}

void PS_Lambert(in  float2 inTexCoord : TEXCOORD0,
                in  float3 inNormal   : TEXCOORD1,
                in  float3 inLightDir : TEXCOORD2,
				out float4 outColor   : COLOR)
{
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL);
			   
	outColor *= tex2D(colorMapSampler, inTexCoord);

}

void PS_LambertWithShadows(in  float4 inLightSpacePos  : TEXCOORD0,
                           in  float2 inShadowTexCoord : TEXCOORD1,
                           in  float2 inTexCoord       : TEXCOORD2,
                           in  float3 inNormal         : TEXCOORD3,
                           in  float3 inLightDir       : TEXCOORD4,
				           out float4 outColor         : COLOR)
{
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	float depth = inLightSpacePos.z / inLightSpacePos.w;
    float shadowOcclusion = PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, depth);
    
    outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL) * shadowOcclusion;
			   
	outColor *= tex2D(colorMapSampler, inTexCoord);
}

void PS_LambertWithShadowsPCF2x2(in  float4 inLightSpacePos  : TEXCOORD0,
                                 in  float2 inShadowTexCoord : TEXCOORD1,
                                 in  float2 inTexCoord       : TEXCOORD2,
                                 in  float3 inNormal         : TEXCOORD3,
				                 in  float3 inLightDir       : TEXCOORD4,
				                 out float4 outColor         : COLOR)
{
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));

    float depth = inLightSpacePos.z / inLightSpacePos.w;
    float shadowOcclusion = 0.0f;
    
    shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(0.0f, 0.0f), depth);
    shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(1.0f, 0.0f), depth);
    
    shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(0.0f, 1.0f), depth);
    shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(1.0f, 1.0f), depth);   
    
    shadowOcclusion /= 4.0f;
            
    outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL) * shadowOcclusion;
			   
	outColor *= tex2D(colorMapSampler, inTexCoord);
}

void PS_LambertWithShadowsPCF3x3(in  float4 inLightSpacePos  : TEXCOORD0,
                                 in  float2 inShadowTexCoord : TEXCOORD1,
                                 in  float2 inTexCoord       : TEXCOORD2,
                                 in  float3 inNormal         : TEXCOORD3,
				                 in  float3 inLightDir       : TEXCOORD4,
				                 out float4 outColor         : COLOR)
{
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	float depth = inLightSpacePos.z / inLightSpacePos.w;
	float shadowOcclusion = 0.0f;
		
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(0.0f, 0.0f), depth);
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(1.0f, 0.0f), depth);
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(2.0f, 0.0f), depth);
	
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(0.0f, 1.0f), depth);
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(1.0f, 1.0f), depth);
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(2.0f, 1.0f), depth);
	
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(0.0f, 2.0f), depth);
    shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(1.0f, 2.0f), depth);
	shadowOcclusion += PS_ShadowMapLookup(shadowMapSampler, inShadowTexCoord, float2(2.0f, 2.0f), depth);
	
	shadowOcclusion /= 9.0f;
	                
    outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL) * shadowOcclusion;
			   
	outColor *= tex2D(colorMapSampler, inTexCoord);
}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique Lambert
{
	pass
	{
		VertexShader = compile vs_2_0 VS_Lambert();
		PixelShader = compile ps_2_0 PS_Lambert();
	}
}

technique LambertWithShadows
{
    pass
    {
        VertexShader = compile vs_2_0 VS_LambertWithShadows();
        PixelShader = compile ps_2_0 PS_LambertWithShadows();
    }
}

technique LambertWithShadows2x2PCF
{
    pass
    {
        VertexShader = compile vs_2_0 VS_LambertWithShadows();
        PixelShader = compile ps_2_0 PS_LambertWithShadowsPCF2x2();
    }
}

technique LambertWithShadows3x3PCF
{
    pass
    {
        VertexShader = compile vs_2_0 VS_LambertWithShadows();
        PixelShader = compile ps_2_0 PS_LambertWithShadowsPCF3x3();
    }
}