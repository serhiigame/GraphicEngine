#pragma once

namespace engine
{
	namespace graphic
	{		
		class IResource
		{
		public:
			int GetId() const {	return m_id; }
		protected:
			IResource() {
				static int idCounter = 0;
				m_id = idCounter++;
			}
			virtual ~IResource() {};

			int m_id = -1;
		};
		
	}
}