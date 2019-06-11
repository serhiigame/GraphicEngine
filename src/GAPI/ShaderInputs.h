#pragma once

#include "EGApiEnums.h"

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
		
		class IShaderInput
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

			GeTexture2d GetTexture() const;
			void SetTexture(const GeTexture2d & texture);

		private:
			GeTexture2d m_texture;
		};

		struct ShaderInputInfoBase
		{
			ShaderInputInfoBase(EShaderInputType type) : Type(type) {}
			std::string Name;
			EShaderInputType Type;
			int Binding;
		};

		struct ShaderInputTexture2dInfo final : ShaderInputInfoBase
		{
			ShaderInputTexture2dInfo() : ShaderInputInfoBase(EShaderInputType::TEXTURE_2D) {}
			GeTexture2d FallbackTexture;
		};


		struct ShaderInputVec4fInfo final : ShaderInputInfoBase
		{
			ShaderInputVec4fInfo() : ShaderInputInfoBase(EShaderInputType::CONSTANT) {}

			float FallbackVec4f[4] = {};
		};


		//TODO
		//struct ShaderInputCustomeInfo final : ShaderInputInfoBase
		//{
		//};

		struct ShaderOutputInfo
		{
			std::string Name;
			int Binding;
			GeTexture2d OutputTexture;
		};
	}
}