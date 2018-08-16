#pragma once


namespace engine
{
	namespace graphic
	{
		class IConstant
		{
		public:
			virtual ~IConstant() {}

			virtual void Write(const size_t offset, const size_t size, const void * data) = 0;

			virtual void Read(const size_t offset, const size_t size, void * o_data) = 0;

			size_t GetSize() const { return m_size; }

		protected:
			size_t m_size = 0;
		};
	}
}