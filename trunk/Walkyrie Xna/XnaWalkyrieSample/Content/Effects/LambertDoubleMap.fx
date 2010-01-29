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
float4x4 worldViewProjection;

float4x4 lightViewProjectionNear;
float4x4 textureScaleBiasNear;

float4x4 lightViewProjectionFar;
float4x4 textureScaleBiasFar;

float3 CamPosition;
float3 lightDir;
float4 lightColor;
float4 materialAmbient;
float4 materialDiffuse;

float depthBiasNear;
float texelSizeNear;

float depthBiasFar;
float texelSizeFar;

float depthNear;

bool TextureDisable;
bool TextureDensite;

bool FogEnable;
float4 ColorFor;
float RangeMaxFog;
float DistanceMinFog;

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

texture densiteMap;
sampler densiteMapSampler = sampler_state
{
	Texture = <densiteMap>;
    MinFilter = Anisotropic;
	MagFilter = Linear;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

texture colorDensiteMap;
sampler colorDensiteMapSampler = sampler_state
{
	Texture = <colorDensiteMap>;
    MinFilter = Anisotropic;
	MagFilter = Linear;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

texture shadowMapNear;
sampler shadowMapNearSampler = sampler_state
{
    Texture = <shadowMapNear>;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = None;
    AddressU = Border;				// border sampling in U
    AddressV = Border;				// border sampling in V
    BorderColor = float4(1,0,0,0);	// outside of border should be black

};

texture shadowMapFar;
sampler shadowMapFarSampler = sampler_state
{
    Texture = <shadowMapFar>;
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
	            in  float2 inTexCoord0  : TEXCOORD0,
	            in  float2 inTexCoord1  : TEXCOORD1,
	            in  float3 inNormal    : NORMAL,
			    out float4 outPosition : POSITION,
			    out float2 outTexCoord0 : TEXCOORD0,
			    out float2 outTexCoord1 : TEXCOORD1,
				out float3 outNormal   : TEXCOORD2,
				out float3 outLightDir : TEXCOORD3)
{
	float4 Position = mul(inPosition,world);
	outPosition = mul(inPosition, worldViewProjection);
	outTexCoord0 = inTexCoord0;
	outTexCoord1 = inTexCoord1;
	outNormal = mul(inNormal, (float3x3)world);
	outLightDir = -lightDir;
}

void VS_LambertWithShadows(in  float4 inPosition			: POSITION,
						   in  float2 inTexCoord0			: TEXCOORD0,
						   in  float2 inTexCoord1			: TEXCOORD1,
	                       in  float3 inNormal				: NORMAL,
			               out float4 outPosition			: POSITION,
			               out float4 outLightSpacePosNear	: TEXCOORD0,
			               out float4 outLightSpacePosFar	: TEXCOORD1,
			               out float2 outTexCoord0			: TEXCOORD2,
			               out float2 outTexCoord1			: TEXCOORD3,
				           out float3 outNormal				: TEXCOORD4,
				           out float3 outLightDir			: TEXCOORD5,
				           out float3 outPositionObject		: TEXCOORD6)
{

	outPosition = mul(inPosition, worldViewProjection);
	float4 PositionWorld = mul(inPosition, world);
	outLightSpacePosNear = mul(PositionWorld, lightViewProjectionNear);
	
	outLightSpacePosFar = mul(PositionWorld, lightViewProjectionFar);

	outTexCoord0 = inTexCoord0;
	outTexCoord1 = inTexCoord1;
	outNormal = mul(inNormal, (float3x3)world);
	outLightDir = -lightDir;
	outPositionObject = PositionWorld;

}


//-----------------------------------------------------------------------------
// Pixel shaders.
//-----------------------------------------------------------------------------

float PS_ShadowMapLookup(sampler shadowMap, float2 texCoord, float depth, float valdepthBias)
{
	return (tex2D(shadowMap, texCoord).r + valdepthBias < depth) ? 0.0f : 1.0f;
}

float PS_ShadowMapLookup(sampler shadowMap, float2 texCoord, float2 offset, float depth, float valdepthBias, float valtexelSize)
{
	return (tex2D(shadowMap, texCoord + offset * valtexelSize).r + valdepthBias < depth) ? 0.0f : 1.0f;
}

void PS_Lambert(in  float2 inTexCoord0 : TEXCOORD0,
				in  float2 inTexCoord1 : TEXCOORD1,
                in  float3 inNormal   : TEXCOORD2,
                in  float3 inLightDir : TEXCOORD3,
				out float4 outColor   : COLOR)
{
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	outColor = (materialAmbient * lightColor) + (materialDiffuse * lightColor * nDotL);
	
	float4 FinalText;
			   
	if(TextureDensite)
	{
	    float4 colorTerre = tex2D(colorDensiteMapSampler,inTexCoord0) * tex2D(densiteMapSampler,inTexCoord1).a;   
		float4 colorHerbe = tex2D(colorMapSampler,inTexCoord0) * (1.0f - tex2D(densiteMapSampler,inTexCoord1).a);
    
		FinalText = colorTerre + colorHerbe;
		FinalText.a = 1.0f;
	}
	else
	{
		FinalText = tex2D(colorMapSampler, inTexCoord0);
	}		   
			 
	if(TextureDisable)
		   outColor *= FinalText;
		   

}

void PS_LambertWithShadows(in  float4 inLightSpacePosNear  : TEXCOORD0,
			               in  float4 inLightSpacePosFar   : TEXCOORD1,
			               in  float2 inTexCoord0       : TEXCOORD2,
			               in  float2 inTexCoord1       : TEXCOORD3,
                           in  float3 inNormal         : TEXCOORD4,
                           in  float3 inLightDir       : TEXCOORD5,
                           in  float3 inPositionObject : TEXCOORD6,
				           out float4 outColor         : COLOR)
{

	float DistanceObject = distance(inPositionObject,CamPosition);
	
	float4 lightSpacePos;
	float4 shadowCoord;		
	if(DistanceObject <= depthNear)
	{
		lightSpacePos = inLightSpacePosNear;
		shadowCoord = mul(lightSpacePos, textureScaleBiasNear);
	}
	else
	{
		lightSpacePos = inLightSpacePosFar;
		shadowCoord = mul(lightSpacePos, textureScaleBiasFar);
	}
			
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	float depth = lightSpacePos.z / lightSpacePos.w;
	
	float2 ShadowTexCoord = shadowCoord.xy / shadowCoord.w;
	
    float shadowOcclusion = 1.0f;

   	if(DistanceObject <= depthNear)
   	{
   		shadowOcclusion  = PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, depth, depthBiasNear);
	}
	else
	{
		shadowOcclusion  = PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, depth, depthBiasFar);
	}
	

    outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL) * shadowOcclusion;
			   
	float4 FinalText;
			   
	if(TextureDensite)
	{
	    float4 colorTerre = tex2D(colorDensiteMapSampler,inTexCoord0) * tex2D(densiteMapSampler,inTexCoord1).a;   
		float4 colorHerbe = tex2D(colorMapSampler,inTexCoord0) * (1.0f - tex2D(densiteMapSampler,inTexCoord1).a);
    
		FinalText = colorTerre + colorHerbe;
		FinalText.a = 1.0f;
	}
	else
	{
		FinalText = tex2D(colorMapSampler, inTexCoord0);
	}		   
			 
	if(TextureDisable)
		   outColor *= FinalText;
		   
	if(FogEnable)
	{
		float coeff = saturate((DistanceObject - DistanceMinFog)/RangeMaxFog);
		outColor = lerp(outColor,ColorFor,coeff);
	}
}


