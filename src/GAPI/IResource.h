#pragma once

namespace engine
{
	namespace graphic
	{		
		class IResource
		{
		public:
			IResource() {
				static int idCounter = 0;
				m_id = idCounter++;
			}
			virtual ~IResource() {};

			int GetId() const {	return m_id; }
		protected:
			int m_id = -1;
		};
		
	}
}