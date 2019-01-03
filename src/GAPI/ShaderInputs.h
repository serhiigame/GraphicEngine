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
			, TEXTURE_CUBEMAP
		};
		
		class IConstant;
		class Texture2d;
		
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

		class ShaderInputTexture2d : public IShaderInput
		{
			friend class GApi;
			friend class GApiImpl;
			friend class Shader;
		public:
			ShaderInputTexture2d() : IShaderInput(EShaderInputType::TEXTURE_2D) {}
			const Texture2d * GetTexture() const {
				return m_texture;
			}
		private:
			Texture2d * m_texture;
		};
	}
}