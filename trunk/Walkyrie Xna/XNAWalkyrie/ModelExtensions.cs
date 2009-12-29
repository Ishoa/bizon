using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;


namespace XNAWalkyrie
{
    public static class ModelExtensions
    {
        public static void SetTexture(this Model model, Texture2D texture)
        {
            foreach (ModelMesh mesh in model.Meshes)
            {
                mesh.SetTexture(texture);
            }
        }

        public static void SetTexture(this ModelMesh mesh, Texture2D texture)
        {
            
            foreach (ModelMeshPart part in mesh.MeshParts)
            {
                BasicEffect eff = part.Effect as BasicEffect;

                if (eff != null)
                {
                    eff.Texture = texture;
                }
            }
            
        }

        public static Texture2D GetTexture(this Model model)
        {
            Texture2D ret = null;
            foreach (ModelMesh mesh in model.Meshes)
            {
                ret = mesh.GetTexture();
                if (ret != null)
                    break;
            }
            return ret;
        }

        public static Texture2D GetTexture(this ModelMesh mesh)
        {
            Texture2D ret = null;
            
            foreach (ModelMeshPart meshPart in mesh.MeshParts)
            {
                BasicEffect effect = meshPart.Effect as BasicEffect;

                if (effect != null)
                {
                    ret = effect.Texture;
                    if(ret != null)
                        break;
                }
            }
        
            return ret;
        }
        public static void SetEffect(this Model model,
                                        Effect effect)
        {
            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (ModelMeshPart mp in mesh.MeshParts)
                {
                    mp.Effect = effect;
                }
            }
        }

        public static void Draw(this Model model,
                                     Matrix World,
                                     Matrix View,
                                     Matrix Projection,
                                     BasicEffect effect)
        {
            model.SetEffect(effect);

            Matrix[] bones = model.GetAboluteBoneTransforms();

            foreach (ModelMesh mesh in model.Meshes)
            {
                foreach (BasicEffect eff in mesh.Effects)
                {
                    eff.World = bones[mesh.ParentBone.Index] * World;
                    eff.View = View;
                    eff.Projection = Projection;
                }
                mesh.Draw();
            }
        }

        public static void Draw(this Model model)
        {
            CameraComponent cam = Utility.Camera;
            model.Draw(Matrix.Identity, cam.ViewMatrix, cam.ProjectionMatrix);
        }

        public static void Draw(this Model model,
                                     Matrix World)
        {
            CameraComponent cam = Utility.Camera;
            model.Draw(World, cam.ViewMatrix, cam.ProjectionMatrix);
        }

        public static void Draw(this Model model,
                                     Matrix World,
                                     Matrix View,
                                     Matrix Projection)
        {
            Matrix[] bones = model.GetAboluteBoneTransforms();

            foreach (ModelMesh mesh in model.Meshes)
            {
                for (int x = 0; x < mesh.Effects.Count; x++)
                {
                    BasicEffect eff = mesh.Effects[x] as BasicEffect;
                    if (eff != null)
                    {
                        eff.World = bones[mesh.ParentBone.Index] * World;
                        eff.View = View;
                        eff.Projection = Projection;
                        eff.EnableDefaultLighting();
                        eff.TextureEnabled = true;
                    }
                }
                mesh.Draw();
            }
        }

        public static void GetModelMeshTransform(ModelBone bone, ref Matrix mat)
        {
            if (bone.Parent == null)
            {
                mat = bone.Transform;
            }
            else
            {
                GetModelMeshTransform(bone.Parent, ref mat);

                mat = bone.Transform * mat;
            }
        }

        public static void DrawModelMesh(this Model model,
                                         ModelMesh mesh)
        {
            Matrix mat = Matrix.Identity;
            
            GetModelMeshTransform(mesh.ParentBone,ref mat);

            Matrix View = Utility.Camera.ViewMatrix;
            Matrix Projection = Utility.Camera.ProjectionMatrix;

            for (int x = 0; x < mesh.Effects.Count; x++)
            {
                BasicEffect eff = mesh.Effects[x] as BasicEffect;
                if (eff != null)
                {
                    eff.World = mat;
                    eff.View = View;
                    eff.Projection = Projection;

                    eff.LightingEnabled = true;
                    eff.EnableDefaultLighting();
                    eff.TextureEnabled = true;
                    eff.VertexColorEnabled = true;
                }
            }
            mesh.Draw();
        }

