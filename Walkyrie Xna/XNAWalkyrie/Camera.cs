//Using Statements
using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;


namespace XNAWalkyrie
{
    public partial class Utility
    {
        private static CameraComponent camera = null;

        public static CameraComponent Camera
        {
            get
            {
                if (camera == null)
                {
                    camera = new CameraComponent(Game);
                    Game.Components.Add(camera);
                }
                return camera;
            }
            set
            {
                if (camera != null)
                {
                    RemoveService<CameraService>();
                }
                camera = value;
                AddService<CameraService>(new CameraService(camera));
            }
        }
    }

    public interface ICameraService
    {
        CameraComponent Camera { get; }
    }

    public class CameraService : ICameraService
    {

        public CameraService(CameraComponent camera)
        {
            this.camera = camera;
        }

        private CameraComponent camera;

        public CameraComponent Camera
        {
            get
            {
                return camera;
            }
        }

        public static implicit operator CameraComponent(CameraService cs)
        {
            return cs.Camera;
        }
    }
    public interface ICamera
    {
        #region Public Methods

        /// <summary>
        /// Builds a look at style viewing matrix using the camera's current
        /// world position, and its current local y axis.
        /// </summary>
        /// <param name="target">The target position to look at.</param>
        void LookAt(Vector3 target);

        /// <summary>
        /// Builds a look at style viewing matrix.
        /// </summary>
        /// <param name="eye">The camera position.</param>
        /// <param name="target">The target position to look at.</param>
        /// <param name="up">The up direction.</param>
        void LookAt(Vector3 eye, Vector3 target, Vector3 up);

        /// <summary>
        /// Moves the camera by dx world units to the left or right; dy
        /// world units upwards or downwards; and dz world units forwards
        /// or backwards.
        /// </summary>
        /// <param name="dx">Distance to move left or right.</param>
        /// <param name="dy">Distance to move up or down.</param>
        /// <param name="dz">Distance to move forwards or backwards.</param>
        void Move(float dx, float dy, float dz);

        /// <summary>
        /// Moves the camera the specified distance in the specified direction.
        /// </summary>
        /// <param name="direction">Direction to move.</param>
        /// <param name="distance">How far to move.</param>
        void Move(Vector3 direction, Vector3 distance);

        /// <summary>
        /// Builds a perspective projection matrix based on a horizontal field
        /// of view.
        /// </summary>
        /// <param name="fovx">Horizontal field of view in degrees.</param>
        /// <param name="aspect">The viewport's aspect ratio.</param>
        /// <param name="znear">The distance to the near clip plane.</param>
        /// <param name="zfar">The distance to the far clip plane.</param>
        void Perspective(float fovx, float aspect, float znear, float zfar);

        /// <summary>
        /// Rotates the camera. Positive angles specify counter clockwise
        /// rotations when looking down the axis of rotation towards the
        /// origin.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation in degrees.</param>
        /// <param name="pitchDegrees">X axis rotation in degrees.</param>
        /// <param name="rollDegrees">Z axis rotation in degrees.</param>
        void Rotate(float headingDegrees, float pitchDegrees, float rollDegrees);

        /// <summary>
        /// Zooms the camera. This method functions differently depending on
        /// the camera's current behavior. When the camera is orbiting this
        /// method will move the camera closer to or further away from the
        /// orbit target. For the other camera behaviors this method will
        /// change the camera's horizontal field of view.
        /// </summary>
        ///
        /// <param name="zoom">
        /// When orbiting this parameter is how far to move the camera.
        /// For the other behaviors this parameter is the new horizontal
        /// field of view.
        /// </param>
        /// 
        /// <param name="minZoom">
        /// When orbiting this parameter is the min allowed zoom distance to
        /// the orbit target. For the other behaviors this parameter is the
        /// min allowed horizontal field of view.
        /// </param>
        /// 
        /// <param name="maxZoom">
        /// When orbiting this parameter is the max allowed zoom distance to
        /// the orbit target. For the other behaviors this parameter is the max
        /// allowed horizontal field of view.
        /// </param>
        void Zoom(float zoom, float minZoom, float maxZoom);

        #endregion

        #region Properties

        /// <summary>
        /// Property to get and set the camera's orientation.
        /// </summary>
        Quaternion Orientation
        {
            get;
            set;
        }

        /// <summary>
        /// Property to get and set the camera's position.
        /// </summary>
        Vector3 Position
        {
            get;
            set;
        }

        /// <summary>
        /// Property to get the camera's perspective projection matrix.
        /// </summary>
        Matrix ProjectionMatrix
        {
            get;
        }

        /// <summary>
        /// Property to get the camera's viewing direction.
        /// </summary>
        Vector3 ViewDirection
        {
            get;
        }

        /// <summary>
        /// Property to get the camera's view matrix.
        /// </summary>
        Matrix ViewMatrix
        {
            get;
        }

        /// <summary>
        /// Property to get the camera's concatenated view-projection matrix.
        /// </summary>
        Matrix ViewProjectionMatrix
        {
            get;
        }

        /// <summary>
        /// Property to get the camera's local x axis.
        /// </summary>
        Vector3 XAxis
        {
            get;
        }

        /// <summary>
        /// Property to get the camera's local y axis.
        /// </summary>
        Vector3 YAxis
        {
            get;
        }

        /// <summary>
        /// Property to get the camera's local z axis.
        /// </summary>
        Vector3 ZAxis
        {
            get;
        }

        #endregion
    }

    public class Camera : ICamera
    {
        public enum Behavior
        {
            FirstPerson,
            Spectator,
            Flight,
            FreeOrbit,
            ThirdPerson,
        };

        public const float DEFAULT_FOVX = 90.0f;
        public const float DEFAULT_ZNEAR = 0.1f;
        public const float DEFAULT_ZFAR = 100.0f;

        private const float DEFAULT_SPRING_CONSTANT = 2.0f;//16.0f;
        private const float DEFAULT_DAMPING_CONSTANT = 8.0f;

        public const float DEFAULT_ORBIT_MIN_ZOOM = DEFAULT_ZNEAR + 1.0f;
        public const float DEFAULT_ORBIT_MAX_ZOOM = DEFAULT_ZFAR * 0.5f;

        public const float DEFAULT_ORBIT_OFFSET_LENGTH = DEFAULT_ORBIT_MIN_ZOOM +
            (DEFAULT_ORBIT_MAX_ZOOM - DEFAULT_ORBIT_MIN_ZOOM) * 0.25f;

        private static Vector3 WORLD_X_AXIS = new Vector3(1.0f, 0.0f, 0.0f);
        private static Vector3 WORLD_Y_AXIS = new Vector3(0.0f, 1.0f, 0.0f);
        private static Vector3 WORLD_Z_AXIS = new Vector3(0.0f, 0.0f, 1.0f);

        private Behavior behavior;
        private bool preferTargetYAxisOrbiting;

        private float fovx;
        private float aspectRatio;
        private float znear;
        private float zfar;
        private float accumPitchDegrees;
        private float orbitMinZoom;
        private float orbitMaxZoom;
        private float orbitOffsetLength;
        private float firstPersonYOffset;

        private Vector3 eye;
        private Vector3 target;
        private Vector3 targetYAxis;
        private Vector3 xAxis;
        private Vector3 yAxis;
        private Vector3 zAxis;
        private Vector3 viewDir;

        private float elapsedTime;

        private Quaternion orientation;
        private Matrix viewMatrix;
        private Matrix projMatrix;

        private Quaternion savedOrientation;
        private Vector3 savedEye;
        //private float savedAccumPitchDegrees;

        public delegate void PerformsCollision(Vector3 OldPosition, Vector3 Velocity,out Vector3 NewPosition);
        public PerformsCollision fonctionCollision;


        private bool enableSpringSystem;
        private float springConstant;
        private float dampingConstant;

        #region Public Methods

        /// <summary>
        /// Constructs a new instance of the camera class. The camera will
        /// have a flight behavior, and will be initially positioned at the
        /// world origin looking down the world negative z axis.
        /// </summary>
        public Camera()
        {
            behavior = Behavior.Flight;
            preferTargetYAxisOrbiting = true;

            enableSpringSystem = true;
            springConstant = DEFAULT_SPRING_CONSTANT;
            dampingConstant = DEFAULT_DAMPING_CONSTANT;

            fovx = DEFAULT_FOVX;
            znear = DEFAULT_ZNEAR;
            zfar = DEFAULT_ZFAR;

            accumPitchDegrees = 0.0f;
            orbitMinZoom = DEFAULT_ORBIT_MIN_ZOOM;
            orbitMaxZoom = DEFAULT_ORBIT_MAX_ZOOM;
            orbitOffsetLength = DEFAULT_ORBIT_OFFSET_LENGTH;
            firstPersonYOffset = 0.0f;

            eye = Vector3.Zero;
            target = Vector3.Zero;
            targetYAxis = Vector3.UnitY;
            xAxis = Vector3.UnitX;
            yAxis = Vector3.UnitY;
            zAxis = Vector3.UnitZ;

            orientation = Quaternion.Identity;
            viewMatrix = Matrix.Identity;

            savedEye = eye;
            savedOrientation = orientation;
            //savedAccumPitchDegrees = 0.0f;
        }

        /// <summary>
        /// Builds a look at style viewing matrix.
        /// </summary>
        /// <param name="target">The target position to look at.</param>
        public void LookAt(Vector3 target)
        {
            LookAt(eye, target, yAxis);
        }

