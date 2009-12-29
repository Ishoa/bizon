
using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using SkinnedModel;

namespace XNAWalkyrie
{
    /// <summary>
    /// A reusable shadow map class for directional light sources. This shadow
    /// map class only supports shadow maps that are square in size. This class
    /// is intended to be used with the ShadowMapping.fx effect file.
    /// </summary>
	public class ShadowMap
	{
	#region Constants
        private const int DEFAULT_SHADOWMAP_SIZE = 2048;
        private const float DEFAULT_DEPTH_BIAS = 0.001f;
	#endregion

    #region Structure

        public enum TypeModel
        {
            Fixe,
            Animated,
        }

        public struct ModelAndMatrix
        {
            public Model model;
            public Matrix matrix;
            public TypeModel type;
            public AnimationPlayer animationPlayer;
        }
    #endregion

    #region Fields
        private int size;
        private float depthBias;
        private Vector3 lightDir;
        private Matrix lightViewMatrix;
        private Matrix lightProjectionMatrix;
		private Matrix lightViewProjectionMatrix;
        private Matrix textureMatrix;
		private RenderTarget2D renderTarget;
        private Texture2D shadowMapTexture;
		private DepthStencilBuffer depthBuffer;
        private DepthStencilBuffer oldDepthStencilBuffer;
        private GraphicsDevice graphics;

        private Effect effectFixe;
        private Effect effectAnimated;

        private Dictionary<ModelMeshPart, Effect> originalEffects;
	#endregion
		
	#region Properties


		public int Size
		{
			get { return size; }
		}
		
        public float DepthBias
        {
            get { return depthBias; }
            set { depthBias = value; }
        }

        public Vector3 LightDirection
        {
            get { return lightDir; }
        }

        public Matrix LightViewMatrix
        {
            get { return lightViewMatrix; }
        }

		public Matrix LightViewProjectionMatrix
		{
			get { return lightViewProjectionMatrix; }
		}
		
        public Matrix LightProjectionMatrix
        {
            get { return lightProjectionMatrix; }
        }

		public RenderTarget2D ShadowMapRenderTarget
		{
			get { return renderTarget; }
		}
		
		public DepthStencilBuffer ShadowMapDepthBuffer
		{
			get { return depthBuffer; }
		}
		
        public Effect ShadowMapEffect
        {
            get { return effectFixe; }
        }

		public Texture2D ShadowMapTexture
		{
            get { return shadowMapTexture; }
		}

        public float TexelSize
        {
            get { return 1.0f / size; }
        }

        public Matrix TextureScaleBiasMatrix
        {
            get { return textureMatrix; }
        }
	#endregion

	#region Public methods
        /// <summary>
        /// Constructs a new shadow map using the ShadowMap class' default
        /// shadow map size.
        /// </summary>
        /// <param name="graphicsDevice"></param>
        /// <param name="content"></param>
		public ShadowMap(GraphicsDevice graphicsDevice, ContentManager content)
            : this(graphicsDevice, content, DEFAULT_SHADOWMAP_SIZE)
		{
		}
		
        /// <summary>
        /// Constructs a new shadow map of the specified size.
        /// </summary>
        /// <param name="graphicsDevice"></param>
        /// <param name="content"></param>
        /// <param name="shadowMapSize"></param>
		public ShadowMap(GraphicsDevice graphicsDevice, ContentManager content, int shadowMapSize)
		{
			size = shadowMapSize;
            depthBias = DEFAULT_DEPTH_BIAS;
            lightDir = Vector3.Forward;
            lightViewMatrix = Matrix.Identity;
            lightProjectionMatrix = Matrix.Identity;
			lightViewProjectionMatrix = Matrix.Identity;
			originalEffects = new Dictionary<ModelMeshPart, Effect>();

			Init(graphicsDevice, content);
            CreateTextureScaleBiasMatrix();
		}

		/// <summary>
		/// Binds the shadow map to the GraphicsDevice so that we can render
        /// to the shadow map.
		/// </summary>
		/// <param name="graphicsDevice"></param>
        public void Begin(GraphicsDevice graphicsDevice)
        {
            graphics = graphicsDevice;

            if (graphics != null)
            {
                graphics.SetRenderTarget(0, renderTarget);
                oldDepthStencilBuffer = graphics.DepthStencilBuffer;
                graphics.DepthStencilBuffer = depthBuffer;
                graphics.Clear(Color.White);
            }
        }

