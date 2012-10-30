using System;
using System.Collections.Generic;
using System.Text;

using CoS;
using CoS.Gfx;
using CoS.Math;

namespace SceneEditor
{
    public class CameraController
    {
        protected Camera m_cam = null;

        protected CameraController(Camera cam)
        {
            m_cam = cam;
        }

        public virtual void Update(double deltaT)
        {

        }
    }
}