        /// <summary>
        /// Builds a look at style viewing matrix.
        /// </summary>
        /// <param name="eye">The camera position.</param>
        /// <param name="target">The target position to look at.</param>
        /// <param name="up">The up direction.</param>
        public void LookAt(Vector3 eye, Vector3 target, Vector3 up)
        {
            this.eye = eye;
            this.target = target;

            zAxis = eye - target;
            zAxis.Normalize();

            viewDir.X = -zAxis.X;
            viewDir.Y = -zAxis.Y;
            viewDir.Z = -zAxis.Z;

            Vector3.Cross(ref up, ref zAxis, out xAxis);
            xAxis.Normalize();

            Vector3.Cross(ref zAxis, ref xAxis, out yAxis);
            yAxis.Normalize();
            xAxis.Normalize();

            viewMatrix.M11 = xAxis.X;
            viewMatrix.M21 = xAxis.Y;
            viewMatrix.M31 = xAxis.Z;
            Vector3.Dot(ref xAxis, ref eye, out viewMatrix.M41);
            viewMatrix.M41 = -viewMatrix.M41;

            viewMatrix.M12 = yAxis.X;
            viewMatrix.M22 = yAxis.Y;
            viewMatrix.M32 = yAxis.Z;
            Vector3.Dot(ref yAxis, ref eye, out viewMatrix.M42);
            viewMatrix.M42 = -viewMatrix.M42;

            viewMatrix.M13 = zAxis.X;
            viewMatrix.M23 = zAxis.Y;
            viewMatrix.M33 = zAxis.Z;
            Vector3.Dot(ref zAxis, ref eye, out viewMatrix.M43);
            viewMatrix.M43 = -viewMatrix.M43;

            viewMatrix.M14 = 0.0f;
            viewMatrix.M24 = 0.0f;
            viewMatrix.M34 = 0.0f;
            viewMatrix.M44 = 1.0f;

            accumPitchDegrees = MathHelper.ToDegrees((float)Math.Asin(viewMatrix.M23));
            Quaternion.CreateFromRotationMatrix(ref viewMatrix, out orientation);
        }

        /// <summary>
        /// Moves the camera by dx world units to the left or right; dy
        /// world units upwards or downwards; and dz world units forwards
        /// or backwards.
        /// </summary>
        /// <param name="dx">Distance to move left or right.</param>
        /// <param name="dy">Distance to move up or down.</param>
        /// <param name="dz">Distance to move forwards or backwards.</param>
        public void Move(float dx, float dy, float dz)
        {

            Vector3 forwards;
            Vector3 OldEye;
            Vector3 Velocity;
            Vector3 NewEye;

            switch (behavior)
            {
                //#################################################################

                case Behavior.FirstPerson:

                        forwards = Vector3.Normalize(Vector3.Cross(WORLD_Y_AXIS, xAxis));

                        OldEye = eye;

                        Velocity = Vector3.Zero;
                        Velocity += xAxis * dx;
                        Velocity += WORLD_Y_AXIS * dy;
                        Velocity += forwards * dz;

                        if (fonctionCollision != null)
                        {
                            fonctionCollision(OldEye, Velocity, out NewEye);
                            eye = NewEye;
                        }
                        else
                            eye = OldEye + Velocity;

                        Position = eye;

                    break;

                //#################################################################

                case Behavior.FreeOrbit:
                case Behavior.ThirdPerson:
                    forwards = Vector3.Normalize(Vector3.Cross(WORLD_Y_AXIS, xAxis));

                    OldEye = OrbitTarget;

                    Velocity = Vector3.Zero;
                    Velocity -= xAxis * dx;
                    Velocity += WORLD_Y_AXIS * dy;
                    Velocity += forwards * dz;


                    if (fonctionCollision != null)
                    {
                        fonctionCollision(OldEye, Velocity, out NewEye);
                        OrbitTarget = NewEye;
                    }
                    else
                        OrbitTarget = OldEye + Velocity;

                    UpdateViewMatrix();
                    break;


                //#################################################################

                case Behavior.Spectator:

                    forwards = viewDir;

                    OldEye = eye;

                    Velocity = Vector3.Zero;
                    Velocity += xAxis * dx;
                    Velocity += WORLD_Y_AXIS * dy;
                    Velocity += forwards * dz;


                    if (fonctionCollision != null)
                    {
                        fonctionCollision(OldEye, Velocity, out NewEye);
                        eye = NewEye;
                    }
                    else
                        eye = OldEye + Velocity;

                    Position = eye;
                    break;

                //#################################################################

                case Behavior.Flight:
                    forwards = viewDir;

                    OldEye = eye;

                    Velocity = Vector3.Zero;
                    Velocity += xAxis * dx;
                    Velocity += WORLD_Y_AXIS * dy;
                    Velocity += forwards * dz;

                    

                    if (fonctionCollision != null)
                    {
                        fonctionCollision(OldEye, Velocity, out NewEye);
                        eye = NewEye;
                    }
                    else
                        eye = OldEye + Velocity;

                    Position = eye;

                    break;

                /*
                case Behavior.ThirdPerson:

                    forwards = Vector3.Normalize(Vector3.Cross(WORLD_Y_AXIS, xAxis));

                    OldEye = OrbitTarget;

                    Velocity = Vector3.Zero;

                    Velocity -= xAxis * dx;
                    Velocity += WORLD_Y_AXIS * dy;
                    Velocity += forwards * dz;


                    if (fonctionCollision != null)
                    {
                        fonctionCollision(OldEye, Velocity, out NewEye);
                        OrbitTarget = NewEye;
                    }
                    else
                        OrbitTarget = OldEye + Velocity;

                    UpdateViewMatrix();

                    break;
                 */
            }

 


        }

        /// <summary>
        /// Moves the camera the specified distance in the specified direction.
        /// </summary>
        /// <param name="direction">Direction to move.</param>
        /// <param name="distance">How far to move.</param>
        public void Move(Vector3 direction, Vector3 distance)
        {
            switch (behavior)
            {
                case Behavior.FirstPerson:
                case Behavior.Flight:
                case Behavior.Spectator:
                            eye.X += direction.X * distance.X;
                            eye.Y += direction.Y * distance.Y;
                            eye.Z += direction.Z * distance.Z;
                            UpdateViewMatrix();
                break;

                case Behavior.FreeOrbit:
                case Behavior.ThirdPerson:
                {
                    Vector3 NewOrbitTarget = new Vector3();
                    NewOrbitTarget = OrbitTarget;
                    NewOrbitTarget.X += direction.X * distance.X;
                    NewOrbitTarget.Y += direction.Y * distance.Y;
                    NewOrbitTarget.Z += direction.Z * distance.Z;
                    OrbitTarget = NewOrbitTarget;
                    UpdateViewMatrix();

                }
                break;
                
            }
        }

        /// <summary>
        /// Builds a perspective projection matrix based on a horizontal field
        /// of view.
        /// </summary>
        /// <param name="fovx">Horizontal field of view in degrees.</param>
        /// <param name="aspect">The viewport's aspect ratio.</param>
        /// <param name="znear">The distance to the near clip plane.</param>
        /// <param name="zfar">The distance to the far clip plane.</param>
        public void Perspective(float fovx, float aspect, float znear, float zfar)
        {
            this.fovx = fovx;
            this.aspectRatio = aspect;
            this.znear = znear;
            this.zfar = zfar;

            float aspectInv = 1.0f / aspect;
            float e = 1.0f / (float)Math.Tan(MathHelper.ToRadians(fovx) / 2.0f);
            float fovy = 2.0f * (float)Math.Atan(aspectInv / e);
            float xScale = 1.0f / (float)Math.Tan(0.5f * fovy);
            float yScale = xScale / aspectInv;

            projMatrix.M11 = xScale;
            projMatrix.M12 = 0.0f;
            projMatrix.M13 = 0.0f;
            projMatrix.M14 = 0.0f;

            projMatrix.M21 = 0.0f;
            projMatrix.M22 = yScale;
            projMatrix.M23 = 0.0f;
            projMatrix.M24 = 0.0f;

            projMatrix.M31 = 0.0f;
            projMatrix.M32 = 0.0f;
            projMatrix.M33 = (zfar + znear) / (znear - zfar);
            projMatrix.M34 = -1.0f;

            projMatrix.M41 = 0.0f;
            projMatrix.M42 = 0.0f;
            projMatrix.M43 = (2.0f * zfar * znear) / (znear - zfar);
            projMatrix.M44 = 0.0f;
        }

        /// <summary>
        /// Rotates the camera. Positive angles specify counter clockwise
        /// rotations when looking down the axis of rotation towards the
        /// origin.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation in degrees.</param>
        /// <param name="pitchDegrees">X axis rotation in degrees.</param>
        /// <param name="rollDegrees">Z axis rotation in degrees.</param>
        public void Rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
        {
            headingDegrees = -headingDegrees;
            pitchDegrees = -pitchDegrees;
            rollDegrees = -rollDegrees;

            switch (behavior)
            {
                case Behavior.FirstPerson:
                case Behavior.Spectator:
                    RotateFirstPerson(headingDegrees, pitchDegrees);
                    UpdateViewMatrix();
                    break;

                case Behavior.Flight:
                    RotateFlight(headingDegrees, pitchDegrees, rollDegrees);
                    UpdateViewMatrix();
                    break;

                case Behavior.FreeOrbit:
                    RotateOrbit(headingDegrees, pitchDegrees, rollDegrees);
                    UpdateViewMatrix();
                    break;

                case Behavior.ThirdPerson:
                    RotateOrbit(headingDegrees, 0.0f, 0.0f);
                    UpdateViewMatrix();
                    break;

                default:
                    break;
            }

            
        }

