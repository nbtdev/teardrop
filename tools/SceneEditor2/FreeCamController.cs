using System;
using System.Collections.Generic;
using System.Text;

using CoS;
using CoS.Gfx;
using CoS.Math;

namespace SceneEditor
{
    public class FreeCamController : CameraController
    {
        public FreeCamController(Camera cam) 
            : base(cam)
        {
        }

        public override void Update(double deltaT)
        {
        }
    }
}
