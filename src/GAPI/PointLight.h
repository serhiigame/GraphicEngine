#pragma once

#include "IResource.h"

#include <vector>

namespace engine
{
	namespace graphic
	{
		class IConstant;
		
		class PointLight : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:

		protected:
			IConstant * m_pointLight = nullptr;
		};
	}
}