        /// <summary>
        /// Undo any camera rolling by leveling the camera. When the camera is
        /// orbiting this method will cause the camera to become level with the
        /// orbit target.
        /// </summary>
        public void UndoRoll()
        {

            switch (behavior)
            {
                case Behavior.FirstPerson:
                case Behavior.Spectator:
                case Behavior.Flight:
                    LookAt(eye, eye + ViewDirection, WORLD_Y_AXIS);
                    break;

                case Behavior.FreeOrbit:
                case Behavior.ThirdPerson:
                    LookAt(eye, eye + ViewDirection, WORLD_Y_AXIS);
                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Zooms the camera. This method functions differently depending on
        /// the camera's current behavior. When the camera is orbiting this
        /// method will move the camera closer to or further away from the
        /// orbit target. For the other camera behaviors this method will
        /// change the camera's horizontal field of view.
        /// </summary>
        ///
        /// <param name="zoom">
        /// When orbiting this parameter is how far to move the camera.
        /// For the other behaviors this parameter is the new horizontal
        /// field of view.
        /// </param>
        /// 
        /// <param name="minZoom">
        /// When orbiting this parameter is the min allowed zoom distance to
        /// the orbit target. For the other behaviors this parameter is the
        /// min allowed horizontal field of view.
        /// </param>
        /// 
        /// <param name="maxZoom">
        /// When orbiting this parameter is the max allowed zoom distance to
        /// the orbit target. For the other behaviors this parameter is the max
        /// allowed horizontal field of view.
        /// </param>
        public void Zoom(float zoom, float minZoom, float maxZoom)
        {


            switch (behavior)
            {
                case Behavior.FirstPerson:
                case Behavior.Spectator:
                case Behavior.Flight:

                    zoom = Math.Min(Math.Max(zoom, minZoom), maxZoom);
                    Perspective(zoom, aspectRatio, znear, zfar);

                    break;

                case Behavior.FreeOrbit:
                case Behavior.ThirdPerson:
                    {
                        Vector3 offset = eye - target;
                        orbitOffsetLength = offset.Length();
                        offset.Normalize();
                        orbitOffsetLength += zoom;
                        orbitOffsetLength = Math.Min(Math.Max(orbitOffsetLength, minZoom), maxZoom);
                        offset *= orbitOffsetLength;
                        eye = offset + target;
                        UpdateViewMatrix();
                    }
                    break;
                default:
                    break;
            }

        }
        #endregion

        #region Private Methods

        /// <summary>
        /// Change to a new camera behavior.
        /// </summary>
        /// <param name="newBehavior">The new camera behavior.</param>
        private void ChangeBehavior(Behavior newBehavior)
        {
            Behavior prevBehavior = behavior;

            if (prevBehavior == newBehavior)
                return;

            behavior = newBehavior;

            switch (newBehavior)
            {
                case Behavior.FirstPerson:
                    switch (prevBehavior)
                    {
                        case Behavior.Flight:
                        case Behavior.Spectator:
                            eye.Y = firstPersonYOffset;
                            UpdateViewMatrix();
                            break;

                        case Behavior.FreeOrbit:
                        case Behavior.ThirdPerson:
                            eye = OrbitTarget;
                            //yAxis = WORLD_Y_AXIS;
                            //target.Y = eye.Y;

                            //orientation = savedOrientation;
                            // accumPitchDegrees = savedAccumPitchDegrees;
                            UpdateViewMatrix();
                            break;

                        default:
                            break;
                    }

                    UndoRoll();
                    break;

                case Behavior.Spectator:
                    switch (prevBehavior)
                    {
                        case Behavior.Flight:
                            UpdateViewMatrix();
                            break;

                        case Behavior.FreeOrbit:
                        case Behavior.ThirdPerson:
                            //eye = savedEye;
                            //orientation = savedOrientation;
                            //accumPitchDegrees = savedAccumPitchDegrees;
                            UpdateViewMatrix();
                            break;

                        default:
                            break;
                    }

                    UndoRoll();
                    break;

                case Behavior.Flight:
                    switch (prevBehavior)
                    {
                        case Behavior.FirstPerson:
                        case Behavior.Spectator:
                        case Behavior.FreeOrbit:
                        case Behavior.ThirdPerson:
                            UpdateViewMatrix();
                            break;

                        default:
                            break;
                    }
                    break;

                case Behavior.FreeOrbit:
                    {
                        switch (prevBehavior)
                        {
                            case Behavior.FirstPerson:
                                {
                                    firstPersonYOffset = eye.Y;
                                    targetYAxis = WORLD_Y_AXIS;
                                    Vector3 diagonal = new Vector3();
                                    diagonal = WORLD_Z_AXIS + WORLD_Y_AXIS * 0.50f;
                                    diagonal.Normalize();
                                    //Vector3 newEye = eye + zAxis * orbitOffsetLength;
                                    Vector3 newEye = eye + diagonal * orbitOffsetLength;
                                    LookAt(newEye, eye, targetYAxis);
                                }
                                break;

                            case Behavior.Spectator:
                            case Behavior.Flight:
                                {
                                    targetYAxis = WORLD_Y_AXIS;
                                    Vector3 newEye = eye + zAxis * orbitOffsetLength;
                                    LookAt(newEye, eye, targetYAxis);
                                }
                                break;

                            case Behavior.ThirdPerson:
                                    return;
                                break;

                            default:
                                break;
                        }



                    }
                    break;


                case Behavior.ThirdPerson:
                    {

                        switch (prevBehavior)
                        {
                            case Behavior.FirstPerson:
                                {
                                    firstPersonYOffset = eye.Y;
                                    targetYAxis = WORLD_Y_AXIS;
                                    //Vector3 newEye = eye + zAxis * orbitOffsetLength;
                                    Vector3 diagonal = new Vector3();
                                    diagonal = WORLD_Z_AXIS + WORLD_Y_AXIS * 0.50f;
                                    diagonal.Normalize();
                                    Vector3 newEye = eye + diagonal * orbitOffsetLength;
                                    LookAt(newEye, eye, targetYAxis);
                                }
                                break;

                            case Behavior.Spectator:
                            case Behavior.Flight:
                                {
                                    targetYAxis = WORLD_Y_AXIS; 
                                    Vector3 newEye = eye + zAxis * orbitOffsetLength;
                                    LookAt(newEye, eye, targetYAxis);
                                }
                                break;

                            case Behavior.FreeOrbit:
                                {
                                    return;
                                }
                                break;

                            default:
                                break;
                        }


                    }
                    break;

                default:
                    break;
            }
        }

        /// <summary>
        /// Sets a new camera orientation.
        /// </summary>
        /// <param name="newOrientation">The new orientation.</param>
        private void ChangeOrientation(Quaternion newOrientation)
        {
            Matrix m = Matrix.CreateFromQuaternion(newOrientation);

            // Store the pitch for this new orientation.
            // First person and spectator behaviors limit pitching to
            // 90 degrees straight up and down.

            float pitch = (float)Math.Asin(m.M23);

            accumPitchDegrees = MathHelper.ToDegrees(pitch);

            // First person and spectator behaviors don't allow rolling.
            // Negate any rolling that might be encoded in the new orientation.

            orientation = newOrientation;

            if (behavior == Behavior.FirstPerson || behavior == Behavior.Spectator)
                LookAt(eye, eye + Vector3.Negate(zAxis), WORLD_Y_AXIS);

            UpdateViewMatrix();
        }

        /// <summary>
        /// Rotates the camera for first person and spectator behaviors.
        /// Pitching is limited to 90 degrees straight up and down.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation angle.</param>
        /// <param name="pitchDegrees">X axis rotation angle.</param>
        private void RotateFirstPerson(float headingDegrees, float pitchDegrees)
        {
            accumPitchDegrees += pitchDegrees;

            if (accumPitchDegrees > 90.0f)
            {
                pitchDegrees = 90.0f - (accumPitchDegrees - pitchDegrees);
                accumPitchDegrees = 90.0f;
            }

            if (accumPitchDegrees < -90.0f)
            {
                pitchDegrees = -90.0f - (accumPitchDegrees - pitchDegrees);
                accumPitchDegrees = -90.0f;
            }

            float heading = MathHelper.ToRadians(headingDegrees);
            float pitch = MathHelper.ToRadians(pitchDegrees);
            Quaternion rotation = Quaternion.Identity;

            // Rotate the camera about the world Y axis.
            if (heading != 0.0f)
            {
                Quaternion.CreateFromAxisAngle(ref WORLD_Y_AXIS, heading, out rotation);
                Quaternion.Concatenate(ref rotation, ref orientation, out orientation);
            }

            // Rotate the camera about its local X axis.
            if (pitch != 0.0f)
            {
                Quaternion.CreateFromAxisAngle(ref WORLD_X_AXIS, pitch, out rotation);
                Quaternion.Concatenate(ref orientation, ref rotation, out orientation);
            }
        }

        /// <summary>
        /// Rotates the camera for flight behavior.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation angle.</param>
        /// <param name="pitchDegrees">X axis rotation angle.</param>
        /// <param name="rollDegrees">Z axis rotation angle.</param>
        private void RotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees)
        {
            accumPitchDegrees += pitchDegrees;

            if (accumPitchDegrees > 360.0f)
                accumPitchDegrees -= 360.0f;

            if (accumPitchDegrees < -360.0f)
                accumPitchDegrees += 360.0f;

            float heading = MathHelper.ToRadians(headingDegrees);
            float pitch = MathHelper.ToRadians(pitchDegrees);
            float roll = MathHelper.ToRadians(rollDegrees);

            Quaternion rotation = Quaternion.CreateFromYawPitchRoll(heading, pitch, roll);
            Quaternion.Concatenate(ref orientation, ref rotation, out orientation);
        }

        /// <summary>
        /// Rotates the camera for orbit behavior. Rotations are either about
        /// the camera's local y axis or the orbit target's y axis. The property
        /// PreferTargetYAxisOrbiting controls which rotation method to use.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation angle.</param>
        /// <param name="pitchDegrees">X axis rotation angle.</param>
        /// <param name="rollDegrees">Z axis rotation angle.</param>
        private void RotateOrbit(float headingDegrees, float pitchDegrees, float rollDegrees)
        {
            float heading = MathHelper.ToRadians(headingDegrees);
            float pitch = MathHelper.ToRadians(pitchDegrees);

            if (preferTargetYAxisOrbiting)
            {
                Quaternion rotation = Quaternion.Identity;

                if (heading != 0.0f)
                {
                    Quaternion.CreateFromAxisAngle(ref targetYAxis, heading, out rotation);
                    Quaternion.Concatenate(ref rotation, ref orientation, out orientation);
                }

                if (pitch != 0.0f)
                {
                    Quaternion.CreateFromAxisAngle(ref WORLD_X_AXIS, pitch, out rotation);
                    Quaternion.Concatenate(ref orientation, ref rotation, out orientation);
                }
            }
            else
            {
                float roll = MathHelper.ToRadians(rollDegrees);
                Quaternion rotation = Quaternion.CreateFromYawPitchRoll(heading, pitch, roll);
                Quaternion.Concatenate(ref orientation, ref rotation, out orientation);
            }
        }

        /// <summary>
        /// Rebuild the view matrix.
        /// </summary>
        private void UpdateViewMatrix()
        {
            Matrix.CreateFromQuaternion(ref orientation, out viewMatrix);

            xAxis.X = viewMatrix.M11;
            xAxis.Y = viewMatrix.M21;
            xAxis.Z = viewMatrix.M31;

            yAxis.X = viewMatrix.M12;
            yAxis.Y = viewMatrix.M22;
            yAxis.Z = viewMatrix.M32;

            zAxis.X = viewMatrix.M13;
            zAxis.Y = viewMatrix.M23;
            zAxis.Z = viewMatrix.M33;

            if (behavior == Behavior.FreeOrbit)
            {
                // Calculate the new camera position based on the current
                // orientation. The camera must always maintain the same
                // distance from the target. Use the current offset vector
                // to determine the correct distance from the target.

                eye = target + zAxis * orbitOffsetLength;


            }

            if (behavior == Behavior.ThirdPerson && enableSpringSystem)
            {
                Vector3 idealPosition = target + zAxis * orbitOffsetLength;
                Vector3 displacement = eye - idealPosition;
                Vector3 springAcceleration = (-springConstant * displacement);

                eye.X += springAcceleration.X * ElapsedTime;
                eye.Y = idealPosition.Y;
                eye.Z += springAcceleration.Z * ElapsedTime;

                zAxis = eye - target;
                zAxis.Normalize();

                Vector3.Negate(ref zAxis, out viewDir);

                Vector3.Cross(ref targetYAxis, ref zAxis, out xAxis);
                xAxis.Normalize();

                Vector3.Cross(ref zAxis, ref xAxis, out yAxis);
                yAxis.Normalize();

                viewMatrix.M11 = xAxis.X;
                viewMatrix.M21 = xAxis.Y;
                viewMatrix.M31 = xAxis.Z;
                Vector3.Dot(ref xAxis, ref eye, out viewMatrix.M41);
                viewMatrix.M41 = -viewMatrix.M41;

                viewMatrix.M12 = yAxis.X;
                viewMatrix.M22 = yAxis.Y;
                viewMatrix.M32 = yAxis.Z;
                Vector3.Dot(ref yAxis, ref eye, out viewMatrix.M42);
                viewMatrix.M42 = -viewMatrix.M42;

                viewMatrix.M13 = zAxis.X;
                viewMatrix.M23 = zAxis.Y;
                viewMatrix.M33 = zAxis.Z;
                Vector3.Dot(ref zAxis, ref eye, out viewMatrix.M43);
                viewMatrix.M43 = -viewMatrix.M43;

                viewMatrix.M14 = 0.0f;
                viewMatrix.M24 = 0.0f;
                viewMatrix.M34 = 0.0f;
                viewMatrix.M44 = 1.0f;
            }
            else
            {
                if (behavior == Behavior.ThirdPerson)
                {
                    eye = target + zAxis * orbitOffsetLength;
                }
                viewMatrix.M41 = -Vector3.Dot(xAxis, eye);
                viewMatrix.M42 = -Vector3.Dot(yAxis, eye);
                viewMatrix.M43 = -Vector3.Dot(zAxis, eye);

                viewDir.X = -zAxis.X;
                viewDir.Y = -zAxis.Y;
                viewDir.Z = -zAxis.Z;

            }
        }


        #endregion


        #region Properties

        /// <summary>
        /// Property to get and set the camera's behavior.
        /// </summary>
        public Behavior CurrentBehavior
        {
            get { return behavior; }
            set { ChangeBehavior(value); }
        }

        /// <summary>
        /// Property to get and set the max orbit zoom distance.
        /// </summary>
        public float OrbitMaxZoom
        {
            get { return orbitMaxZoom; }
            set { orbitMaxZoom = value; }
        }

        public float ElapsedTime
        {
            get { return elapsedTime; }
            set { elapsedTime = value; }
        }

        /// <summary>
        /// Property to get and set the min orbit zoom distance.
        /// </summary>
        public float OrbitMinZoom
        {
            get { return orbitMinZoom; }
            set { orbitMinZoom = value; }
        }

        /// <summary>
        /// Property to get and set the distance from the target when orbiting.
        /// </summary>
        public float OrbitOffsetDistance
        {
            get { return orbitOffsetLength; }
            set { orbitOffsetLength = value; }
        }


        /// <summary>
        /// Property to get and set the camera orbit target position.
        /// </summary>
        public Vector3 OrbitTarget
        {
            get { return target; }
            set { target = value; }
        }

        public Vector3 TargetYAxis
        {
            get { return targetYAxis; }
            set { targetYAxis = value; }
        }

        /// <summary>
        /// Property to get and set the camera orientation.
        /// </summary>
        public Quaternion Orientation
        {
            get { return orientation; }
            set { ChangeOrientation(value); }
        }

        /// <summary>
        /// Property to get and set the camera position.
        /// </summary>
        public Vector3 Position
        {
            get { return eye; }

            set
            {
                eye = value;
                UpdateViewMatrix();
            }
        }

        public Vector3 DirectEye
        {
            set
            {
                eye = value;
            }
            get
            {
                return DirectEye;
            }
        }

        /// <summary>
        /// Property to get and set the flag to force the camera
        /// to orbit around the orbit target's Y axis rather than the camera's
        /// local Y axis.
        /// </summary>
        public bool PreferTargetYAxisOrbiting
        {
            get { return preferTargetYAxisOrbiting; }

            set
            {
                preferTargetYAxisOrbiting = value;

                if (preferTargetYAxisOrbiting)
                    UndoRoll();
            }
        }

        /// <summary>
        /// Property to get the perspective projection matrix.
        /// </summary>
        public Matrix ProjectionMatrix
        {
            get { return projMatrix; }
        }

        /// <summary>
        /// Property to get the viewing direction vector.
        /// </summary>
        public Vector3 ViewDirection
        {
            set { viewDir = value; }
            get { return viewDir; }
        }

        /// <summary>
        /// Property to get the view matrix.
        /// </summary>
        public Matrix ViewMatrix
        {
            set { viewMatrix = value; }
            get { return viewMatrix; }
        }

        /// <summary>
        /// Property to get the concatenated view-projection matrix.
        /// </summary>
        public Matrix ViewProjectionMatrix
        {
            get { return viewMatrix * projMatrix; }
        }

        /// <summary>
        /// Property to get the camera's local X axis.
        /// </summary>
        public Vector3 XAxis
        {
            set { xAxis = value; }
            get { return xAxis; }
        }

        /// <summary>
        /// Property to get the camera's local Y axis.
        /// </summary>
        public Vector3 YAxis
        {
            set { yAxis = value; }
            get { return yAxis; }
        }

        /// <summary>
        /// Property to get the camera's local Z axis.
        /// </summary>
        public Vector3 ZAxis
        {
            set { zAxis = value; }
            get { return zAxis; }
        }
        #endregion
    }

