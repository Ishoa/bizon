
// Maximum number of bone matrices we can render using shader 2.0 in a single pass.
// If you change this, update SkinnedModelProcessor.cs to match.
#define MaxBones 59

// Input parameters.
float4x4 lightViewProjection;

float4x4 Bones[MaxBones];

// Vertex shader program.
void VS_CreateShadowMap(	 in float4 Position : POSITION0,
							 in float4 BoneIndices : BLENDINDICES0,
							 in float4 BoneWeights : BLENDWEIGHT0,
							 out float4 outPosition : POSITION,
							 out float2 outDepth    : TEXCOORD)
{
    
    // Blend between the weighted bone matrices.
    float4x4 skinTransform = 0;
    
    skinTransform += Bones[BoneIndices.x] * BoneWeights.x;
    skinTransform += Bones[BoneIndices.y] * BoneWeights.y;
    skinTransform += Bones[BoneIndices.z] * BoneWeights.z;
    skinTransform += Bones[BoneIndices.w] * BoneWeights.w;
    
    // Skin the vertex position.
    float4 position = mul(Position, skinTransform);
    
    outPosition = mul(position,lightViewProjection);
	
	outDepth =  outPosition.zw;
    
}

void PS_CreateShadowMap(in  float2 inDepth  : TEXCOORD,
                        out float4 outColor : COLOR)
{
    outColor = float4(inDepth.x / inDepth.y, 0.0f, 0.0f, 1.0f);
}


technique ShadowMapSkinnedModelTechnique
{
    pass SkinnedModelPass
    {
        VertexShader = compile vs_2_0 VS_CreateShadowMap();
        PixelShader = compile ps_2_0 PS_CreateShadowMap();
    }
}