        /// <summary>
        /// Convenience method to render an XNA Model object to the shadow
        /// map using the ShadowMap class' ShadowMapping.fx effect file.
        /// This method temporarily changes the model's effect to use the
        /// ShadowMapping.fx effect for rendering to the shadow map texture.
        /// When rendering has finished the model's original effect is restored.
        /// </summary>
        /// <param name="model"></param>
        /// <param name="world"></param>
        public void Draw(List<ModelAndMatrix> ModelAndMatrixList)
        {
            originalEffects.Clear();
            foreach (ModelAndMatrix ModAndMat in ModelAndMatrixList)
            {
                Matrix[] bones = ModAndMat.model.GetAboluteBoneTransforms();

                foreach (ModelMesh m in ModAndMat.model.Meshes)
                {
                    
                    foreach (ModelMeshPart p in m.MeshParts)
                    {
                        originalEffects[p] = p.Effect;
                        if(ModAndMat.type == TypeModel.Animated)
                            p.Effect = effectAnimated;
                        else
                            p.Effect = effectFixe;

                    }

                    if (ModAndMat.type == TypeModel.Animated)
                    {
                        Matrix[] mybones = ModAndMat.animationPlayer.GetSkinTransforms();

                        foreach (Effect e in m.Effects)
                        {
                            e.CurrentTechnique = e.Techniques["ShadowMapSkinnedModelTechnique"];
                            e.Parameters["Bones"].SetValue(mybones);
                            e.Parameters["lightViewProjection"].SetValue(lightViewProjectionMatrix);
                        }
                    }
                    else
                    {

                        foreach (Effect e in m.Effects)
                        {
                            e.CurrentTechnique = e.Techniques["CreateShadowMap"];
                            e.Parameters["world"].SetValue(bones[m.ParentBone.Index] * ModAndMat.matrix);
                            e.Parameters["lightViewProjection"].SetValue(lightViewProjectionMatrix);
                        }
                    }


                    m.Draw();

                    
                    foreach (KeyValuePair<ModelMeshPart, Effect> entry in originalEffects)
                        entry.Key.Effect = entry.Value;
                    
                    originalEffects.Clear();
                }
            }
        }

        /// <summary>
        /// Unbinds the shadow map from the GraphicsDevice.
        /// </summary>
        public void End()
        {
            if (graphics != null)
            {
                graphics.SetRenderTarget(0, null);
                graphics.DepthStencilBuffer = oldDepthStencilBuffer;

                oldDepthStencilBuffer = null;
                graphics = null;

                shadowMapTexture = renderTarget.GetTexture();                
            }
        }

        /// <summary>
        /// Calculates the view-projection matrix for the directional light
        /// source based on the camera's current view-projection matrix. See
        /// the inline code comments in this method for further details. For a
        /// full discussion on the algorithm used here see the URLs provided
        /// in the remarks below.
        /// </summary>
        /// <param name="lightDir">World space direction of the light.</param>
        /// <param name="viewProjection">The camera's view-projection matrix.</param>
        /// <remarks>
        /// http://www.gamedev.net/community/forums/topic.asp?topic_id=505893&whichpage=1&#3299665
        /// http://forums.xna.com/forums/t/16734.aspx        
        /// </remarks>
        public void Update(Vector3 lightDir, Matrix viewProjection)
        {
            // 1. Calculate the world space location of the 8 corners of the
            // view frustum.

            BoundingFrustum frustum = new BoundingFrustum(viewProjection);
            Vector3[] frustumCornersWorldSpace = frustum.GetCorners();
                        
            // 2. Calculate the centroid of the frustum.

            Vector3 centroid = Vector3.Zero;

            lightDir.Normalize();

            foreach (Vector3 frustumCorner in frustumCornersWorldSpace)
                centroid += frustumCorner;

            centroid /= (float)frustumCornersWorldSpace.Length;

            // 3. Calculate the position of the direction light.
            // Start at the centroid and then move back in the opposite
            // direction of the light by an amount equal to the camera's far
            // clip plane. This is the position of the light.

            float distance = Math.Abs(frustum.Near.D) + Math.Abs(frustum.Far.D);
            Vector3 Position = centroid - (lightDir * distance);
            Vector3 Target = centroid;
            lightViewMatrix = Matrix.CreateLookAt(Position, Target, Vector3.Up);

            // 4. Calculate the light space locations of the 8 corners of the
            // (world space) view frustum. The lightViewMatrix is used to
            // transform each world space frustum corner into light space.

            Vector3[] frustumCornersLightSpace = new Vector3[frustumCornersWorldSpace.Length];
            Vector3.Transform(frustumCornersWorldSpace, ref lightViewMatrix, frustumCornersLightSpace);

            // 5. Calculate the bounding box for the light space frustum
            // corners. The bounding box is used to construct the proper
            // orthographic projection matrix for the directional light.

            BoundingBox box = BoundingBox.CreateFromPoints(frustumCornersLightSpace);
            lightProjectionMatrix = Matrix.CreateOrthographicOffCenter(
               box.Min.X, box.Max.X, box.Min.Y , box.Max.Y , -box.Max.Z - 750.0f, -box.Min.Z + 750.0f);
            
            lightViewProjectionMatrix = lightViewMatrix * lightProjectionMatrix;
        }