    /// <summary>
    /// A general purpose quaternion based camera component for XNA. This
    /// camera component provides the necessary bindings to the XNA framework
    /// to allow the camera to be manipulated by the keyboard, mouse, and game
    /// pad. This camera component is implemented in terms of the Camera class.
    /// As a result the camera component supports all of the features of the
    /// Camera class. The camera component maps input to a series of actions.
    /// These actions are defined by the Actions enumeration. Methods are
    /// provided to remap the camera components default bindings.
    /// </summary>
    public class CameraComponent : GameComponent, ICamera
    {
        public enum Actions
        {
            FlightYawLeftPrimary,
            FlightYawLeftAlternate,
            FlightYawRightPrimary,
            FlightYawRightAlternate,

            MoveForwardsPrimary,
            MoveForwardsAlternate,
            MoveBackwardsPrimary,
            MoveBackwardsAlternate,

            MoveDownPrimary,
            MoveDownAlternate,
            MoveUpPrimary,
            MoveUpAlternate,

            OrbitRollLeftPrimary,
            OrbitRollLeftAlternate,
            OrbitRollRightPrimary,
            OrbitRollRightAlternate,

            PitchUpPrimary,
            PitchUpAlternate,
            PitchDownPrimary,
            PitchDownAlternate,

            YawLeftPrimary,
            YawLeftAlternate,
            YawRightPrimary,
            YawRightAlternate,

            RollLeftPrimary,
            RollLeftAlternate,
            RollRightPrimary,
            RollRightAlternate,

            StrafeRightPrimary,
            StrafeRightAlternate,
            StrafeLeftPrimary,
            StrafeLeftAlternate,

            Crouch,
            Jump,
            Run
        };

        public enum Posture
        {
            Standing,
            Crouching,
            Rising,
            Jumping
        };


        private const float DEFAULT_ACCELERATION_X = 8.0f;
        private const float DEFAULT_ACCELERATION_Y = 8.0f;
        private const float DEFAULT_ACCELERATION_Z = 8.0f;
        private const float DEFAULT_MOUSE_SMOOTHING_SENSITIVITY = 0.5f;
        private const float DEFAULT_SPEED_FLIGHT_YAW = 100.0f;
        private const float DEFAULT_SPEED_MOUSE_WHEEL = 1.0f;
        private const float DEFAULT_SPEED_ORBIT_ROLL = 100.0f;
        private const float DEFAULT_SPEED_ROTATION = 0.3f;
        private const float DEFAULT_VELOCITY_X = 1.0f;
        private const float DEFAULT_VELOCITY_Y = 1.0f;
        private const float DEFAULT_VELOCITY_Z = 1.0f;


        private const int MOUSE_SMOOTHING_CACHE_SIZE = 10;
        private const float HEIGHT_MULTIPLIER_CROUCHING = 0.05f;
        private const float DEFAULT_RUNNING_MULTIPLIER = 2.0f;

        private Camera camera;
        private bool movingAlongPosX;
        private bool movingAlongNegX;
        private bool movingAlongPosY;
        private bool movingAlongNegY;
        private bool movingAlongPosZ;
        private bool movingAlongNegZ;
        private int savedMousePosX;
        private int savedMousePosY;
        private int mouseIndex;
        private float rotationSpeed;
        private float orbitRollSpeed;
        private float flightYawSpeed;
        private float mouseSmoothingSensitivity;
        private float mouseWheelSpeed;
        private Vector3 acceleration;

        private Vector3 currentVelocity;
        private Vector3 currentForce;

        private Vector3 velocity;
        private Vector3 velocityWalking;
        private Vector3 velocityRunning;
        private bool bEndurance = true;
        private bool bonground = true;

        private Vector2[] mouseMovement;
        private Vector2[] mouseSmoothingCache;
        private Vector2 smoothedMouseMovement;

        private MouseState currentMouseState;
        private MouseState previousMouseState;


        private Dictionary<Actions, Keys> actionKeys;
        private BoundingFrustum frustum;

        private Posture posture;
        private float eyeHeightStanding;
        private float eyeHeightCrouching;



        #region Public Methods

