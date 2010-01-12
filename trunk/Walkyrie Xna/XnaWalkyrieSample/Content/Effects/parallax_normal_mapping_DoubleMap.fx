
struct Light
{
	float3 dir;				// world space direction
	float3 pos;				// world space position
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float spotInnerCone;	// spot light inner cone (theta) angle
	float spotOuterCone;	// spot light outer cone (phi) angle
	float radius;           // applies to point and spot lights only
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 emissive;
	float4 specular;
	float shininess;
};

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

float4x4 worldMatrix;
float4x4 worldInverseTransposeMatrix;
float4x4 worldViewProjectionMatrix;

float3 cameraPos;
float4 globalAmbient;
float2 scaleBias;

float4x4 lightViewProjectionNear;
float4x4 textureScaleBiasNear;

float4x4 lightViewProjectionFar;
float4x4 textureScaleBiasFar;

float depthBiasNear;
float texelSizeNear;

float depthBiasFar;
float texelSizeFar;

float depthNear;

bool TextureDisable;

Light light;
Material material;

//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------

texture colorMapTexture;
texture normalMapTexture;
texture heightMapTexture;

sampler2D colorMap = sampler_state
{
	Texture = <colorMapTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

sampler2D normalMap = sampler_state
{
    Texture = <normalMapTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

sampler2D heightMap = sampler_state
{
    Texture = <heightMapTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
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
// Vertex Shaders.
//-----------------------------------------------------------------------------

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
    float4 tangent : TANGENT;
};

struct VS_OUTPUT_DIR
{
	float4 position			: POSITION;
	float2 texCoord			: TEXCOORD0;
	float3 halfVector		: TEXCOORD1;
	float3 lightDir			: TEXCOORD2;
    float4 LightSpacePosNear	: TEXCOORD3;
    float4 LightSpacePosFar		: TEXCOORD4;
    float3 PositionObject		: TEXCOORD5;
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
};

struct VS_OUTPUT_POINT
{
	float4 position			: POSITION;
	float2 texCoord			: TEXCOORD0;
	float3 viewDir			: TEXCOORD1;
	float3 lightDir			: TEXCOORD2;
    float4 LightSpacePosNear	: TEXCOORD3;
    float4 LightSpacePosFar		: TEXCOORD4;
    float3 PositionObject		: TEXCOORD5;
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
};

struct VS_OUTPUT_SPOT
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 viewDir : TEXCOORD1;
	float3 lightDir : TEXCOORD2;
	float3 spotDir : TEXCOORD3;
    float4 LightSpacePosNear	: TEXCOORD4;
    float4 LightSpacePosFar		: TEXCOORD5;
    float3 PositionObject		: TEXCOORD6;
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
};


VS_OUTPUT_DIR VS_DirLighting(VS_INPUT IN)
{
	VS_OUTPUT_DIR OUT;

	float3 worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
	float3 lightDir = -light.dir;
	float3 viewDir = cameraPos - worldPos;
	float3 halfVector = normalize(normalize(lightDir) + normalize(viewDir));
		
	float3 n = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);
	float3 t = mul(IN.tangent.xyz, (float3x3)worldInverseTransposeMatrix);
	float3 b = cross(n, t) * IN.tangent.w;
	float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);
	

	
	OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
	OUT.texCoord = IN.texCoord;
	OUT.halfVector = mul(halfVector, tbnMatrix);
	OUT.lightDir = mul(lightDir, tbnMatrix);
	
	OUT.LightSpacePosNear = mul(float4(worldPos,1.0f), lightViewProjectionNear);
	OUT.LightSpacePosFar = mul(float4(worldPos,1.0f), lightViewProjectionFar);
	
	OUT.PositionObject = float4(worldPos,1.0f);
	
	OUT.diffuse = material.diffuse * light.diffuse;
	OUT.specular = material.specular * light.specular;

	return OUT;
}

