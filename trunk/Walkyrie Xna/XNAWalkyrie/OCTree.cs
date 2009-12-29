using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;


namespace XNAWalkyrie
{

    public struct VisibleOCTreeNode : IComparable<VisibleOCTreeNode>
    {
        public float DistanceSquared;
        public OCTreeNode Node;

        public VisibleOCTreeNode(float distSquared, OCTreeNode node)
        {
            this.DistanceSquared = distSquared;
            this.Node = node;
        }

        public int CompareTo(VisibleOCTreeNode other)
        {
            if (DistanceSquared > other.DistanceSquared)
                return 1;
            else if (DistanceSquared < other.DistanceSquared)
                return -1;
            else
                return 0;
        }
    }

    public class OCTreeNode
    {
        public BoundingBox Bounds;
        public Vector3 Center;
        public Vector3[] Vertices = null;
        public int[] Indices = null;
        public Plane[] Planes = null;
        public OCTreeNode[] Children = null;

        

        public bool FrustumCull(
            BoundingFrustum frustum, 
            ref Vector3 cameraPosition,
            ref List<VisibleOCTreeNode> visibleNodeList,
            bool cull)
        {
            ContainmentType t = ContainmentType.Contains;

            if(cull)
                t = frustum.Contains(Bounds);

            if (t != ContainmentType.Disjoint)
            {
                if (Indices != null)
                {
                    visibleNodeList.Add(
                        new VisibleOCTreeNode(
                            Vector3.DistanceSquared(cameraPosition, Center),
                            this));
                }

                if (Children != null)
                {
                    for (int x = 0; x < Children.Length; x++)
                    {
                        Children[x].FrustumCull(
                            frustum, 
                            ref cameraPosition, 
                            ref visibleNodeList,
                            t == ContainmentType.Intersects);
                    }
                }
            }

            return visibleNodeList.Count > 0;
        }