        /// <summary>
        /// Constructs a new instance of the CameraComponent class. The
        /// camera will have a spectator behavior, and will be initially
        /// positioned at the world origin looking down the world negative
        /// z axis. An initial perspective projection matrix is created
        /// as well as setting up initial key bindings to the actions.
        /// </summary>
        public CameraComponent(Game game)
            : base(game)
        {



            camera = new Camera();
            camera.CurrentBehavior = Camera.Behavior.Spectator;


            movingAlongPosX = false;
            movingAlongNegX = false;
            movingAlongPosY = false;
            movingAlongNegY = false;
            movingAlongPosZ = false;
            movingAlongNegZ = false;

            savedMousePosX = -1;
            savedMousePosY = -1;

            rotationSpeed = DEFAULT_SPEED_ROTATION;
            orbitRollSpeed = DEFAULT_SPEED_ORBIT_ROLL;
            flightYawSpeed = DEFAULT_SPEED_FLIGHT_YAW;
            mouseWheelSpeed = DEFAULT_SPEED_MOUSE_WHEEL;
            mouseSmoothingSensitivity = DEFAULT_MOUSE_SMOOTHING_SENSITIVITY;
            acceleration = new Vector3(DEFAULT_ACCELERATION_X, DEFAULT_ACCELERATION_Y, DEFAULT_ACCELERATION_Z);
            velocityWalking = new Vector3(DEFAULT_VELOCITY_X, DEFAULT_VELOCITY_Y, DEFAULT_VELOCITY_Z);
            velocityRunning = velocityWalking * DEFAULT_RUNNING_MULTIPLIER;
            velocity = velocityWalking;
            currentForce = Vector3.Zero;

            mouseSmoothingCache = new Vector2[MOUSE_SMOOTHING_CACHE_SIZE];

            posture = Posture.Standing;

            mouseIndex = 0;
            mouseMovement = new Vector2[2];
            mouseMovement[0].X = 0.0f;
            mouseMovement[0].Y = 0.0f;
            mouseMovement[1].X = 0.0f;
            mouseMovement[1].Y = 0.0f;

            Rectangle clientBounds = game.Window.ClientBounds;
            float aspect = (float)clientBounds.Width / (float)clientBounds.Height;

            Perspective(Camera.DEFAULT_FOVX, aspect, Camera.DEFAULT_ZNEAR, Camera.DEFAULT_ZFAR);

            actionKeys = new Dictionary<Actions, Keys>();

            actionKeys.Add(Actions.FlightYawLeftPrimary, Keys.Left);
            actionKeys.Add(Actions.FlightYawLeftAlternate, Keys.Q);
            actionKeys.Add(Actions.FlightYawRightPrimary, Keys.Right);
            actionKeys.Add(Actions.FlightYawRightAlternate, Keys.D);
            actionKeys.Add(Actions.MoveForwardsPrimary, Keys.Up);
            actionKeys.Add(Actions.MoveForwardsAlternate, Keys.Z);
            actionKeys.Add(Actions.MoveBackwardsPrimary, Keys.Down);
            actionKeys.Add(Actions.MoveBackwardsAlternate, Keys.S);
            actionKeys.Add(Actions.MoveDownPrimary, Keys.A);
            actionKeys.Add(Actions.MoveDownAlternate, Keys.PageDown);
            actionKeys.Add(Actions.MoveUpPrimary, Keys.E);
            actionKeys.Add(Actions.MoveUpAlternate, Keys.PageUp);
            actionKeys.Add(Actions.OrbitRollLeftPrimary, Keys.Left);
            actionKeys.Add(Actions.OrbitRollLeftAlternate, Keys.Q);
            actionKeys.Add(Actions.OrbitRollRightPrimary, Keys.Right);
            actionKeys.Add(Actions.OrbitRollRightAlternate, Keys.D);
            actionKeys.Add(Actions.StrafeRightPrimary, Keys.Right);
            actionKeys.Add(Actions.StrafeRightAlternate, Keys.D);
            actionKeys.Add(Actions.StrafeLeftPrimary, Keys.Left);
            actionKeys.Add(Actions.StrafeLeftAlternate, Keys.Q);
            actionKeys.Add(Actions.Crouch, Keys.LeftShift);
            actionKeys.Add(Actions.Jump, Keys.Space);
            actionKeys.Add(Actions.Run, Keys.LeftControl);


            Game.Activated += new EventHandler(HandleGameActivatedEvent);
            Game.Deactivated += new EventHandler(HandleGameDeactivatedEvent);

            UpdateOrder = 1;

        }

        /// <summary>
        /// Initializes the CameraComponent class. This method repositions the
        /// mouse to the center of the game window.
        /// </summary>
        public override void Initialize()
        {
            base.Initialize();

            Rectangle clientBounds = Game.Window.ClientBounds;
            Mouse.SetPosition(clientBounds.Width / 2, clientBounds.Height / 2);
        }

        /// <summary>
        /// Builds a look at style viewing matrix.
        /// </summary>
        /// <param name="target">The target position to look at.</param>
        public void LookAt(Vector3 target)
        {
            camera.LookAt(target);
        }

        /// <summary>
        /// Builds a look at style viewing matrix.
        /// </summary>
        /// <param name="eye">The camera position.</param>
        /// <param name="target">The target position to look at.</param>
        /// <param name="up">The up direction.</param>
        public void LookAt(Vector3 eye, Vector3 target, Vector3 up)
        {
            camera.LookAt(eye, target, up);
        }

        /// <summary>
        /// Binds an action to a keyboard key.
        /// </summary>
        /// <param name="action">The action to bind.</param>
        /// <param name="key">The key to map the action to.</param>
        public void MapActionToKey(Actions action, Keys key)
        {
            actionKeys[action] = key;
        }

        /// <summary>
        /// Moves the camera by dx world units to the left or right; dy
        /// world units upwards or downwards; and dz world units forwards
        /// or backwards.
        /// </summary>
        /// <param name="dx">Distance to move left or right.</param>
        /// <param name="dy">Distance to move up or down.</param>
        /// <param name="dz">Distance to move forwards or backwards.</param>
        public void Move(float dx, float dy, float dz)
        {
            camera.Move(dx, dy, dz);
        }

        /// <summary>
        /// Moves the camera the specified distance in the specified direction.
        /// </summary>
        /// <param name="direction">Direction to move.</param>
        /// <param name="distance">How far to move.</param>
        public void Move(Vector3 direction, Vector3 distance)
        {
            camera.Move(direction, distance);
        }

        /// <summary>
        /// Builds a perspective projection matrix based on a horizontal field
        /// of view.
        /// </summary>
        /// <param name="fovx">Horizontal field of view in degrees.</param>
        /// <param name="aspect">The viewport's aspect ratio.</param>
        /// <param name="znear">The distance to the near clip plane.</param>
        /// <param name="zfar">The distance to the far clip plane.</param>
        public void Perspective(float fovx, float aspect, float znear, float zfar)
        {
            camera.Perspective(fovx, aspect, znear, zfar);
        }

        /// <summary>
        /// Rotates the camera. Positive angles specify counter clockwise
        /// rotations when looking down the axis of rotation towards the
        /// origin.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation in degrees.</param>
        /// <param name="pitchDegrees">X axis rotation in degrees.</param>
        /// <param name="rollDegrees">Z axis rotation in degrees.</param>
        public void Rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
        {
            camera.Rotate(headingDegrees, pitchDegrees, rollDegrees);
        }

        /// <summary>
        /// Updates the state of the CameraComponent class.
        /// </summary>
        /// <param name="gameTime">Time elapsed since the last call to Update.</param>
        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);
            UpdateInput();
            UpdateCamera(gameTime);
        }

        /// <summary>
        /// Undo any camera rolling by leveling the camera. When the camera is
        /// orbiting this method will cause the camera to become level with the
        /// orbit target.
        /// </summary>
        public void UndoRoll()
        {
            camera.UndoRoll();
        }

        /// <summary>
        /// Zooms the camera. This method functions differently depending on
        /// the camera's current behavior. When the camera is orbiting this
        /// method will move the camera closer to or further away from the
        /// orbit target. For the other camera behaviors this method will
        /// change the camera's horizontal field of view.
        /// </summary>
        ///
        /// <param name="zoom">
        /// When orbiting this parameter is how far to move the camera.
        /// For the other behaviors this parameter is the new horizontal
        /// field of view.
        /// </param>
        /// 
        /// <param name="minZoom">
        /// When orbiting this parameter is the min allowed zoom distance to
        /// the orbit target. For the other behaviors this parameter is the
        /// min allowed horizontal field of view.
        /// </param>
        /// 
        /// <param name="maxZoom">
        /// When orbiting this parameter is the max allowed zoom distance to
        /// the orbit target. For the other behaviors this parameter is the max
        /// allowed horizontal field of view.
        /// </param>
        public void Zoom(float zoom, float minZoom, float maxZoom)
        {
            camera.Zoom(zoom, minZoom, maxZoom);
        }




        /// <summary>
        /// Calculates the world transformation matrix for the weapon attached
        /// to the FirstPersonCamera. The weapon moves along with the camera.
        /// The offsets are to ensure the weapon is slightly in front of the
        /// camera and to one side.
        /// </summary>
        /// <param name="xOffset">How far to position the weapon left or right.</param>
        /// <param name="yOffset">How far to position the weapon up or down.</param>
        /// <param name="zOffset">How far to position the weapon in front or behind.</param>
        /// <returns>The weapon world transformation matrix.</returns>
        public Matrix WeaponWorldMatrix(float xOffset, float yOffset, float zOffset)
        {
            Vector3 weaponPos = camera.Position;

            weaponPos += ZAxis * zOffset;
            weaponPos += YAxis * yOffset;
            weaponPos += XAxis * xOffset;

            return Matrix.CreateFromQuaternion(Orientation)
                * Matrix.CreateTranslation(weaponPos);
        }