void PS_LambertWithShadowsPCF2x2(  in  float4 inLightSpacePosNear  : TEXCOORD0,
								   in  float4 inLightSpacePosFar   : TEXCOORD1,
								   in  float2 inTexCoord0       : TEXCOORD2,
								   in  float2 inTexCoord1       : TEXCOORD3,
								   in  float3 inNormal         : TEXCOORD4,
								   in  float3 inLightDir       : TEXCOORD5,
								   in  float3 inPositionObject : TEXCOORD6,
								   out float4 outColor         : COLOR)
{

	float DistanceObject = distance(inPositionObject,CamPosition);
	
	float4 lightSpacePos;
	float4 shadowCoord;		
	if(DistanceObject <= depthNear)
	{
		lightSpacePos = inLightSpacePosNear;
		shadowCoord = mul(lightSpacePos, textureScaleBiasNear);
	}
	else
	{
		lightSpacePos = inLightSpacePosFar;
		shadowCoord = mul(lightSpacePos, textureScaleBiasFar);
	}
			
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	float depth = lightSpacePos.z / lightSpacePos.w;
	
	float2 ShadowTexCoord = shadowCoord.xy / shadowCoord.w;
	
    float shadowOcclusion = 0.0f;

   	if(DistanceObject <=depthNear)
   	{

   		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(0.0f, 0.0f), depth,depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(1.0f, 0.0f), depth,depthBiasNear, texelSizeNear);
	    
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(0.0f, 1.0f), depth,depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(1.0f, 1.0f), depth,depthBiasNear, texelSizeNear);   
	    
	}
	else
	{
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(0.0f, 0.0f), depth,depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(1.0f, 0.0f), depth,depthBiasFar, texelSizeFar);
	    
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(0.0f, 1.0f), depth,depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(1.0f, 1.0f), depth,depthBiasFar, texelSizeFar);   
	    
	}
	

    shadowOcclusion /= 4.0f;
            
    outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL) * shadowOcclusion;
			   
	float4 FinalText;
			   
	if(TextureDensite)
	{
	    float4 colorTerre = tex2D(colorDensiteMapSampler,inTexCoord0) * tex2D(densiteMapSampler,inTexCoord1).a;   
		float4 colorHerbe = tex2D(colorMapSampler,inTexCoord0) * (1.0f - tex2D(densiteMapSampler,inTexCoord1).a);
    
    	FinalText = colorTerre + colorHerbe;
		FinalText.a = 1.0f;
	}
	else
	{
		FinalText = tex2D(colorMapSampler, inTexCoord0);
	}		   
			 
	if(TextureDisable)
		   outColor *= FinalText;
		   
	if(FogEnable)
	{
		float coeff = saturate((DistanceObject - DistanceMinFog)/RangeMaxFog);
		outColor = lerp(outColor,ColorFor,coeff);
	}
}

