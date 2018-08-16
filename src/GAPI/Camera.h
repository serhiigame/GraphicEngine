#pragma once

#include "IResource.h"

#include <vector>

namespace engine
{
	namespace graphic
	{
		class IConstant;
		
		class Camera : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:

		protected:
			IConstant * m_cameraTransforms = nullptr;
		};
	}
}