        /// <summary>
        /// Calculates the world transformation matrix for the weapon attached
        /// to the FirstPersonCamera. The weapon moves along with the camera.
        /// The offsets are to ensure the weapon is slightly in front of the
        /// camera and to one side.
        /// </summary>
        /// <param name="xOffset">How far to position the weapon left or right.</param>
        /// <param name="yOffset">How far to position the weapon up or down.</param>
        /// <param name="zOffset">How far to position the weapon in front or behind.</param>
        /// <param name="scale">How much to scale the weapon.</param>
        /// <returns>The weapon world transformation matrix.</returns>
        public Matrix WeaponWorldMatrix(float xOffset, float yOffset, float zOffset, float scale)
        {
            Vector3 weaponPos = camera.Position;

            weaponPos += ZAxis * zOffset;
            weaponPos += YAxis * yOffset;
            weaponPos += XAxis * xOffset;

            return Matrix.CreateScale(scale)
                 * Matrix.CreateFromQuaternion(Quaternion.Inverse(Orientation))
                * Matrix.CreateTranslation(weaponPos);
        }

        #endregion

        #region Private Methods

        /// <summary>
        /// Determines which way to move the camera based on player input.
        /// The returned values are in the range [-1,1].
        /// </summary>
        /// <param name="direction">The movement direction.</param>
        private void GetMovementDirection(out Vector3 direction)
        {
            direction.X = 0.0f;
            direction.Y = 0.0f;
            direction.Z = 0.0f;

            if (Utility.InputState.IsKeyDown(actionKeys[Actions.MoveForwardsPrimary]) ||
                Utility.InputState.IsKeyDown(actionKeys[Actions.MoveForwardsAlternate]) ||
                Utility.InputState.IsButtonPressDirectionUp()
#if !XBOX360
                || Utility.InputState.DirectionUpPadPc() != 0.0f
#endif
                )
            {
                if (!movingAlongNegZ)
                {
                    movingAlongNegZ = true;
                    currentVelocity.Z = 0.0f;
                }

                direction.Z += 1.0f;
            }
            else
            {
                movingAlongNegZ = false;
            }

            if (Utility.InputState.IsKeyDown(actionKeys[Actions.MoveBackwardsPrimary]) ||
                Utility.InputState.IsKeyDown(actionKeys[Actions.MoveBackwardsAlternate])
#if !XBOX360
                || Utility.InputState.DirectionDownPadPc() != 0.0f
#endif

                )
            {
                if (!movingAlongPosZ)
                {
                    movingAlongPosZ = true;
                    currentVelocity.Z = 0.0f;
                }

                direction.Z -= 1.0f;
            }
            else
            {
                movingAlongPosZ = false;
            }

            if (Utility.InputState.IsKeyDown(actionKeys[Actions.MoveUpPrimary]) ||
                Utility.InputState.IsKeyDown(actionKeys[Actions.MoveUpAlternate]))
            {
                if (!movingAlongPosY)
                {
                    movingAlongPosY = true;
                    currentVelocity.Y = 0.0f;
                }

                direction.Y += 1.0f;
            }
            else
            {
                movingAlongPosY = false;
            }

            if (Utility.InputState.IsKeyDown(actionKeys[Actions.MoveDownPrimary]) ||
                Utility.InputState.IsKeyDown(actionKeys[Actions.MoveDownAlternate]))
            {
                if (!movingAlongNegY)
                {
                    movingAlongNegY = true;
                    currentVelocity.Y = 0.0f;
                }

                direction.Y -= 1.0f;
            }
            else
            {
                movingAlongNegY = false;
            }

            if (Utility.InputState.IsKeyDown(actionKeys[Actions.Jump]) &&
                 Utility.InputState.IsKeyDown(actionKeys[Actions.Jump]) || Utility.InputState.IsButtonJoYstickPCDown(1))
            {
                switch (posture)
                {
                    case Posture.Standing:
                        if (bonground)
                            posture = Posture.Jumping;
                        direction.Y += 1.0f;
                        break;

                    case Posture.Jumping:
                        direction.Y = 0.0f;
                        break;

                    default:
                        break;
                }
            }

            switch (CurrentBehavior)
            {
                case Camera.Behavior.FirstPerson:
                case Camera.Behavior.Spectator:
                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.StrafeRightPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.StrafeRightAlternate])
#if !XBOX360
 || Utility.InputState.DirectionRightPadPc() != 0.0f
#endif

                        )
                    {
                        if (!movingAlongPosX)
                        {
                            movingAlongPosX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X += 1.0f;
                    }
                    else
                    {
                        movingAlongPosX = false;
                    }

                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.StrafeLeftPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.StrafeLeftAlternate])
#if !XBOX360
 || Utility.InputState.DirectionLeftPadPc() != 0.0f
#endif                     
                        )
                    {
                        if (!movingAlongNegX)
                        {
                            movingAlongNegX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X -= 1.0f;
                    }
                    else
                    {
                        movingAlongNegX = false;
                    }
                    /*
                        //###############################################################

                        if (currentKeyboardState.IsKeyDown(actionKeys[Actions.Crouch]))
                        {
                            switch (posture)
                            {
                                case Posture.Standing:
                                    posture = Posture.Crouching;
                                    direction.Y -= 1.0f;
                                    currentVelocity.Y = 0.0f;
                                    break;

                                case Posture.Crouching:
                                    direction.Y -= 1.0f;
                                    break;

                                case Posture.Rising:
                                    // Finish rising before allowing another crouch.
                                    direction.Y += 1.0f;
                                    break;

                                default:
                                    break;
                            }
                        }
                        else
                        {
                            switch (posture)
                            {
                                case Posture.Crouching:
                                    posture = Posture.Rising;
                                    direction.Y += 1.0f;
                                    currentVelocity.Y = 0.0f;
                                    break;

                                case Posture.Rising:
                                    direction.Y += 1.0f;
                                    break;

                                default:
                                    break;
                            }
                        }
                    */
                    
                    //###############################################################
                    break;


                case Camera.Behavior.Flight:
                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.FlightYawLeftPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.FlightYawLeftAlternate])
#if !XBOX360
 || Utility.InputState.DirectionLeftPadPc() != 0.0f
#endif                        
                        )
                    {
                        if (!movingAlongPosX)
                        {
                            movingAlongPosX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X += 1.0f;
                    }
                    else
                    {
                        movingAlongPosX = false;
                    }

                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.FlightYawRightPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.FlightYawRightAlternate])
#if !XBOX360
                || Utility.InputState.DirectionUpPadPc() != 0.0f
#endif
                        )
                    {
                        if (!movingAlongNegX)
                        {
                            movingAlongNegX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X -= 1.0f;
                    }
                    else
                    {
                        movingAlongNegX = false;
                    }
                    break;

                case Camera.Behavior.FreeOrbit:
                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollLeftPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollLeftAlternate])
#if !XBOX360
 || Utility.InputState.DirectionLeftPadPc() != 0.0f
#endif                        
                        )
                    {
                        if (!movingAlongPosX)
                        {
                            movingAlongPosX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X += 1.0f;
                    }
                    else
                    {
                        movingAlongPosX = false;
                    }

                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollRightPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollRightAlternate])
#if !XBOX360
 || Utility.InputState.DirectionRightPadPc() != 0.0f
#endif
                        )
                    {
                        if (!movingAlongNegX)
                        {
                            movingAlongNegX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X -= 1.0f;
                    }
                    else
                    {
                        movingAlongNegX = false;
                    }
                    break;


                case Camera.Behavior.ThirdPerson:
                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollLeftPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollLeftAlternate])
#if !XBOX360
 || Utility.InputState.DirectionLeftPadPc() != 0.0f
#endif
                        )
                    {
                        if (!movingAlongPosX)
                        {
                            movingAlongPosX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X += 1.0f;
                    }
                    else
                    {
                        movingAlongPosX = false;
                    }

                    if (Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollRightPrimary]) ||
                        Utility.InputState.IsKeyDown(actionKeys[Actions.OrbitRollRightAlternate])
#if !XBOX360
 || Utility.InputState.DirectionRightPadPc() != 0.0f
