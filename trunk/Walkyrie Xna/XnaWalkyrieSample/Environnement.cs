using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using XNAWalkyrie;

namespace XnaWalkyrieSample
{
    public class Environnement
    {
        enum Mur
        {
            Sol = 0,
            Sol_High,
            MurDroit,
            MurGauche,
            MurAvant,
            MurArriere,
            Nb_Mur,
        }

        private Texture2D floorColorMap;
        private Texture2D floorNormalMap;
        private Texture2D floorHeightMap;

        private Texture2D WoodColorMap;
        private Texture2D WoodNormalMap;
        private Texture2D WoodHeightMap;

        private Texture2D briqueColorMap;
        private Texture2D briqueNormalMap;
        private Texture2D briqueHeightMap;


        private Light light;
        private Material material;

        private XNAWalkyrie.Utility.NormalMappedQuad[] NormalMappedQuad;
        private XNAWalkyrie.Utility.NormalMappedModel NormalMappedModel;

        private Model modelTest;


        public Environnement(ref Light l, ref Material m)
        {
            light = l;
            material = m;


        }

        public void Initialize()
        {


            NormalMappedQuad = new XNAWalkyrie.Utility.NormalMappedQuad[(int)Mur.Nb_Mur];

            //########################################### SOL #################################################################

            NormalMappedQuad[(int)Mur.Sol] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(100.0f, -295.0f, -280.0f), Vector3.Up, Vector3.Forward,
                                                            1400.0f, 1200.0f, 20.0f, 15.0f);

            //########################################### SOL_HIGH #################################################################

            NormalMappedQuad[(int)Mur.Sol_High] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(-35.5f, -293.5f, 1.0f), Microsoft.Xna.Framework.Vector3.Up, Vector3.Forward,
                                                            391.0f, 322.0f, 10.0f, 5.0f);


            //########################################### RIGHT #################################################################

            NormalMappedQuad[(int)Mur.MurDroit] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(-528.0f, -258.0f, -280.0f), Vector3.Right, Vector3.Forward,
                                                            75.0f, 1100.0f, 20.0f, 1.0f);


            //########################################### LEFT #################################################################

            NormalMappedQuad[(int)Mur.MurGauche] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(686.0f, -258.0f, -280.0f), Vector3.Left, Vector3.Forward,
                                                            75.0f, 1100.0f, 20.0f, 1.0f);


            //########################################### BEHIND #################################################################

            NormalMappedQuad[(int)Mur.MurArriere] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(100.0f, -258.0f, -800.0f), Vector3.UnitZ, Vector3.Right,
                                                            75.0f, 1300.0f, 20.0f, 1.0f);


            //########################################### FORWARD #################################################################

            NormalMappedQuad[(int)Mur.MurAvant] = new XNAWalkyrie.Utility.NormalMappedQuad(ref light, ref material,
                                                            new Vector3(100.0f, -258.0f, 235.0f), -Vector3.UnitZ, Vector3.Right,
                                                            75.0f, 1300.0f, 20.0f, 1.0f);



        }

        public void LoadContent(ref ShadowMap shadowMapNear, ref ShadowMap shadowMapFar, float FocalShadowMapNear)
        {
            floorColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\floor_color_map");
            floorNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\floor_normal_map");
            floorHeightMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\floor_height_map");

            briqueColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\brick_color_map");
            briqueNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\brick_normal_map");
            briqueHeightMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\brick_height_map");

            //WoodColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\wood_color_map");
            WoodColorMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\woodplanks");
            //WoodNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\wood_normal_map");
            WoodNormalMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\woodplanks_normal");
            WoodHeightMap = Utility.Game.Content.Load<Texture2D>(@"Textures\ColorNormalHeightMap\wood_height_map");


            modelTest = Utility.Game.Content.Load<Model>(@"Models\box");



            NormalMappedModel = new XNAWalkyrie.Utility.NormalMappedModel(ref light, ref material, ref modelTest);
            NormalMappedModel.Initialize();

            NormalMappedModel.LoadContent();

            NormalMappedModel.FocaleCameraNear = FocalShadowMapNear;
            NormalMappedModel.ShadowMapNear = shadowMapNear;
            NormalMappedModel.ShadowMapFar = shadowMapFar;

            NormalMappedModel.textureColorMap = briqueColorMap;
            NormalMappedModel.textureNormalMap = briqueNormalMap;
            NormalMappedModel.textureHeightMap = briqueHeightMap;
            NormalMappedModel.World = Matrix.CreateTranslation(-300.0f, -295.0f, -200.0f);

            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].Initialize();
                NormalMappedQuad[i].LoadContent();
            }

            NormalMappedQuad[(int)Mur.Sol].textureColorMap = floorColorMap;
            NormalMappedQuad[(int)Mur.Sol].textureNormalMap = floorNormalMap;
            NormalMappedQuad[(int)Mur.Sol].textureHeightMap = floorHeightMap;
            NormalMappedQuad[(int)Mur.Sol].FocaleCameraNear = FocalShadowMapNear;
            NormalMappedQuad[(int)Mur.Sol].ShadowMapNear = shadowMapNear;
            NormalMappedQuad[(int)Mur.Sol].ShadowMapFar = shadowMapFar;

            NormalMappedQuad[(int)Mur.Sol_High].textureColorMap = WoodColorMap;
            NormalMappedQuad[(int)Mur.Sol_High].textureNormalMap = WoodNormalMap;
            NormalMappedQuad[(int)Mur.Sol_High].textureHeightMap = WoodHeightMap;
            NormalMappedQuad[(int)Mur.Sol_High].FocaleCameraNear = FocalShadowMapNear;
            NormalMappedQuad[(int)Mur.Sol_High].ShadowMapNear = shadowMapNear;
            NormalMappedQuad[(int)Mur.Sol_High].ShadowMapFar = shadowMapFar;

            for (int i = (int)Mur.MurDroit; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].textureColorMap = briqueColorMap;
                NormalMappedQuad[i].textureNormalMap = briqueNormalMap;
                NormalMappedQuad[i].textureHeightMap = briqueHeightMap;
                NormalMappedQuad[i].FocaleCameraNear = FocalShadowMapNear;
                NormalMappedQuad[i].ShadowMapNear = shadowMapNear;
                NormalMappedQuad[i].ShadowMapFar = shadowMapFar;
            }


        }


        public void UnloadContent()
        {

        }

        public void Update(GameTime gameTime)
        {

            NormalMappedModel.Update(gameTime);
            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].Update(gameTime);
            }


        }

        public void Draw(GameTime gameTime)
        {

            for (int i = 0; i < (int)Mur.Nb_Mur; i++)
            {
                NormalMappedQuad[i].Draw(gameTime);
            }

            NormalMappedModel.Draw(gameTime);
        }
    }
}
