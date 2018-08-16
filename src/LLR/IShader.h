#pragma once

#include <map>

#include "LLREnum.h"

namespace engine
{
	namespace graphic
	{
		class IBuffer;
		class IConstant;
		class ITexture;

		typedef std::map<int, ITexture * > TextureBindings;
		typedef std::map<int, ITexture * > ConstantBindings;

		extern enum class EDataType type;

		class IShader
		{
		public:
			virtual ~IShader() {}

			virtual void AttachAttribute(const IBuffer * buffer, const int location, const size_t offset, const size_t count, const EDataType & type) = 0;

			virtual void AttachConstant(const IConstant * constant, const int location) = 0;

			virtual  void AttachTexture(const ITexture * constant, const int location) = 0;
		};
	}
}