VS_OUTPUT_POINT VS_PointLighting(VS_INPUT IN)
{
	VS_OUTPUT_POINT OUT;

	float3 worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
	float3 viewDir = cameraPos - worldPos;
	float3 lightDir = (light.pos - worldPos) / light.radius;
       
    float3 n = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);
	float3 t = mul(IN.tangent.xyz, (float3x3)worldInverseTransposeMatrix);
	float3 b = cross(n, t) * IN.tangent.w;
	float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);
			
	OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
	OUT.texCoord = IN.texCoord;
	OUT.viewDir = mul(viewDir, tbnMatrix);
	OUT.lightDir = mul(lightDir, tbnMatrix);
	
	OUT.LightSpacePosNear = mul(float4(worldPos,1.0f), lightViewProjectionNear);
	OUT.LightSpacePosFar = mul(float4(worldPos,1.0f), lightViewProjectionFar);
	
	OUT.PositionObject = float4(worldPos,1.0f);
	
	OUT.diffuse = material.diffuse * light.diffuse;
	OUT.specular = material.specular * light.specular;
	
	return OUT;
}

VS_OUTPUT_SPOT VS_SpotLighting(VS_INPUT IN)
{
    VS_OUTPUT_SPOT OUT;
    
    float3 worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
    float3 viewDir = cameraPos - worldPos;
	float3 lightDir = (light.pos - worldPos) / light.radius;
    
    float3 n = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);
	float3 t = mul(IN.tangent.xyz, (float3x3)worldInverseTransposeMatrix);
	float3 b = cross(n, t) * IN.tangent.w;
	float3x3 tbnMatrix = float3x3(t.x, b.x, n.x,
	                              t.y, b.y, n.y,
	                              t.z, b.z, n.z);
		       
    OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
	OUT.texCoord = IN.texCoord;
	OUT.viewDir = mul(viewDir, tbnMatrix);
	OUT.lightDir = mul(lightDir, tbnMatrix);
    OUT.spotDir = mul(light.dir, tbnMatrix);
    
    OUT.LightSpacePosNear = mul(float4(worldPos,1.0f), lightViewProjectionNear);
	OUT.LightSpacePosFar = mul(float4(worldPos,1.0f), lightViewProjectionFar);
	
	OUT.PositionObject = float4(worldPos,1.0f);
	
    OUT.diffuse = material.diffuse * light.diffuse;
	OUT.specular = material.specular * light.specular;
       
    return OUT;
}

//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float PS_ShadowMapLookup(sampler shadowMap, float2 texCoord, float depth, float valdepthBias)
{
	return (tex2D(shadowMap, texCoord).r + valdepthBias < depth) ? 0.0f : 1.0f;
}

float PS_ShadowMapLookup(sampler shadowMap, float2 texCoord, float2 offset, float depth, float valdepthBias, float valtexelSize)
{
	return (tex2D(shadowMap, texCoord + offset * valtexelSize).r + valdepthBias < depth) ? 0.0f : 1.0f;
}

float4 PS_DirLighting(VS_OUTPUT_DIR IN, uniform bool bParallax) : COLOR
{
    float2 texCoord;
    float3 h = normalize(IN.halfVector);

    if (bParallax == true)
    {
        float height = tex2D(heightMap, IN.texCoord).r;
        
        height = height * scaleBias.x + scaleBias.y;
        texCoord = IN.texCoord + (height * h.xy);
    }
    else
    {
        texCoord = IN.texCoord;
    }

    float3 l = normalize(IN.lightDir);
    float3 n = normalize(tex2D(normalMap, texCoord).rgb * 2.0f - 1.0f);
    
    float nDotL = saturate(dot(n, l));
    float nDotH = saturate(dot(n, h));
    float power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);
    
    float DistanceObject = distance(IN.PositionObject,cameraPos);
	
	float4 lightSpacePos;
	float4 shadowCoord;	
		
	if(DistanceObject <= depthNear)
	{
		lightSpacePos = IN.LightSpacePosNear;
		shadowCoord = mul(lightSpacePos, textureScaleBiasNear);
	}
	else
	{
		lightSpacePos = IN.LightSpacePosFar;
		shadowCoord = mul(lightSpacePos, textureScaleBiasFar);
	}
	
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
    
    
	float4 color = (material.ambient * (globalAmbient + light.ambient)) +
                   (IN.diffuse * nDotL * shadowOcclusion) + (IN.specular * power * shadowOcclusion);

	color = color * tex2D(colorMap, texCoord);
	color = float4(color.xyz,1.0f);
	return color;
}