        public static void DrawModelMeshBasic(this Model model,
                                         ModelMesh mesh)
        {
            Matrix mat = Matrix.Identity;

            GetModelMeshTransform(mesh.ParentBone, ref mat);

            Matrix View = Utility.Camera.ViewMatrix;
            Matrix Projection = Utility.Camera.ProjectionMatrix;

            for (int x = 0; x < mesh.Effects.Count; x++)
            {
                BasicEffect eff = mesh.Effects[x] as BasicEffect;
                if (eff != null)
                {
                    eff.World = mat;
                    eff.View = View;
                    eff.Projection = Projection;

                    eff.LightingEnabled = false;
                    eff.TextureEnabled = false;
                    eff.FogEnabled = false;
                    eff.VertexColorEnabled = false;
                }
            }
            mesh.Draw();
        }

        public static void DrawDummy(this Model model, 
            string dummyName, bool enableDepthBuffer)
        {
            Matrix transform = model.GetAbsoluteBoneTransform(dummyName);
            VectorRenderer vectorRenderer = Utility.VectorRenderer;
            GraphicsDevice graphicsDevice = Utility.GraphicsDevice;

            vectorRenderer.SetColor(Color.Red);
            vectorRenderer.SetViewProjMatrix(Utility.Camera.ViewProjectionMatrix);
            vectorRenderer.SetWorldMatrix(transform);

            Vector3 scale, translation;
            Quaternion rot;
            transform.Decompose(out scale, out rot, out translation);

            bool depthWasEnabled = graphicsDevice.RenderState.DepthBufferEnable;
            graphicsDevice.RenderState.DepthBufferEnable = enableDepthBuffer;
            vectorRenderer.DrawBoundingBox(
                new BoundingBox(-Vector3.One * scale.Length() * 2.0f,
                                Vector3.One * scale.Length() * 2.0f));

            vectorRenderer.SetColor(Color.Green);
            vectorRenderer.DrawLine(Vector3.Zero,
                                    transform.Up * scale.Length() * 2.0f);

            vectorRenderer.SetColor(Color.Yellow);
            vectorRenderer.DrawLine(Vector3.Zero,
                                    transform.Forward * scale.Length() * 2.0f);

            vectorRenderer.SetColor(Color.Blue);
            vectorRenderer.DrawLine(Vector3.Zero,
                                    transform.Right * scale.Length() * 2.0f);

            graphicsDevice.RenderState.DepthBufferEnable = depthWasEnabled;
        }

        public static int GetBoneIndexByName(this Model model, string boneName)
        {
            for (int x = 0; x < model.Bones.Count; x++)
            {
                if (string.Compare(model.Bones[x].Name, boneName, true) == 0)
                {
                    return x;
                }
            }
            return -1;
        }

        public static BoundingBox GetBoundingBox(this Model model)
        {
            Vector3 min = Vector3.One * float.MaxValue;
            Vector3 max = Vector3.One * float.MinValue;

            Vector3[] verts = model.GetFlattenedPositionArray();

            for (int x = 0; x < verts.Length; x++)
            {
                min = Vector3.Min(verts[x], min);
                max = Vector3.Max(verts[x], max);
            }

            return new BoundingBox(min, max);
        }
        public static BoundingBox GetBoundingBox(this Model model, Matrix transform)
        {
            Vector3 min = Vector3.One * float.MaxValue;
            Vector3 max = Vector3.One * float.MinValue;

            Vector3[] verts = model.GetFlattenedPositionArray(transform);

            for (int x = 0; x < verts.Length; x++)
            {
                min = Vector3.Min(verts[x], min);
                max = Vector3.Max(verts[x], max);
            }

            return new BoundingBox(min, max);
        }
        public static BoundingBox GetBoundingBox(this Model model, ModelMesh mesh)
        {
            Vector3 min = Vector3.One * float.MaxValue;
            Vector3 max = Vector3.One * float.MinValue;

            Vector3[] verts = mesh.GetFlattenedPositionArray(
                model.GetAbsoluteBoneTransform(mesh.ParentBone.Index));

            for (int x = 0; x < verts.Length; x++)
            {
                min = Vector3.Min(verts[x], min);
                max = Vector3.Max(verts[x], max);
            }

            return new BoundingBox(min, max);
        }

