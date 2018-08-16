#pragma once

#include "ITransform.h"


namespace engine
{
	namespace graphic
	{		
		class GeomTransform : ITransform
		{
		public:
			virtual void SetTranslation(Vec3f translation) override
			{
				transform.SetTranslation(translation);
			}

			virtual void SetQuaternion(Quatf quaternion) override
			{
			
			}

			virtual void SetScale(Vec3f scale) override
			{
			
			}

			Mat4f GetTransformationMatrix()
			{
				return transform.GetHMatrix();
			}

		protected:
			gte::Transform transform;
		};
		
	}
}