void PS_LambertWithShadowsPCF3x3(  in  float4 inLightSpacePosNear  : TEXCOORD0,
								   in  float4 inLightSpacePosFar   : TEXCOORD1,
								   in  float2 inTexCoord0       : TEXCOORD2,
								   in  float2 inTexCoord1       : TEXCOORD3,
								   in  float3 inNormal         : TEXCOORD4,
								   in  float3 inLightDir       : TEXCOORD5,
								   in  float3 inPositionObject : TEXCOORD6,
								   out float4 outColor         : COLOR)
{

	float DistanceObject = distance(inPositionObject,CamPosition);
	
	float4 lightSpacePos;
	float4 shadowCoord;		
	if(DistanceObject <= depthNear)
	{
		lightSpacePos = inLightSpacePosNear;
		shadowCoord = mul(lightSpacePos, textureScaleBiasNear);
	}
	else
	{
		lightSpacePos = inLightSpacePosFar;
		shadowCoord = mul(lightSpacePos, textureScaleBiasFar);
	}	
		
	float3 l = normalize(inLightDir);
	float3 n = normalize(inNormal);
	float nDotL = saturate(dot(n, l));
	
	float depth = lightSpacePos.z / lightSpacePos.w;
	
     
	float2 ShadowTexCoord = shadowCoord.xy / shadowCoord.w;
	
    float shadowOcclusion = 0.0f;

   	if(DistanceObject <=depthNear)
   	{
   		
   		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(0.0f, 0.0f), depth, depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(1.0f, 0.0f), depth, depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(2.0f, 0.0f), depth, depthBiasNear, texelSizeNear);
		
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(0.0f, 1.0f), depth, depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(1.0f, 1.0f), depth, depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(2.0f, 1.0f), depth, depthBiasNear, texelSizeNear);
		
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(0.0f, 2.0f), depth, depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(1.0f, 2.0f), depth, depthBiasNear, texelSizeNear);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapNearSampler, ShadowTexCoord, float2(2.0f, 2.0f), depth, depthBiasNear, texelSizeNear);
		
	}
	else
	{

		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(0.0f, 0.0f), depth, depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(1.0f, 0.0f), depth, depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(2.0f, 0.0f), depth, depthBiasFar, texelSizeFar);
		
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(0.0f, 1.0f), depth, depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(1.0f, 1.0f), depth, depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(2.0f, 1.0f), depth, depthBiasFar, texelSizeFar);
		
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(0.0f, 2.0f), depth, depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(1.0f, 2.0f), depth, depthBiasFar, texelSizeFar);
		shadowOcclusion += PS_ShadowMapLookup(shadowMapFarSampler, ShadowTexCoord, float2(2.0f, 2.0f), depth, depthBiasFar, texelSizeFar);

	}
		
	shadowOcclusion /= 9.0f;
	                
    outColor = (materialAmbient * lightColor) +
	           (materialDiffuse * lightColor * nDotL) * shadowOcclusion;
	
	float4 FinalText;
			   
	if(TextureDensite)
	{
	    float4 colorTerre = tex2D(colorDensiteMapSampler,inTexCoord0) * tex2D(densiteMapSampler,inTexCoord1).a;   
		float4 colorHerbe = tex2D(colorMapSampler,inTexCoord0) * (1.0f - tex2D(densiteMapSampler,inTexCoord1).a);
    
    	FinalText = colorTerre + colorHerbe;
		FinalText.a = 1.0f;
	}
	else
	{
		FinalText = tex2D(colorMapSampler, inTexCoord0);
	}		   
			 
	if(TextureDisable)
		   outColor *= FinalText;
	

	if(FogEnable)
	{
		float coeff = saturate((DistanceObject - DistanceMinFog)/RangeMaxFog);
		outColor = lerp(outColor,ColorFor,coeff);
	}

}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique Lambert
{
	pass
	{
		VertexShader = compile vs_3_0 VS_Lambert();
		PixelShader = compile ps_3_0 PS_Lambert();
	}
}

technique LambertWithShadows
{
    pass
    {
        VertexShader = compile vs_3_0 VS_LambertWithShadows();
        PixelShader = compile ps_3_0 PS_LambertWithShadows();
    }
}

technique LambertWithShadows2x2PCF
{
    pass
    {
        VertexShader = compile vs_3_0 VS_LambertWithShadows();
        PixelShader = compile ps_3_0 PS_LambertWithShadowsPCF2x2();
    }
}

technique LambertWithShadows3x3PCF
{
    pass
    {
        VertexShader = compile vs_3_0 VS_LambertWithShadows();
        PixelShader = compile ps_3_0 PS_LambertWithShadowsPCF3x3();
    }
}