    #endregion

    #region Private methods
        private void CreateTextureScaleBiasMatrix()
        {
            float offset = 0.5f + (0.5f / (float)size);
            
            textureMatrix = new Matrix(0.5f,    0.0f,  0.0f, 0.0f,
                                       0.0f,   -0.5f,  0.0f, 0.0f,
                                       0.0f,    0.0f,  0.0f, 0.0f,
                                       offset, offset, 0.0f, 1.0f);
        }

		private void Init(GraphicsDevice graphicsDevice, ContentManager content)
		{
			bool supported = false;
			SurfaceFormat shadowMapFormat = SurfaceFormat.Unknown;
            DepthFormat shadowMapDepthFormat = DepthFormat.Unknown;
			
			SurfaceFormat[] shadowMapFormats =
			{    
                //SurfaceFormat.Color,
				SurfaceFormat.Single,
				SurfaceFormat.HalfSingle,
                SurfaceFormat.Color

			};

            DepthFormat[] shadowMapDepthFormats =
            {
                DepthFormat.Depth24Stencil8Single,
                DepthFormat.Depth24Stencil8,
                DepthFormat.Depth24,
                DepthFormat.Depth16,
                DepthFormat.Depth15Stencil1
            };

			foreach (SurfaceFormat surfaceFormat in shadowMapFormats)
			{
				supported = GraphicsAdapter.DefaultAdapter.CheckDeviceFormat(
					        DeviceType.Hardware,
					        GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Format,
		                    TextureUsage.Linear,
					        QueryUsages.None,
		                    ResourceType.RenderTarget,
					        surfaceFormat);
							
				if (supported)
				{
					shadowMapFormat = surfaceFormat;
					break;
				}
			}

            foreach (DepthFormat depthFormat in shadowMapDepthFormats)
            {
                supported = GraphicsAdapter.DefaultAdapter.CheckDepthStencilMatch(
                            DeviceType.Hardware,
                            GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Format,
                            shadowMapFormat,
                            depthFormat);

                if (supported)
                {
                    shadowMapDepthFormat = depthFormat;
                    break;
                }
            }

			renderTarget = new RenderTarget2D(graphicsDevice,
		                   size, size, 1, shadowMapFormat);

			depthBuffer = new DepthStencilBuffer(graphicsDevice, 
                          size, size, shadowMapDepthFormat);

            string[] effectFixeAssetNames =
            {
                "ShadowMapping",
                @"Effects\ShadowMapping",
                @"Shaders\ShadowMapping"
            };

            foreach (string assetName in effectFixeAssetNames)
            {
                try
                {
                    effectFixe = content.Load<Effect>(assetName);
                    break;
                }
                catch (ContentLoadException)
                {
                }
            }

            if (effectFixe == null)
                throw new ContentLoadException("Failed to load ShadowMapping.fx");

            string[] effectAnimatedAssetNames =
            {
                "ShadowMappingSkinned",
                @"Effects\ShadowMappingSkinned",
                @"Shaders\ShadowMappingSkinned"
            };

            foreach (string assetName in effectAnimatedAssetNames)
            {
                try
                {
                    effectAnimated = content.Load<Effect>(assetName);
                    break;
                }
                catch (ContentLoadException)
                {
                }
            }

            if (effectAnimated == null)
                throw new ContentLoadException("Failed to load ShadowMapping.fx");

		}
	#endregion
	}
}