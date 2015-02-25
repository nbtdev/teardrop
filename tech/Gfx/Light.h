/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(GFXLIGHT_INCLUDED)
#define GFXLIGHT_INCLUDED

#include "Gfx/Allocators.h"
#include "Math/Vector4.h"

namespace Teardrop
{
    namespace Gfx {
        class RenderTarget;
        class Camera;

        class Light
        {
        public:
            Light();
            virtual ~Light();

            enum Type
            {
                LT_POINT,
                LT_SPOT,
                LT_DIRECTIONAL,
            };

            /** Accessors
            */
            //! these usually come from a scene-graph override
            virtual const Vector4& getPosition() const;
            //! not meaningful for point light
            const Vector4& getDirection() const;
            //! get light type
            Type getType() const;
            //! use only to determine whether an object can be affected by this light
            float getRange() const;
            //! actually determines the amount of light that falls on an object
            void getAttenuation(float& /*out*/constant, float& /*out*/linear, float& /*out*/quadratic) const;
            //! used only with spotlight types
            void getSpotParams(float& /*out*/innerAngle, float& /*out*/outerAngle) const;
            //! used for all types (this is the primary color, if the light supports multiple colors)
            void getColor(float& /*out*/r, float& /*out*/g, float& /*out*/b) const;
            void getColor(unsigned char& /*out*/r, unsigned char& /*out*/g, unsigned char& /*out*/b) const;
            RenderTarget* getShadowTexture();
            const RenderTarget* getShadowTexture() const;
            Camera* getShadowCamera();
            const Camera* getShadowCamera() const;

            /** Mutators
            */
            //! set light direction (inapplicable for point lights)
            void setDirection(const Vector4& direction);
            //! set point, spot or directional light type
            void setType(Type type);
            //! only used to determine whether an object can be affected by this light
            void setRange(float range);
            //! actually determines the amount of light that falls on an object
            void setAttenuation(float constant=1, float linear=0, float quadratic=0);
            //! used only with spotlight types
            void setSpotParams(float innerAngle, float outerAngle);
            //! used for all types (this is the primary color, if the light supports multiple colors)
            void setColor(float r, float g, float b);
            void setColor(unsigned char r, unsigned char g, unsigned char b);
            void setShadowCamera(Camera* pCam);

            DECLARE_GFX_ALIGNED_ALLOCATOR();

        private:
            Vector4 m_direction;
            float m_range;
            float m_constant;
            float m_linear;
            float m_quadratic;
            float m_innerAngle;
            float m_outerAngle;
            float m_red;
            float m_green;
            float m_blue;
            Type m_type;

            // for shadowmapping
            RenderTarget* m_pShadowTexture;
            Camera* m_pShadowCam;
            void createShadowCamera();
        };
    } // namespace Gfx
} // namespace Teadrop

#endif // GFXLIGHT_INCLUDED
