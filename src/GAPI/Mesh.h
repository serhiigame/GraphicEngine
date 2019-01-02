#pragma once

#include "GeomTransform.h"
#include "IResource.h"

#include <string>
#include <vector>

namespace engine
{
	namespace graphic
	{
		class IBuffer;
		class IConstant;
		
		class Mesh : public GeomTransform, IResource
		{
			friend class GApi;
			friend class GApiImpl;
		public:
			~Mesh()
			{
				if (m_pos)
				{
					delete m_pos;
					m_pos = nullptr;
				}

				if (m_norm)
				{
					delete m_norm;
					m_norm = nullptr;
				}

				if (m_idx)
				{
					delete m_idx;
					m_idx = nullptr;
				}

				if (m_transform)
				{
					delete m_transform;
					m_transform = nullptr;
				}
			}
		protected:
			IBuffer * m_pos = nullptr;
			IBuffer * m_norm = nullptr;
			IBuffer * m_uv = nullptr;
			IBuffer * m_idx = nullptr;
			IConstant * m_transform = nullptr;
		};
	}
}