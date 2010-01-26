xof 0303txt 0032
template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}


Mesh {
 24;
 -4.000000;-3.000000;6.000000;,
 4.000000;-3.000000;6.000000;,
 -4.000000;-3.000000;-6.000000;,
 4.000000;-3.000000;-6.000000;,
 -4.000000;3.000000;6.000000;,
 4.000000;3.000000;6.000000;,
 -4.000000;-3.000000;6.000000;,
 4.000000;-3.000000;6.000000;,
 4.000000;3.000000;6.000000;,
 4.000000;3.000000;-6.000000;,
 4.000000;-3.000000;6.000000;,
 4.000000;-3.000000;-6.000000;,
 4.000000;3.000000;-6.000000;,
 -4.000000;3.000000;-6.000000;,
 4.000000;-3.000000;-6.000000;,
 -4.000000;-3.000000;-6.000000;,
 -4.000000;3.000000;-6.000000;,
 -4.000000;3.000000;6.000000;,
 -4.000000;-3.000000;-6.000000;,
 -4.000000;-3.000000;6.000000;,
 -4.000000;3.000000;-6.000000;,
 4.000000;3.000000;-6.000000;,
 -4.000000;3.000000;6.000000;,
 4.000000;3.000000;6.000000;;
 12;
 3;0,1,2;,
 3;1,3,2;,
 3;4,5,6;,
 3;5,7,6;,
 3;8,9,10;,
 3;9,11,10;,
 3;12,13,14;,
 3;13,15,14;,
 3;16,17,18;,
 3;17,19,18;,
 3;20,21,22;,
 3;21,23,22;;

 MeshNormals {
  24;
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;-1.000000;0.000000;;
  12;
  3;0,1,2;,
  3;1,3,2;,
  3;4,5,6;,
  3;5,7,6;,
  3;8,9,10;,
  3;9,11,10;,
  3;12,13,14;,
  3;13,15,14;,
  3;16,17,18;,
  3;17,19,18;,
  3;20,21,22;,
  3;21,23,22;;
 }

 MeshTextureCoords {
  24;
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.500000;,
  1.000000;1.500000;,
  0.000000;0.000000;,
  2.000000;0.000000;,
  0.000000;1.500000;,
  2.000000;1.500000;,
  0.000000;0.000000;,
  3.000000;0.000000;,
  0.000000;1.500000;,
  3.000000;1.500000;,
  0.000000;0.000000;,
  2.000000;0.000000;,
  0.000000;1.500000;,
  2.000000;1.500000;,
  0.000000;0.000000;,
  3.000000;0.000000;,
  0.000000;1.500000;,
  3.000000;1.500000;,
  -0.150000;0.000000;,
  1.250000;0.000000;,
  -0.150000;2.100000;,
  1.250000;2.100000;;
 }

 MeshMaterialList {
  3;
  12;
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2;

  Material {
   0.600000;0.600000;1.000000;1.000000;;
   40.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;

   TextureFilename {
    "cellfloor.jpg";
   }
  }

  Material {
   0.600000;0.600000;1.000000;1.000000;;
   40.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;

   TextureFilename {
    "cellwall.jpg";
   }
  }

  Material {
   1.000000;1.000000;1.000000;1.000000;;
   40.000000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;

   TextureFilename {
    "cellceiling.jpg";
   }
  }
 }
}