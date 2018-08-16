#pragma once
#include "Math.h"


namespace engine
{
	namespace graphic
	{
		
		
		class ITransform
		{
		public:
			virtual void SetTranslation(Vec3f translation) = 0;
			virtual void SetQuaternion(Quatf quaternion) = 0;
			virtual void SetScale(Vec3f scale) = 0;

			// TODO: complex transformation
		};
		
	}
}