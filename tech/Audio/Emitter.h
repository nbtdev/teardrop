/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(AUDIO_EMITTER_INCLUDED)
#define AUDIO_EMITTER_INCLUDED

namespace Teardrop
{
	class Transform;

	namespace Audio
	{
		class Sample;

		/*
			Audio emitter that exists in 3D space; can reference Stream or Sample
		*/
		class Emitter
		{
		public:
			Emitter();
			virtual ~Emitter();

			//! 'source' is reference to Stream or Sample
			virtual void initialize() = 0;
			virtual void destroy() = 0;
			virtual void setTransformWS(const Transform& transform) = 0;
		};
	}
}

#endif // AUDIO_EMITTER_INCLUDED
