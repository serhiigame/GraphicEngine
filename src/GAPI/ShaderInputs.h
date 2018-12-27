#pragma once

#include "IResource.h"

#include "Math.h"
//#include <stdint.h>

namespace engine
{
	namespace graphic
	{
		
		enum class EShaderInputType
		{
			NONE = -1
			, CONSTANT = 0
			, TEXTURE_2D
			, TEXTURECUBEMAP
		};
		
		class IConstant;
		
		class IShaderInput : public IResource
		{
		public:
			IShaderInput(EShaderInputType type) : m_type(type) {}
			virtual ~IShaderInput() {}

			virtual const EShaderInputType GetShaderInputType() const { return m_type; }

		protected:
			const EShaderInputType m_type;
		};

		class ShaderInputConstant : public IShaderInput
		{
			friend class GApi;
			friend class GApiImpl;
			friend class Shader;
		public:
			~ShaderInputConstant();
			ShaderInputConstant() : IShaderInput(EShaderInputType::CONSTANT) {}
		private:
			IConstant * m_constant;
		};
	}
}