float4 PS_PointLighting(VS_OUTPUT_POINT IN, uniform bool bParallax) : COLOR
{
    float2 texCoord;
    float3 v = normalize(IN.viewDir);
    
    if (bParallax == true)
    {
        float height = tex2D(heightMap, IN.texCoord).r;
        
        height = height * scaleBias.x + scaleBias.y;
        texCoord = IN.texCoord + (height * v.xy);
    }
    else
    {
        texCoord = IN.texCoord;
    }

    float atten = saturate(1.0f - dot(IN.lightDir, IN.lightDir));

	float3 n = normalize(tex2D(normalMap, texCoord).rgb * 2.0f - 1.0f);
    float3 l = normalize(IN.lightDir);
    float3 h = normalize(l + v);
    
    float nDotL = saturate(dot(n, l));
    float nDotH = saturate(dot(n, h));
    float power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);
    
	float4 color = (material.ambient *(globalAmbient + (atten * light.ambient))) +
                   (IN.diffuse * nDotL * atten) + (IN.specular * power * atten);
                   
    color = color * tex2D(colorMap, texCoord);
    color = float4(color.xyz,1.0f);               
	return color;
}

float4 PS_SpotLighting(VS_OUTPUT_SPOT IN, uniform bool bParallax) : COLOR
{   
    float2 texCoord;
    float3 v = normalize(IN.viewDir);
        	
    if (bParallax == true)
    {
        float height = tex2D(heightMap, IN.texCoord).r;
        
        height = height * scaleBias.x + scaleBias.y;
        texCoord = IN.texCoord + (height * v.xy);
    }
    else
    {
        texCoord = IN.texCoord;
    }
    	
    float atten = saturate(1.0f - dot(IN.lightDir, IN.lightDir));
    	
	float3 l = normalize(IN.lightDir);
    float2 cosAngles = cos(float2(light.spotOuterCone, light.spotInnerCone) * 0.5f);
    float spotDot = dot(-l, normalize(IN.spotDir));
    float spotEffect = smoothstep(cosAngles[0], cosAngles[1], spotDot);
    
    atten *= spotEffect;
                                
    float3 n = normalize(tex2D(normalMap, texCoord).rgb * 2.0f - 1.0f);
	float3 h = normalize(l + v);
    
    float nDotL = saturate(dot(n, l));
    float nDotH = saturate(dot(n, h));
    float power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);
    
    float4 color = (material.ambient * (globalAmbient + (atten * light.ambient))) +
                   (IN.diffuse * nDotL * atten) + (IN.specular * power * atten);
    
	color = color * tex2D(colorMap, texCoord);
	color = float4(color.xyz,1.0f); 
	
	return color;
}



//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique NormalMappingDirectionalLighting
{
	pass
	{
		VertexShader = compile vs_3_0 VS_DirLighting();
		PixelShader = compile ps_3_0 PS_DirLighting(false);
	}
}

technique NormalMappingPointLighting
{
    pass
    {
        VertexShader = compile vs_3_0 VS_PointLighting();
        PixelShader = compile ps_3_0 PS_PointLighting(false);
    }
}

technique NormalMappingSpotLighting
{
    pass
    {
        VertexShader = compile vs_3_0 VS_SpotLighting();
        PixelShader = compile ps_3_0 PS_SpotLighting(false);
    }
}

technique ParallaxNormalMappingDirectionalLighting
{
	pass
	{
		VertexShader = compile vs_3_0 VS_DirLighting();
		PixelShader = compile ps_3_0 PS_DirLighting(true);
	}
}

technique ParallaxNormalMappingPointLighting
{
    pass
    {
        VertexShader = compile vs_3_0 VS_PointLighting();
        PixelShader = compile ps_3_0 PS_PointLighting(true);
    }
}

technique ParallaxNormalMappingSpotLighting
{
    pass
    {
        VertexShader = compile vs_3_0 VS_SpotLighting();
        PixelShader = compile ps_3_0 PS_SpotLighting(true);
    }
}