        public static BoundingBox[] GetBoundingBoxes(this Model model)
        {
            BoundingBox[] ret = new BoundingBox[model.Meshes.Count];

            for (int x = 0; x < model.Meshes.Count; x++)
            {
                ModelMesh mesh = model.Meshes[x];

                Vector3 min = Vector3.One * float.MaxValue;
                Vector3 max = Vector3.One * float.MinValue;

                Vector3[] verts = mesh.GetFlattenedPositionArray(
                    model.GetAbsoluteBoneTransform(mesh.ParentBone.Index));

                for (int k = 0; k < verts.Length; k++)
                {
                    min = Vector3.Min(verts[k], min);
                    max = Vector3.Max(verts[k], max);
                }

                ret[x] = new BoundingBox(min, max);
            }
            return ret;
        }

        public static BoundingSphere GetBoundingSphere(this Model model)
        {
            BoundingSphere bs = model.Meshes[0].BoundingSphere;

            Matrix[] bones = model.GetAboluteBoneTransforms();

            foreach (ModelMesh mesh in model.Meshes)
            {
                bs = BoundingSphere.CreateMerged(bs, 
                    mesh.BoundingSphere.Transform(bones[mesh.ParentBone.Index]));
            }
            return bs;
        }
        public static BoundingSphere GetBoundingSphere(this Model model, ModelMesh mesh)
        {
            BoundingSphere bs = mesh.BoundingSphere;

            Matrix[] bones = model.GetAboluteBoneTransforms();

            foreach (ModelMesh m in model.Meshes)
            {
                if (m == mesh)
                    continue;

                bool IsChild = false;
                ModelBone bone = m.ParentBone;
                while (bone != null)
                {
                    if (bone.Name == mesh.Name)
                    {
                        IsChild = true;
                        break;
                    }
                    bone = bone.Parent;
                }
                if (IsChild)
                {
                    bs = BoundingSphere.CreateMerged(bs,
                        m.BoundingSphere.Transform(bones[m.ParentBone.Index]));
                }
            }
            return bs;
        }
        public static Matrix GetAbsoluteBoneTransform(this Model model, string boneName)
        {
            int boneIndex = model.GetBoneIndexByName(boneName);
            return GetAbsoluteBoneTransform(model, boneIndex);
        }

        public static Vector3 GetAbsoluteBoneTranslation(this Model model, string boneName)
        {
            int boneIndex = model.GetBoneIndexByName(boneName);
            return GetAbsoluteBoneTransform(model, boneIndex).Translation;
        }

        public static Matrix GetAbsoluteBoneTransform(this Model model, int boneIndex)
        {
            Matrix ret = Matrix.Identity;

            if (boneIndex != -1)
            {
                Matrix[] boneTransforms = model.GetAboluteBoneTransforms();

                ret = boneTransforms[boneIndex];
            }

            return ret;
        }

        public static Matrix[] GetAboluteBoneTransforms(this Model model)
        {
            Matrix[] bones = new Matrix[model.Bones.Count];
            model.CopyAbsoluteBoneTransformsTo(bones);
            return bones;
        }

        public static int VertexCount(this Model model)
        {
            int numVertices = 0;
            foreach (ModelMesh mesh in model.Meshes)
            {
                numVertices += VertexCount(mesh);
            }
            return numVertices;
        }

        public static int VertexCount(this ModelMesh mesh)
        {
            int numVertices = 0;
            foreach (ModelMeshPart meshPart in mesh.MeshParts)
            {
                if (meshPart.VertexDeclaration.IsPositionDecl())
                {
                    numVertices += meshPart.NumVertices;
                }
            }
            return numVertices;
        }

        public static int IndexCount(this Model model)
        {
            int numIndices = 0;
            foreach (ModelMesh mesh in model.Meshes)
            {
                numIndices += IndexCount(mesh);
            }
            return numIndices;
        }

        public static int IndexCount(this ModelMesh mesh)
        {
            int numIndices = 0;
            foreach (ModelMeshPart meshPart in mesh.MeshParts)
            {
                numIndices += meshPart.PrimitiveCount * 3;
            }
            return numIndices;
        }
    }
}