        public bool GetIntersectingPolygon(
            ref Ray ray,
            ref OCTreeIntersection closest)
        {
            if (Indices != null)
            {
                float? collVal = ray.Intersects(Bounds);
                if (collVal.HasValue)
                {
                    float closestDist = closest.IntersectType == OCTreeIntersectionType.None ? 
                        float.MaxValue : closest.DistanceSquared;

                    float t, u, v;
                    if (collVal.Value < closestDist)
                    {
                        for (int polyIndex = 0; polyIndex < Indices.Length / 3; polyIndex++)
                        {
                            if (Intersection.RayTriangleIntersect(
                                ray.Position, 
                                ray.Direction,
                                Vertices[Indices[polyIndex * 3 + 0]],
                                Vertices[Indices[polyIndex * 3 + 1]],
                                Vertices[Indices[polyIndex * 3 + 2]],
                                out t, out u, out v))
                            {
                                Vector3 intersection =
                                    ray.Position + ray.Direction * t;

                                float dist = Vector3.DistanceSquared(
                                    intersection, 
                                    ray.Position);

                                if (dist < closestDist)
                                {
                                    closestDist = dist;
                                    
                                    closest.IntersectionNormal =
                                        MathExtensions.NormalFromPoints(
                                            Vertices[Indices[polyIndex * 3 + 0]],
                                            Vertices[Indices[polyIndex * 3 + 1]],
                                            Vertices[Indices[polyIndex * 3 + 2]]);

                                    closest.IntersectionPoint = intersection;
                                    closest.Node = this;
                                    closest.PolygonIndex = polyIndex;
                                    closest.DistanceSquared = dist;
                                    
                                    closest.IntersectType = OCTreeIntersectionType.Inside;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (Children != null)
                {
                    for (int x = 0; x < Children.Length; x++)
                    {
                        Children[x].GetIntersectingPolygon(ref ray, ref closest);
                    }
                }
            }

            return closest.IntersectType != OCTreeIntersectionType.None;
        }


        public bool GetIntersectingNodes(
            ref BoundingBox bbox,
            ref List<OCTreeNode> nodes)
        {
            if(bbox.Intersects(Bounds))
            {
                if (Indices != null)
                {
                    nodes.Add(this);
                }
            
                if (Children != null)
                {
                    for (int x = 0; x < Children.Length; x++)
                    {
                        Children[x].GetIntersectingNodes(ref bbox, ref nodes);
                    }
                }
            }

            return nodes.Count > 0;
        }

        public bool GetIntersectingNodes(
            ref BoundingSphere sphere,
            ref List<OCTreeNode> nodes)
        {
            if (sphere.Intersects(Bounds))
            {
                if (Indices != null)
                {
                    nodes.Add(this);
                }

                if (Children != null)
                {
                    for (int x = 0; x < Children.Length; x++)
                    {
                        Children[x].GetIntersectingNodes(ref sphere, ref nodes);
                    }
                }
            }

            return nodes.Count > 0;
        }

        public bool UpdateIntersection(
            ref OCTreeIntersection intersection,
            ref BoundingSphere sphere,
            ref Vector3 velocityNormal)
        {
            OCTreeIntersection isec;

            if (GetIntersection(
                ref sphere,
                ref velocityNormal,
                intersection.PolygonIndex,
                out isec))
            {
                intersection = isec;
            }
            else
            {
                intersection.IntersectType = OCTreeIntersectionType.None;
            }
            return intersection.IntersectType != OCTreeIntersectionType.None;
        }

        public bool GetIntersectingPolygons(
            ref BoundingSphere sphere,
            ref Vector3 velocityNormal,
            ref List<OCTreeIntersection> nodes)
        {
            if (Bounds.Contains(sphere) != ContainmentType.Disjoint)
            {
                if (Indices != null)
                {
                    for (int polyIndex = 0; polyIndex < Indices.Length / 3; polyIndex++)
                    {
                        OCTreeIntersection isec;
                        if (GetIntersection(
                            ref sphere,
                            ref velocityNormal,
                            polyIndex,
                            out isec))
                        {
                            nodes.Add(isec);
                        }
                    }
                }

                if (Children != null)
                {
                    for (int x = 0; x < Children.Length; x++)
                    {
                        Children[x].GetIntersectingPolygons(
                            ref sphere,
                            ref velocityNormal,
                            ref nodes);
                    }
                }
            }

            return nodes.Count > 0;
        }

        public bool GetIntersection(
            ref BoundingSphere sphere, 
            ref Vector3 velocityNormal,
            int polyIndex,
            out OCTreeIntersection intersection)
        {
            bool ret = false;

            intersection = default(OCTreeIntersection);

            Plane p = Planes[polyIndex];

            if (p.DotNormal(velocityNormal) >= 0)
                return ret;

            if (p.DotCoordinate(sphere.Center) < 0)
                return ret;

            if (p.Intersects(sphere) != PlaneIntersectionType.Intersecting)
                return ret;

            Vector3 vA = Vertices[Indices[polyIndex * 3 + 0]];
            Vector3 vB = Vertices[Indices[polyIndex * 3 + 1]];
            Vector3 vC = Vertices[Indices[polyIndex * 3 + 2]];

            Vector3 vOnPlane =
                Intersection.ClosestPointOnPlane(sphere.Center, p);

            if (Intersection.PointInTriangle(vOnPlane, vA, vB, vC))
            {
                intersection.IntersectionPoint = vOnPlane;
                intersection.IntersectionNormal = p.Normal;
                intersection.IntersectionDepth =
                    sphere.Radius - Vector3.Distance(vOnPlane, sphere.Center);
                intersection.Node = this;
                intersection.PolygonIndex = polyIndex;
                intersection.IntersectType = OCTreeIntersectionType.Inside;
                
                ret = true;
            }
            else if (sphere.Contains(vA) != ContainmentType.Disjoint)
            {
                intersection.IntersectionPoint = vA;
                intersection.IntersectionNormal =
                    Vector3.Normalize(sphere.Center - vA);
                intersection.IntersectionDepth =
                    sphere.Radius - Vector3.Distance(vA, sphere.Center);
                intersection.Node = this;
                intersection.PolygonIndex = polyIndex;
                intersection.IntersectType = OCTreeIntersectionType.Point;
                
                ret = true;
            }
            else if (sphere.Contains(vB) != ContainmentType.Disjoint)
            {
                intersection.IntersectionPoint = vB;
                intersection.IntersectionNormal =
                    Vector3.Normalize(sphere.Center - vB);
                intersection.IntersectionDepth =
                    sphere.Radius - Vector3.Distance(vB, sphere.Center);
                intersection.Node = this;
                intersection.PolygonIndex = polyIndex;
                intersection.IntersectType = OCTreeIntersectionType.Point;
                
                ret = true;
            }
            else if (sphere.Contains(vC) != ContainmentType.Disjoint)
            {
                intersection.IntersectionPoint = vC;
                intersection.IntersectionNormal =
                    Vector3.Normalize(sphere.Center - vC);
                intersection.IntersectionDepth =
                    sphere.Radius - Vector3.Distance(vC, sphere.Center);
                intersection.Node = this;
                intersection.PolygonIndex = polyIndex;
                intersection.IntersectType = OCTreeIntersectionType.Point;
                
                ret = true;
            }
            else
            {
                float t; Vector3 d;

                Intersection.ClosestPointOnSegment(sphere.Center,
                    vA, vB, out t, out d);

                if (sphere.Contains(d) != ContainmentType.Disjoint)
                {
                    intersection.IntersectionPoint = d;
                    intersection.IntersectionNormal =
                        Vector3.Normalize(sphere.Center - d);
                    intersection.IntersectionDepth =
                        sphere.Radius - Vector3.Distance(d, sphere.Center);
                    intersection.IntersectType = OCTreeIntersectionType.Edge;
                    intersection.Node = this;
                    intersection.PolygonIndex = polyIndex;
                    
                    ret = true;
                }
                else
                {
                    Intersection.ClosestPointOnSegment(sphere.Center,
                        vB, vC, out t, out d);

                    if (sphere.Contains(d) != ContainmentType.Disjoint)
                    {
                        intersection.IntersectionPoint = d;
                        intersection.IntersectionNormal =
                            Vector3.Normalize(sphere.Center - d);
                        intersection.IntersectionDepth =
                            sphere.Radius - Vector3.Distance(d, sphere.Center);
                        intersection.IntersectType = OCTreeIntersectionType.Edge;
                        intersection.Node = this;
                        intersection.PolygonIndex = polyIndex;
                        
                        ret = true;
                    }
                    else
                    {
                        Intersection.ClosestPointOnSegment(sphere.Center,
                            vC, vA, out t, out d);

                        if (sphere.Contains(d) != ContainmentType.Disjoint)
                        {
                            intersection.IntersectionPoint = d;
                            intersection.IntersectionNormal =
                                Vector3.Normalize(sphere.Center - d);
                            intersection.IntersectionDepth =
                                sphere.Radius - Vector3.Distance(d, sphere.Center);
                            intersection.IntersectType = OCTreeIntersectionType.Edge;
                            intersection.Node = this;
                            intersection.PolygonIndex = polyIndex;
                            
                            ret = true;
                        }
                    }
                }
            }
            return ret;
        }

        #region DrawDebug
#if DEBUG
        Utility.VertexPositionNormal[] DrawVerts;

        public void DrawDebugPolygons(GraphicsDevice device)
        {
            if(Indices != null && DrawVerts == null)
            {
                DrawVerts = new Utility.VertexPositionNormal[Indices.Length];
                
                for (int x = 0; x < Indices.Length / 3; x++)
                {
                    DrawVerts[x * 3 + 0].Position = Vertices[Indices[x * 3 + 0]];
                    DrawVerts[x * 3 + 1].Position = Vertices[Indices[x * 3 + 1]];
                    DrawVerts[x * 3 + 2].Position = Vertices[Indices[x * 3 + 2]];

                    DrawVerts[x * 3 + 0].Normal =
                        DrawVerts[x * 3 + 1].Normal =
                        DrawVerts[x * 3 + 2].Normal =
                            MathExtensions.NormalFromPoints(
                                DrawVerts[x * 3 + 0].Position,
                                DrawVerts[x * 3 + 1].Position,
                                DrawVerts[x * 3 + 2].Position);
                }
            }

            if (DrawVerts != null)
            {
                
                device.DrawUserPrimitives<Utility.VertexPositionNormal>(PrimitiveType.TriangleList,
                    DrawVerts, 0, DrawVerts.Length / 3);
                
            }
            else
            {
                if (Children != null)
                {
                    for (int x = 0; x < Children.Length; x++)
                    {
                        Children[x].DrawDebugPolygons(device);
                    }
                }
            }
        }

        public void DrawDebugBounds(GraphicsDevice device)
        {
            Utility.VectorRenderer.DrawBoundingBox(this.Bounds);

            if (Children != null)
            {
                for (int x = 0; x < Children.Length; x++)
                {
                    Children[x].DrawDebugBounds(device);
                }
            }
        }
#endif
        #endregion
    }

    public enum OCTreeIntersectionType
    {
        None,
        Inside,
        Edge,
        Point,
    }

    public struct OCTreeIntersection : IComparable<OCTreeIntersection>
    {
        public OCTreeNode Node;
        public int PolygonIndex;
        public Vector3 IntersectionPoint;
        public Vector3 IntersectionNormal;
        public float DistanceSquared;
        public float IntersectionDepth;
        public OCTreeIntersectionType IntersectType;
        
        public int CompareTo(OCTreeIntersection other)
        {
            if (IntersectionDepth < other.IntersectionDepth)
                return 1;
            else if (IntersectionDepth > other.IntersectionDepth)
                return -1;
            else 
                return 0;
        }

        #region DrawDebug
#if DEBUG

        public void DrawDebug()
        {
            if (IntersectType == OCTreeIntersectionType.Point)
            {
                Utility.VectorRenderer.SetColor(Color.Red);
            }
            if (IntersectType == OCTreeIntersectionType.Edge)
            {
                Utility.VectorRenderer.SetColor(Color.Green);
            }
            if (IntersectType == OCTreeIntersectionType.Inside)
            {
                Utility.VectorRenderer.SetColor(Color.Yellow);
            }

            Utility.VectorRenderer.DrawBoundingSphere(
                    new BoundingSphere(IntersectionPoint,
                        5.0f));

            Utility.VectorRenderer.SetColor(Color.HotPink);
            Utility.VectorRenderer.DrawLine(
                IntersectionPoint,
                IntersectionPoint +
                IntersectionNormal * 10);
        }
#endif
        #endregion

    }


    public class OCTree
    {
        public OCTreeNode Root;

        public bool GetIntersectingPolygon(
            Ray ray,
            Matrix world,
            out OCTreeIntersection intersection)
        {
            intersection = default(OCTreeIntersection);

            Matrix worldInverse = Matrix.Invert(world);

            ray.Position = Vector3.Transform(ray.Position, worldInverse);
            ray.Direction = Vector3.TransformNormal(ray.Direction, worldInverse);
            ray.Direction.Normalize();
            
            bool intersects = Root.GetIntersectingPolygon(ref ray, ref intersection);

            if (intersects)
            {
                intersection.IntersectionPoint =
                    Vector3.Transform(intersection.IntersectionPoint, world);

                intersection.IntersectionNormal =
                    Vector3.TransformNormal(intersection.IntersectionNormal, world);
                intersection.IntersectionNormal.Normalize();
                intersection.IntersectType = OCTreeIntersectionType.Inside;
                intersection.DistanceSquared =
                    Vector3.DistanceSquared(
                    ray.Position, 
                    intersection.IntersectionPoint);
            }

            return intersects;
        }
        public bool GetIntersectingPolygon(
            ref Ray ray,
            out OCTreeIntersection intersection)
        {
            intersection = default(OCTreeIntersection);
            return Root.GetIntersectingPolygon(ref ray, ref intersection);
        }

        public bool GetIntersectingNodes(
            ref BoundingBox bbox,
            ref List<OCTreeNode> intersection)
        {
            return Root.GetIntersectingNodes(ref bbox, ref intersection);
        }

        public bool GetIntersectingNodes(
            ref BoundingSphere sphere,
            ref Matrix world,
            ref List<OCTreeNode> intersection)
        {
            sphere.Center = Vector3.Transform(
                sphere.Center, 
                Matrix.Invert(world));

            return Root.GetIntersectingNodes(ref sphere, ref intersection);

        }
        public bool GetIntersectingNodes(
            ref BoundingSphere sphere,
            ref List<OCTreeNode> intersection)
        {
            return Root.GetIntersectingNodes(ref sphere, ref intersection);
        }
        public bool GetIntersectingPolygons(
            ref BoundingSphere sphere,
            ref Vector3 velocityNormal,
            ref List<OCTreeIntersection> intersection)
        {
            bool ret = Root.GetIntersectingPolygons(
                ref sphere, 
                ref velocityNormal,
                ref intersection);

            if (ret)
            {
                intersection.Sort();
            }
            return ret;
        }

        public void MoveSphere(
            ref BoundingSphere sphere,
            ref Vector3 sphereVelocity,
            float Friction)
        {
            List<OCTreeIntersection> sphereColliders = new List<OCTreeIntersection>();
            MoveSphere(ref sphere, ref sphereVelocity, Friction, ref sphereColliders);
        }

        public void MoveSphere(
            ref BoundingSphere sphere, 
            ref Vector3 sphereVelocity,
            float Friction, 
            ref List<OCTreeIntersection> sphereColliders)
        {

            Vector3 velocity = sphereVelocity;
            Vector3 velocityNormal = Vector3.Normalize(velocity);

            BoundingSphere newBounds = new BoundingSphere(
                    sphere.Center + velocity,
                    sphere.Radius);


            List<OCTreeIntersection> colliders
                    = new List<OCTreeIntersection>();

            newBounds.Radius = sphere.Radius + velocity.Length();
            GetIntersectingPolygons(
                ref newBounds,
                ref velocityNormal,
                ref colliders);
/*
            if (velocity.Length() > sphere.Radius)
                newBounds.Center = sphere.Center;
*/
            newBounds.Radius = sphere.Radius;
/*
            bool bCollide = false;
            do
            {
                if (velocity.Length() > sphere.Radius)
                    newBounds.Center = newBounds.Center + velocityNormal * sphere.Radius;
                    
                float fDist = Vector3.Distance(newBounds.Center, sphere.Center);
                if (fDist > velocity.Length())
                    newBounds.Center = sphere.Center + velocity;
*/            
                for (int i = 0; i < 5; i++)
                {
                    int numCollisions = 0;
                    for (int x = 0; x < colliders.Count; x++)
                    {
                        OCTreeIntersection isec = colliders[x];

                        if (isec.Node.UpdateIntersection(
                            ref isec,
                            ref newBounds,
                            ref velocityNormal))
                        {

                            if (newBounds.Contains(isec.IntersectionPoint) !=
                                ContainmentType.Disjoint)
                            {
                                numCollisions++;
                                Vector3 adjustment = isec.IntersectionNormal *
                                    (isec.IntersectionDepth + 0.001f);

                                newBounds.Center += adjustment;

                                velocity = newBounds.Center - sphere.Center;
                                velocityNormal = Vector3.Normalize(velocity);

                                bool found = false;
                                for (int c = 0; c < sphereColliders.Count; c++)
                                {
                                    if (sphereColliders[c].Node == isec.Node &&
                                        sphereColliders[c].PolygonIndex == isec.PolygonIndex)
                                    {
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found)
                                {
                                    sphereColliders.Add(isec);
                                }
                            }
                        }
                    }
                    if (numCollisions == 0)
                    {
                        break;
                    }
                }
/*
            }
            while (!bCollide && newBounds.Center != sphere.Center + velocity);
*/


            bool appliedFriction = false;
            for (int c = 0; c < sphereColliders.Count; c++)
            {
                if (Vector3.Dot(sphereColliders[c].IntersectionNormal,
                    Vector3.Up) > 0.5f)
                {
                    velocity -= (velocity * (Vector3.One - Vector3.Up)) * Friction;
                    appliedFriction = true;
                    break;
                }
            }
            if (appliedFriction == false)
            {
                velocity -= (velocity * (Vector3.One - Vector3.Up)) * Friction * 0.5f;
            }
            sphereVelocity = velocity;

            sphere = newBounds;
        }

        public bool FrustumCull(
            BoundingFrustum frustum,
            Vector3 cameraPosition,
            ref Matrix world,
            ref List<VisibleOCTreeNode> visibleNodeList)
        {

            frustum.Matrix = world * frustum.Matrix;

            return Root.FrustumCull(
                frustum, 
                ref cameraPosition,
                ref visibleNodeList,
                true);
        }
        public bool FrustumCull(
            BoundingFrustum frustum,
            Vector3 cameraPosition,
            ref List<VisibleOCTreeNode> visibleNodeList)
        {
            return Root.FrustumCull(
                frustum,
                ref cameraPosition,
                ref visibleNodeList,
                true);
        }
        #region DrawDebug
#if DEBUG
        BasicEffect effect;
        VertexDeclaration decl;
        public int DrawDebugPolygons()
        {
            return DrawDebugPolygons(Matrix.Identity);
        }

        public int DrawDebugPolygons(Matrix world)
        {
            List<VisibleOCTreeNode> visibleNodes = new List<VisibleOCTreeNode>();


            if (FrustumCull(
                Utility.Camera.Frustum,
                Utility.Camera.Position, 
                ref world,
                ref visibleNodes))
            {
                visibleNodes.Sort();

                if (effect == null)
                {
                    decl = new VertexDeclaration(
                        Utility.GraphicsDevice,
                        Utility.VertexPositionNormal.VertexElements);

                    effect = new BasicEffect(Utility.GraphicsDevice, new EffectPool());
                    effect.EnableDefaultLighting();
                }

                Utility.GraphicsDevice.VertexDeclaration = decl;

                effect.View = Utility.Camera.ViewMatrix;
                effect.Projection = Utility.Camera.ProjectionMatrix;
                effect.World = world;
                
                effect.EnableDefaultLighting();
                effect.Begin();

                effect.Techniques[0].Passes[0].Begin();

                for (int x = 0; x < visibleNodes.Count; x++)
                {
                    visibleNodes[x].Node.DrawDebugPolygons(Utility.GraphicsDevice);
                }

                effect.Techniques[0].Passes[0].End();
                effect.End();
            }

            return visibleNodes.Count;
        }

        public void DrawDebugBounds()
        {
            DrawDebugBounds(Matrix.Identity);
        }

        public void DrawDebugBounds(Matrix world)
        {
            Utility.VectorRenderer.SetWorldMatrix(world);
            Utility.VectorRenderer.SetColor(Color.Red);

            List<VisibleOCTreeNode> visibleNodes = new List<VisibleOCTreeNode>();

            Root.DrawDebugBounds(Utility.GraphicsDevice);
        }
#endif
        #endregion

    }

    public class IntermediateOCTreeNode
    {
        public BoundingBox Bounds;

        public OCTPoly[] Polys;

        public IntermediateOCTreeNode[] Children;

        public OCTreeNode Build(float WeldDelta)
        {
            OCTreeNode n = new OCTreeNode();
            n.Bounds = this.Bounds;
            n.Center = (Bounds.Min + Bounds.Max) * 0.5f;


            if (Polys != null &&
                Polys.Length > 0)
            {
                
                List<Vector3> newVerts = new List<Vector3>();
                List<int> newIndices = new List<int>();
                
                for (int x = 0; x < Polys.Length; x++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        Vector3 polyVert = Polys[x].Verts[k];

                        int foundIndex = -1;
                        for (int j = 0; j < newVerts.Count; j++)
                        {
                            if (newVerts[j].NearlyEqual(polyVert, WeldDelta))
                            {
                                foundIndex = j;
                                break;
                            }
                        }

                        if (foundIndex != -1)
                        {
                            newIndices.Add(foundIndex);
                        }
                        else
                        {
                            newVerts.Add(Polys[x].Verts[k]);
                            newIndices.Add(newVerts.Count - 1);
                        }
                    }
                }

                List<int> validIndices = new List<int>();
                for (int x = 0; x < newIndices.Count/ 3; x++)
                {
                    if (Vector3.DistanceSquared(newVerts[newIndices[x * 3 + 0]],
                        newVerts[newIndices[x * 3 + 1]]) > MathExtensions.Epsilon &&
                        Vector3.DistanceSquared(newVerts[newIndices[x * 3 + 1]],
                        newVerts[newIndices[x * 3 + 2]]) > MathExtensions.Epsilon &&
                        Vector3.DistanceSquared(newVerts[newIndices[x * 3 + 2]],
                        newVerts[newIndices[x * 3 + 0]]) > MathExtensions.Epsilon)
                    {
                        Plane p = new Plane(newVerts[newIndices[x * 3 + 0]],
                            newVerts[newIndices[x * 3 + 1]],
                            newVerts[newIndices[x * 3 + 2]]);

                        if (!float.IsNaN(p.Normal.X))
                        {
                            validIndices.Add(newIndices[x * 3 + 0]);
                            validIndices.Add(newIndices[x * 3 + 1]);
                            validIndices.Add(newIndices[x * 3 + 2]);
                        }
                        
                    }
                    
                }

                n.Indices = validIndices.ToArray();
                n.Vertices = newVerts.ToArray();

                n.Planes = new Plane[n.Indices.Length / 3];
                for (int x = 0; x < n.Indices.Length / 3; x++)
                {
                    n.Planes[x] = new Plane(
                        n.Vertices[n.Indices[x * 3 + 0]],
                        n.Vertices[n.Indices[x * 3 + 2]],
                        n.Vertices[n.Indices[x * 3 + 1]]);
                }
            }


            if (Children != null)
            {
                n.Children = new OCTreeNode[Children.Length];
                for (int x = 0; x < Children.Length; x++)
                {
                    n.Children[x] = Children[x].Build(WeldDelta);
                }
            }


            return n;
        }
    }
    public struct OCTPoly
    {
        public Vector3[] Verts;
        public OCTPoly(Vector3 a, Vector3 b, Vector3 c)
        {
            Verts = new Vector3[3];
            Verts[0] = a;
            Verts[1] = b;
            Verts[2] = c;
        }

        public OCTPoly(OCTPoly poly)
        {
            Verts = new Vector3[3];
            Verts[0] = poly.Verts[0];
            Verts[1] = poly.Verts[1];
            Verts[2] = poly.Verts[2];
        }
    }


    public class OCTreeBuilder
    {
        public struct ModelAndMatrix
        {
            public Model Model;
            public Matrix Matrix;
        }

        int MaxPolygonsPerNode;
        int MaxSubdivisions;
        bool MakeSquare;
        float WeldDelta;
        float ExpandBoundsFactor;

        public OCTreeBuilder(
            int maxPolygonsPerNode,
            int maxSubdivisions,
            bool makeSquare,
            float weldDelta,
            float expandBoundsFactor)
        {
            this.MaxPolygonsPerNode = maxPolygonsPerNode;
            this.MaxSubdivisions = maxSubdivisions;
            this.MakeSquare = makeSquare;
            this.WeldDelta = weldDelta;
            this.ExpandBoundsFactor = expandBoundsFactor;
        }


        


        Vector3[] nodeOffsets = new Vector3[8] 
        { 
            (Vector3.Up + Vector3.Forward + Vector3.Left) ,
            (Vector3.Up + Vector3.Forward + Vector3.Right) ,
            (Vector3.Up + Vector3.Backward + Vector3.Left) ,
            (Vector3.Up + Vector3.Backward + Vector3.Right) ,
            (Vector3.Down + Vector3.Forward + Vector3.Left) ,
            (Vector3.Down + Vector3.Forward + Vector3.Right) ,
            (Vector3.Down + Vector3.Backward + Vector3.Left) ,
            (Vector3.Down + Vector3.Backward + Vector3.Right) ,
        };
        Vector3[] planeOffsets = new Vector3[6] 
        { 
            Vector3.Forward,
            Vector3.Backward,
            Vector3.Left,
            Vector3.Right,
            Vector3.Up,
            Vector3.Down,
        };
        Vector3[] planeNormals = new Vector3[6] 
        { 
            Vector3.Backward,
            Vector3.Forward,
            Vector3.Right,
            Vector3.Left,
            Vector3.Down,
            Vector3.Up,
        };

        BoundingBox CalculateBounds(OCTPoly[] polyList)
        {
            Vector3 min = Vector3.One * float.MaxValue;
            Vector3 max = Vector3.One * float.MinValue;

            for (int x = 0; x < polyList.Length; x++)
            {
                for (int i = 0; i < 3; i++)
                {
                    Vector3.Min(ref min, ref polyList[x].Verts[i], out min);
                    Vector3.Max(ref max, ref polyList[x].Verts[i], out max);
                }
            }

            return new BoundingBox(min, max);
        }

        public OCTree Build(List<ModelAndMatrix> ListmodelAndMatrix)
        {
            
             List<OCTPoly> MeshPolyList = new List<OCTPoly>();

             foreach (ModelAndMatrix ModelAndMatrix in ListmodelAndMatrix)
             {
                 Vector3[] Vertices = ModelAndMatrix.Model.GetFlattenedPositionArray();
                 int[] Indices = ModelAndMatrix.Model.GetFlattenedIndexArray();

                 
                 for (int x = 0; x < Indices.Length / 3; x++)
                 {
                     Vector3 A = Vector3.Transform(Vertices[Indices[x * 3 + 0]], ModelAndMatrix.Matrix);
                     Vector3 B = Vector3.Transform(Vertices[Indices[x * 3 + 1]], ModelAndMatrix.Matrix);
                     Vector3 C = Vector3.Transform(Vertices[Indices[x * 3 + 2]], ModelAndMatrix.Matrix);

                     MeshPolyList.Add(new OCTPoly(A, B, C));
                 }
             }


            OCTPoly[] PolyList = MeshPolyList.ToArray();

            BoundingBox bbox = CalculateBounds(PolyList);
            
            Vector3 center = (bbox.Max + bbox.Min) * 0.5f;
            Vector3 extents = bbox.Max - center;

            if (MakeSquare)
            {
                float f = extents.X;
                if (extents.Y > f)
                    f = extents.Y;
                if (extents.Z > f)
                    f = extents.Z;

                extents.X = extents.Y = extents.Z = f;
            }
            extents += extents * ExpandBoundsFactor;

            bbox.Min = center - extents;
            bbox.Max = center + extents;

            IntermediateOCTreeNode node = new IntermediateOCTreeNode();
            node.Bounds = bbox;
            
            RecursiveBuild(node, PolyList, center, extents, 0);

            OCTree ret = new OCTree();
            ret.Root = node.Build(WeldDelta);

            return ret;
        }

        void RecursiveBuild(
            IntermediateOCTreeNode node, 
            OCTPoly[] PolyList, 
            Vector3 center, 
            Vector3 extents, 
            int depth)
        {
            Plane[] planes = new Plane[6]
            {
                MathExtensions.PlaneFromPointNormal(
                    center + planeOffsets[0] * extents, planeNormals[0]),
                MathExtensions.PlaneFromPointNormal(
                    center + planeOffsets[1] * extents, planeNormals[1]),
                MathExtensions.PlaneFromPointNormal(
                    center + planeOffsets[2] * extents, planeNormals[2]),
                MathExtensions.PlaneFromPointNormal(
                    center + planeOffsets[3] * extents, planeNormals[3]),
                MathExtensions.PlaneFromPointNormal(
                    center + planeOffsets[4] * extents, planeNormals[4]),
                MathExtensions.PlaneFromPointNormal(
                    center + planeOffsets[5] * extents, planeNormals[5]),
            };

            
            OCTPoly[] PolyCopy = new OCTPoly[PolyList.Length];
            for (int x = 0; x < PolyList.Length; x++)
                PolyCopy[x] = new OCTPoly(PolyList[x]);


            
            for (int planeIndex = 0; planeIndex < planes.Length; planeIndex++)
            {
                List<OCTPoly> tmpPolyList = new List<OCTPoly>();

                for (int polyIndex = 0; polyIndex < PolyCopy.Length; polyIndex++)
                {
                    int numPolys;
                    OCTPoly[] tmpNewPolys = new OCTPoly[2];
                    Split(
                        PolyCopy[polyIndex], 
                        planes[planeIndex], 
                        ref tmpNewPolys, 
                        out numPolys);
                     

                    for (int x = 0; x < numPolys; x++)
                    {
                        Plane p = new Plane(tmpNewPolys[x].Verts[0],
                            tmpNewPolys[x].Verts[1],
                            tmpNewPolys[x].Verts[2]);

                        if (!float.IsNaN(p.Normal.X))
                        {
                            tmpPolyList.Add(tmpNewPolys[x]);
                        }
                    }
                }

                PolyCopy = new OCTPoly[tmpPolyList.Count];
                for (int x = 0; x < tmpPolyList.Count; x++)
                    PolyCopy[x] = new OCTPoly(tmpPolyList[x]);
                  

            }
            
            if (PolyCopy.Length <= this.MaxPolygonsPerNode ||
                depth >= this.MaxSubdivisions)
            {
                node.Polys = new OCTPoly[PolyCopy.Length];

                if (PolyCopy.Length > 0)
                {
                    for (int x = 0; x < PolyCopy.Length; x++)
                    {
                        node.Polys[x] = new OCTPoly(PolyCopy[x]);
                    }
                }
            }
            else
            {
                node.Children = new IntermediateOCTreeNode[8];
                for (int x = 0; x < 8; x++)
                {
                    node.Children[x] = new IntermediateOCTreeNode();

                    Vector3 newExtents = extents * 0.5f;
                    Vector3 newCenter = center + nodeOffsets[x] * newExtents;

                    node.Children[x].Bounds = new BoundingBox(
                        newCenter - newExtents,
                        newCenter + newExtents);

                    RecursiveBuild(
                        node.Children[x], 
                        PolyList, 
                        newCenter, 
                        newExtents, 
                        depth + 1);
                }
            }
        }

        
        void Split(OCTPoly poly, 
            Plane p, 
            ref OCTPoly[] newPolys, 
            out int numPolys)
        {
            numPolys = 0;

            float dA = p.DotCoordinate(poly.Verts[0]);
            float dB = p.DotCoordinate(poly.Verts[1]);
            float dC = p.DotCoordinate(poly.Verts[2]);

            Vector3 n = MathExtensions.NormalFromPoints(
                poly.Verts[0], 
                poly.Verts[1], 
                poly.Verts[2]);

            if (dA > 0 && dB > 0 && dC > 0)
            {
                newPolys[0] = new OCTPoly(
                    poly.Verts[0], 
                    poly.Verts[1], 
                    poly.Verts[2]);

                numPolys = 1;
            }
            else if (dA < 0 && dB < 0 && dC < 0)
            {
                numPolys = 0;
            }
            else if (dA == 0 && dB == 0 && dC == 0)
            {
                if (p.DotNormal(n) >= 0)
                {
                    newPolys[0] = new OCTPoly(
                        poly.Verts[0], 
                        poly.Verts[1], 
                        poly.Verts[2]);

                    numPolys = 1;
                }
                else
                {
                    numPolys = 0;
                }
            }
            else
            {

                Vector3[] NewVerts = new Vector3[4];
                int numNewVerts = 0;

                for (int x = 0; x < 3; x++)
                {
                    Vector3 v1 = poly.Verts[x + 0];
                    Vector3 v2 = poly.Verts[(x + 1) > 2 ? 0 : (x + 1)];

                    float d1 = p.DotCoordinate(v1);
                    float d2 = p.DotCoordinate(v2);

                    //point is on or in front of the splitting plane
                    if (d1 >= 0)
                    {
                        NewVerts[numNewVerts++] = v1;
                    }

                    //if point 1 is in front of the plane and 
                    //      point 2 is on or behind the plane
                    //or if point 1 is behind the plane and 
                    //      point 2 is on or in front of the plane
                    if (d2 <= 0 && d1 > 0 ||
                        d2 >= 0 && d1 < 0)
                    {
                        Vector3 newV = Vector3.Zero;
                        if (LineSegmentIntersectsPlane(v1, v2, p, ref newV))
                        {
                            NewVerts[numNewVerts++] = newV;
                        }
                    }
                }

                if (numNewVerts == 3 ||
                    numNewVerts == 4)
                {
                    newPolys[0] = new OCTPoly(
                        NewVerts[0], 
                        NewVerts[1], 
                        NewVerts[2]);

                    numPolys = 1;
                
                    if (numNewVerts == 4)
                    {
                        newPolys[1] = new OCTPoly(
                            NewVerts[0], 
                            NewVerts[2], 
                            NewVerts[3]);

                        numPolys = 2;
                    }
                }
            }
        }

        bool LineSegmentIntersectsPlane(
            Vector3 v1,
            Vector3 v2,
            Plane plane,
            ref Vector3 pos)
        {
            Vector3 vAB = v2 - v1;
            float lAB = vAB.Length();
            Vector3 nAB = Vector3.Normalize(vAB);
            Ray rAB = new Ray(v1, nAB);

            float? f = rAB.Intersects(plane);
            if (f.HasValue && f.Value < lAB)
            {
                pos = rAB.Position + rAB.Direction * f.Value;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
