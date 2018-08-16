#pragma once


namespace engine
{
	namespace graphic
	{
		enum class EBufferType
		{
			VERTEX,
			INDEX,
			NONE = -1
		};
		
		class IBuffer
		{
		public:
			virtual ~IBuffer() {};

			virtual void Init(const size_t size, const EBufferType type) = 0;

			virtual void Write(const size_t offset, const size_t size, const void * data) = 0;

			virtual void Read(const size_t offset, const size_t size, void * o_data) = 0;
			
			size_t GetSize() const { return m_Size; }

			EBufferType GetBufferType() const {return m_Type; }

		protected:
			EBufferType m_Type = EBufferType::NONE;
			size_t m_Size = 0ULL;
		};
	}
}