#endif 
                        )
                    {
                        if (!movingAlongNegX)
                        {
                            movingAlongNegX = true;
                            currentVelocity.X = 0.0f;
                        }

                        direction.X -= 1.0f;
                    }
                    else
                    {
                        movingAlongNegX = false;
                    }
                    break;

                default:
                    break;
            }
        }

        /// <summary>
        /// Determines which way the mouse wheel has been rolled.
        /// The returned value is in the range [-1,1].
        /// </summary>
        /// <returns>
        /// A positive value indicates that the mouse wheel has been rolled
        /// towards the player. A negative value indicates that the mouse
        /// wheel has been rolled away from the player.
        /// </returns>
        private float GetMouseWheelDirection()
        {
            int currentWheelValue = currentMouseState.ScrollWheelValue;
            int previousWheelValue = previousMouseState.ScrollWheelValue;

            if (currentWheelValue > previousWheelValue || Utility.InputState.IsButtonJoYstickPCDown(4))
                return -1.0f;
            else if (currentWheelValue < previousWheelValue || Utility.InputState.IsButtonJoYstickPCDown(5))
                return 1.0f;
            else
                return 0.0f;
        }

        /// <summary>
        /// Event handler for when the game window acquires input focus.
        /// </summary>
        /// <param name="sender">Ignored.</param>
        /// <param name="e">Ignored.</param>
        private void HandleGameActivatedEvent(object sender, EventArgs e)
        {
            if (savedMousePosX >= 0 && savedMousePosY >= 0)
                Mouse.SetPosition(savedMousePosX, savedMousePosY);
        }

        /// <summary>
        /// Event hander for when the game window loses input focus.
        /// </summary>
        /// <param name="sender">Ignored.</param>
        /// <param name="e">Ignored.</param>
        private void HandleGameDeactivatedEvent(object sender, EventArgs e)
        {
            MouseState state = Mouse.GetState();

            savedMousePosX = state.X;
            savedMousePosY = state.Y;
        }

        /// <summary>
        /// Filters the mouse movement based on a weighted sum of mouse
        /// movement from previous frames.
        /// <para>
        /// For further details see:
        ///  Nettle, Paul "Smooth Mouse Filtering", flipCode's Ask Midnight column.
        ///  http://www.flipcode.com/cgi-bin/fcarticles.cgi?show=64462
        /// </para>
        /// </summary>
        /// <param name="x">Horizontal mouse distance from window center.</param>
        /// <param name="y">Vertical mouse distance from window center.</param>
        private void PerformMouseFiltering(float x, float y)
        {
            // Shuffle all the entries in the cache.
            // Newer entries at the front. Older entries towards the back.
            for (int i = mouseSmoothingCache.Length - 1; i > 0; --i)
            {
                mouseSmoothingCache[i].X = mouseSmoothingCache[i - 1].X;
                mouseSmoothingCache[i].Y = mouseSmoothingCache[i - 1].Y;
            }

            // Store the current mouse movement entry at the front of cache.
            mouseSmoothingCache[0].X = x;
            mouseSmoothingCache[0].Y = y;

            float averageX = 0.0f;
            float averageY = 0.0f;
            float averageTotal = 0.0f;
            float currentWeight = 1.0f;

            // Filter the mouse movement with the rest of the cache entries.
            // Use a weighted average where newer entries have more effect than
            // older entries (towards the back of the cache).
            for (int i = 0; i < mouseSmoothingCache.Length; ++i)
            {
                averageX += mouseSmoothingCache[i].X * currentWeight;
                averageY += mouseSmoothingCache[i].Y * currentWeight;
                averageTotal += 1.0f * currentWeight;
                currentWeight *= mouseSmoothingSensitivity;
            }

            // Calculate the new smoothed mouse movement.
            smoothedMouseMovement.X = averageX / averageTotal;
            smoothedMouseMovement.Y = averageY / averageTotal;
        }

        /// <summary>
        /// Averages the mouse movement over a couple of frames to smooth out
        /// the mouse movement.
        /// </summary>
        /// <param name="x">Horizontal mouse distance from window center.</param>
        /// <param name="y">Vertical mouse distance from window center.</param>
        private void PerformMouseSmoothing(float x, float y)
        {
            mouseMovement[mouseIndex].X = x;
            mouseMovement[mouseIndex].Y = y;

            smoothedMouseMovement.X = (mouseMovement[0].X + mouseMovement[1].X) * 0.5f;
            smoothedMouseMovement.Y = (mouseMovement[0].Y + mouseMovement[1].Y) * 0.5f;

            mouseIndex ^= 1;
            mouseMovement[mouseIndex].X = 0.0f;
            mouseMovement[mouseIndex].Y = 0.0f;
        }

        /// <summary>
        /// Dampens the rotation by applying the rotation speed to it.
        /// </summary>
        /// <param name="headingDegrees">Y axis rotation in degrees.</param>
        /// <param name="pitchDegrees">X axis rotation in degrees.</param>
        /// <param name="rollDegrees">Z axis rotation in degrees.</param>
        private void RotateSmoothly(float headingDegrees, float pitchDegrees, float rollDegrees)
        {
            headingDegrees *= rotationSpeed;
            pitchDegrees *= rotationSpeed;
            rollDegrees *= rotationSpeed;

            Rotate(headingDegrees, pitchDegrees, rollDegrees);
        }

        /// <summary>
        /// Gathers and updates input from all supported input devices for use
        /// by the CameraComponent class.
        /// </summary>
        private void UpdateInput()
        {

            previousMouseState = currentMouseState;
            currentMouseState = Mouse.GetState();

            Rectangle clientBounds = Game.Window.ClientBounds;

            int centerX = clientBounds.Width / 2;
            int centerY = clientBounds.Height / 2;
            int deltaX = centerX - currentMouseState.X;
            int deltaY = centerY - currentMouseState.Y;

            Mouse.SetPosition(centerX, centerY);

            PerformMouseFiltering((float)deltaX, (float)deltaY);
            PerformMouseSmoothing(smoothedMouseMovement.X, smoothedMouseMovement.Y);
        }

        /// <summary>
        /// Updates the camera's velocity based on the supplied movement
        /// direction and the elapsed time (since this method was last
        /// called). The movement direction is the in the range [-1,1].
        /// </summary>
        /// <param name="direction">Direction moved.</param>
        /// <param name="elapsedTimeSec">Elapsed game time.</param>
        private void UpdateVelocity(ref Vector3 direction, float elapsedTimeSec)
        {
            if (direction.X != 0.0f)
            {
                // Camera is moving along the x axis.
                // Linearly accelerate up to the camera's max speed.

                currentVelocity.X += direction.X * acceleration.X * elapsedTimeSec;

                if (currentVelocity.X > velocity.X)
                    currentVelocity.X = velocity.X;
                else if (currentVelocity.X < -velocity.X)
                    currentVelocity.X = -velocity.X;
            }
            else
            {
                // Camera is no longer moving along the x axis.
                // Linearly decelerate back to stationary state.

                if (currentVelocity.X > 0.0f)
                {
                    if ((currentVelocity.X -= acceleration.X * 2.0f * elapsedTimeSec) < 0.0f)
                        currentVelocity.X = 0.0f;
                }
                else if (currentVelocity.X < 0.0f)
                {
                    if ((currentVelocity.X += acceleration.X * 2.0f * elapsedTimeSec) > 0.0f)
                        currentVelocity.X = 0.0f;
                }
            }

            if (direction.Y != 0.0f)
            {
                // Camera is moving along the y axis.
                // Linearly accelerate up to the camera's max speed.


                if (posture == Posture.Jumping)
                {
                    currentVelocity.Y += direction.Y * acceleration.Y * elapsedTimeSec - 12.0f * currentForce.Y * acceleration.Y * elapsedTimeSec;
                    posture = Posture.Standing;
                }
                //                currentVelocity.Y += direction.Y * acceleration.Y * elapsedTimeSec;

                if (currentVelocity.Y > velocity.Y - 12.0f * currentForce.Y * acceleration.Y * elapsedTimeSec)
                    currentVelocity.Y = velocity.Y - 12.0f * currentForce.Y * acceleration.Y * elapsedTimeSec;
                else if (currentVelocity.Y < -velocity.Y)
                    currentVelocity.Y = -velocity.Y;
            }
            else
            {
                // Camera is no longer moving along the y axis.
                // Linearly decelerate back to stationary state.


                if (currentVelocity.Y > 0.0f)
                {
                    if ((currentVelocity.Y -= acceleration.Y * 2.0f * elapsedTimeSec) < 0.0f)
                        currentVelocity.Y = 0.0f;
                }
                else if (currentVelocity.Y < 0.0f)
                {
                    if ((currentVelocity.Y += acceleration.Y * 2.0f * elapsedTimeSec) > 0.0f)
                        currentVelocity.Y = 0.0f;
                }
                
            }

            if (direction.Z != 0.0f)
            {
                // Camera is moving along the z axis.
                // Linearly accelerate up to the camera's max speed.

                currentVelocity.Z += direction.Z * acceleration.Z * elapsedTimeSec;

                if (currentVelocity.Z > velocity.Z)
                    currentVelocity.Z = velocity.Z;
                else if (currentVelocity.Z < -velocity.Z)
                    currentVelocity.Z = -velocity.Z;
            }
            else
            {
                // Camera is no longer moving along the z axis.
                // Linearly decelerate back to stationary state.

                if (currentVelocity.Z > 0.0f)
                {
                    if ((currentVelocity.Z -= acceleration.Z * 2.0f * elapsedTimeSec) < 0.0f)
                        currentVelocity.Z = 0.0f;
                }
                else if(currentVelocity.Z < 0.0f)
                {
                    if ((currentVelocity.Z += acceleration.Z * 2.0f * elapsedTimeSec) > 0.0f)
                        currentVelocity.Z = 0.0f;
                }
                
            }

            currentVelocity += currentForce * acceleration * elapsedTimeSec;
        }

        /// <summary>
        /// Moves the camera based on player input.
        /// </summary>
        /// <param name="direction">Direction moved.</param>
        /// <param name="elapsedTimeSec">Elapsed game time.</param>
        private void UpdatePosition(ref Vector3 direction, float elapsedTimeSec)
        {
            if (currentVelocity.LengthSquared() != 0.0f)
            {
                // Only move the camera if the velocity vector is not of zero
                // length. Doing this guards against the camera slowly creeping
                // around due to floating point rounding errors.

                Vector3 displacement = (currentVelocity * elapsedTimeSec);
               // +(0.5f * acceleration * elapsedTimeSec * elapsedTimeSec);

                // Floating point rounding errors will slowly accumulate and
                // cause the camera to move along each axis. To prevent any
                // unintended movement the displacement vector is clamped to
                // zero for each direction that the camera isn't moving in.
                // Note that the UpdateVelocity() method will slowly decelerate
                // the camera's velocity back to a stationary state when the
                // camera is no longer moving along that direction. To account
                // for this the camera's current velocity is also checked.

                if (direction.X == 0.0f && (float)Math.Abs(currentVelocity.X) < 1e-6f)
                    displacement.X = 0.0f;

                if (direction.Y == 0.0f && (float)Math.Abs(currentVelocity.Y) < 1e-6f)
                    displacement.Y = 0.0f;

                if (direction.Z == 0.0f && (float)Math.Abs(currentVelocity.Z) < 1e-6f)
                    displacement.Z = 0.0f;

                Move(displacement.X, displacement.Y, displacement.Z);
                
                /*
                switch (posture)
                {
                    case Posture.Standing:
                        break;

                    case Posture.Crouching:
                        if (Position.Y < eyeHeightCrouching)
                            Position = new Vector3(Position.X, eyeHeightCrouching, Position.Z);
                        break;

                    case Posture.Rising:
                        if (Position.Y > eyeHeightStanding)
                        {
                            Position = new Vector3(Position.X, eyeHeightStanding, Position.Z);
                            posture = Posture.Standing;
                            currentVelocity.Y = 0.0f;
                        }
                        break;

                    case Posture.Jumping:
                        if (Position.Y > 0.0f)
                        {
                            posture = Posture.Standing;
                            currentVelocity.Y = 0.0f;
                        }
                        break;
                }
                */
            }

            // Continuously update the camera's velocity vector even if the
            // camera hasn't moved during this call. When the camera is no
            // longer being moved the camera is decelerating back to its
            // stationary state.

            UpdateVelocity(ref direction, elapsedTimeSec);
        }

        private void UpdateOrientation(ref Vector3 direction, float elapsedTimeSec)
        {
            float headingDegrees = direction.X * elapsedTimeSec * orbitRollSpeed;

            float heading = MathHelper.ToRadians(headingDegrees);
            Quaternion rotation = Quaternion.Identity;

            if (heading != 0.0f)
            {
                Vector3 TargetYAxis = camera.TargetYAxis;
                Quaternion.CreateFromAxisAngle(ref TargetYAxis, -heading, out rotation);
                Quaternion Orientation = camera.Orientation;
                Quaternion.Concatenate(ref rotation, ref Orientation, out Orientation);
                camera.Orientation = Orientation;
            }

        }

        /// <summary>
        /// Updates the state of the camera based on player input.
        /// </summary>
        /// <param name="gameTime">Elapsed game time.</param>
        private void UpdateCamera(GameTime gameTime)
        {
            float elapsedTimeSec = 0.0f;

            if (Game.IsFixedTimeStep)
                elapsedTimeSec = (float)gameTime.ElapsedGameTime.TotalSeconds;
            else
                elapsedTimeSec = (float)gameTime.ElapsedRealTime.TotalSeconds;

            camera.ElapsedTime = elapsedTimeSec;

            Vector3 direction = new Vector3();

            if (bEndurance && Utility.InputState.IsKeyDown(actionKeys[Actions.Run]))
                velocity = velocityRunning;
            else
                velocity = velocityWalking;

            GetMovementDirection(out direction);

            float dx = 0.0f;
            float dy = 0.0f;
            float dz = 0.0f;

            switch (camera.CurrentBehavior)
            {
                case Camera.Behavior.FirstPerson:
                case Camera.Behavior.Spectator:
                    dx = smoothedMouseMovement.X;
                    dy = smoothedMouseMovement.Y;

                    RotateSmoothly(dx, dy, 0.0f);
                    UpdatePosition(ref direction, elapsedTimeSec);
                    break;

                case Camera.Behavior.Flight:
                    dy = -smoothedMouseMovement.Y;
                    dz = smoothedMouseMovement.X;

                    RotateSmoothly(0.0f, dy, dz);

                    if ((dx = direction.X * flightYawSpeed * elapsedTimeSec) != 0.0f)
                        camera.Rotate(dx, 0.0f, 0.0f);

                    direction.X = 0.0f; // ignore yaw motion when updating camera's velocity
                    UpdatePosition(ref direction, elapsedTimeSec);
                    break;

                case Camera.Behavior.FreeOrbit:
                    dx = -smoothedMouseMovement.X;
                    dy = -smoothedMouseMovement.Y;

                    RotateSmoothly(dx, dy, 0.0f);

                    if (!camera.PreferTargetYAxisOrbiting)
                    {
                        if ((dz = direction.X * orbitRollSpeed * elapsedTimeSec) != 0.0f)
                            camera.Rotate(0.0f, 0.0f, dz);
                    }

                    UpdatePosition(ref direction, elapsedTimeSec);

                    if ((dz = GetMouseWheelDirection() * mouseWheelSpeed) != 0.0f)
                        camera.Zoom(dz, camera.OrbitMinZoom, camera.OrbitMaxZoom);


                    break;


                case Camera.Behavior.ThirdPerson:

                    /*
                    RotateSmoothly(direction.X * orbitRollSpeed * 5.0f * elapsedTimeSec, 0.0f, 0.0f);

                    if (!camera.PreferTargetYAxisOrbiting)
                    {
                        if ((dz = direction.X * orbitRollSpeed  * elapsedTimeSec) != 0.0f)
                            camera.Rotate(0.0f, 0.0f, dz);
                    }

                    
                    */

                    
                    //UpdateOrientation(ref direction, elapsedTimeSec);

                    RotateSmoothly(direction.X * orbitRollSpeed * 5.0f * elapsedTimeSec, 0.0f, 0.0f);

                    direction.X = 0.0f;
                    UpdatePosition(ref direction, elapsedTimeSec);

                    if ((dz = GetMouseWheelDirection() * mouseWheelSpeed) != 0.0f)
                        camera.Zoom(dz, camera.OrbitMinZoom, camera.OrbitMaxZoom);

                    break;
                default:
                    break;
            }
        }

        #endregion

        #region Properties

        /// <summary>
        /// Property to get and set the camera's acceleration.
        /// </summary>
        public Vector3 Acceleration
        {
            get { return acceleration; }
            set { acceleration = value; }
        }


        public Vector3 VelocityWalking
        {
            get { return velocityWalking; }
            set
            {
                velocityWalking = value;
                VelocityRunning = velocityWalking * DEFAULT_RUNNING_MULTIPLIER;
            }
        }

        public Vector3 VelocityRunning
        {
            get { return velocityRunning; }
            set
            {
                velocityRunning = value;
                velocityWalking = velocityRunning / DEFAULT_RUNNING_MULTIPLIER;
            }
        }

        public float EyeHeightStanding
        {
            get { return eyeHeightStanding; }

            set
            {
                eyeHeightStanding = value;
                eyeHeightCrouching = value * HEIGHT_MULTIPLIER_CROUCHING;
                Position = new Vector3(Position.X, eyeHeightStanding, Position.Z);
            }
        }

        /// <summary>
        /// Property to get and set the camera's behavior.
        /// </summary>
        public Camera.Behavior CurrentBehavior
        {
            get { return camera.CurrentBehavior; }
            set { camera.CurrentBehavior = value; }
        }

        /// <summary>
        /// Property to get the camera's current velocity.
        /// </summary>
        public Vector3 CurrentVelocity
        {
            set { currentVelocity = value; }
            get { return currentVelocity; }
        }

        public Vector3 CurrentForce
        {
            get { return currentForce; }
            set { currentForce = value; }
        }

        public bool OnGround
        {
            get { return bonground; }
            set { bonground = value; }
        }

        /// <summary>
        /// Property to get and set the flight behavior's yaw speed.
        /// </summary>
        public float FlightYawSpeed
        {
            get { return flightYawSpeed; }
            set { flightYawSpeed = value; }
        }

        /// <summary>
        /// Property to get and set the sensitivity value used to smooth
        /// mouse movement.
        /// </summary>
        public float MouseSmoothingSensitivity
        {
            get { return mouseSmoothingSensitivity; }
            set { mouseSmoothingSensitivity = value; }
        }

        /// <summary>
        /// Property to get and set the speed of the mouse wheel.
        /// This is used to zoom in and out when the camera is orbiting.
        /// </summary>
        public float MouseWheelSpeed
        {
            get { return mouseWheelSpeed; }
            set { mouseWheelSpeed = value; }
        }

        /// <summary>
        /// Property to get and set the max orbit zoom distance.
        /// </summary>
        public float OrbitMaxZoom
        {
            get { return camera.OrbitMaxZoom; }
            set { camera.OrbitMaxZoom = value; }
        }

        /// <summary>
        /// Property to get and set the min orbit zoom distance.
        /// </summary>
        public float OrbitMinZoom
        {
            get { return camera.OrbitMinZoom; }
            set { camera.OrbitMinZoom = value; }
        }

        /// <summary>
        /// Property to get and set the distance from the target when orbiting.
        /// </summary>
        public float OrbitOffsetDistance
        {
            get { return camera.OrbitOffsetDistance; }
            set { camera.OrbitOffsetDistance = value; }
        }

        /// <summary>
        /// Property to get and set the orbit behavior's rolling speed.
        /// This only applies when PreferTargetYAxisOrbiting is set to false.
        /// Orbiting with PreferTargetYAxisOrbiting set to true will ignore
        /// any camera rolling.
        /// </summary>
        public float OrbitRollSpeed
        {
            get { return orbitRollSpeed; }
            set { orbitRollSpeed = value; }
        }

        /// <summary>
        /// Property to get and set the camera orbit target position.
        /// </summary>
        public Vector3 OrbitTarget
        {
            get { return camera.OrbitTarget; }
            set { camera.OrbitTarget = value; }
        }

        /// <summary>
        /// Property to get and set the camera orientation.
        /// </summary>
        public Quaternion Orientation
        {
            get { return camera.Orientation; }
            set { camera.Orientation = value; }
        }

        /// <summary>
        /// Property to get and set the camera position.
        /// </summary>
        public Vector3 Position
        {
            get { return camera.Position; }
            set { camera.Position = value; }
        }


        /// <summary>
        /// Property to get and set the flag to force the camera
        /// to orbit around the orbit target's Y axis rather than the camera's
        /// local Y axis.
        /// </summary>
        public bool PreferTargetYAxisOrbiting
        {
            get { return camera.PreferTargetYAxisOrbiting; }
            set { camera.PreferTargetYAxisOrbiting = value; }
        }

        /// <summary>
        /// Property to get the perspective projection matrix.
        /// </summary>
        public Matrix ProjectionMatrix
        {
            get { return camera.ProjectionMatrix; }
        }

        /// <summary>
        /// Property to get and set the mouse rotation speed.
        /// </summary>
        public float RotationSpeed
        {
            get { return rotationSpeed; }
            set { rotationSpeed = value; }
        }

        /// <summary>
        /// Property to get and set the camera's velocity.
        /// </summary>
        public Vector3 Velocity
        {
            get { return velocity; }
            set { velocity = value; }
        }

        public bool Endurance
        {
            get { return bEndurance; }
            set { bEndurance = value; }
        }

        /// <summary>
        /// Property to get the viewing direction vector.
        /// </summary>
        public Vector3 ViewDirection
        {
            get { return camera.ViewDirection; }
        }

        /// <summary>
        /// Property to get the view matrix.
        /// </summary>
        public Matrix ViewMatrix
        {
            get { return camera.ViewMatrix; }
        }

        /// <summary>
        /// Property to get the concatenated view-projection matrix.
        /// </summary>
        public Matrix ViewProjectionMatrix
        {
            get { return camera.ViewProjectionMatrix; }
        }

        /// <summary>
        /// Property to get the camera's local X axis.
        /// </summary>
        public Vector3 XAxis
        {
            get { return camera.XAxis; }
        }

        /// <summary>
        /// Property to get the camera's local Y axis.
        /// </summary>
        public Vector3 YAxis
        {
            get { return camera.YAxis; }
        }

        /// <summary>
        /// Property to get the camera's local Z axis.
        /// </summary>
        public Vector3 ZAxis
        {
            get { return camera.ZAxis; }
        }

        public Camera.PerformsCollision PerformsCollisionFonction
        {
            get { return camera.fonctionCollision; }
            set { camera.fonctionCollision = value; }
        }

        public BoundingFrustum Frustum
        {
            set { frustum = value; }
            get
            {
                if(frustum == null)
                    frustum = new BoundingFrustum(ViewProjectionMatrix);
                else
                    frustum.Matrix = ViewProjectionMatrix;

                return frustum;
            }
        }

        #endregion
    }
}