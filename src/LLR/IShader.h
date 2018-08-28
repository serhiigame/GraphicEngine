#pragma once

#include <map>

#include "LLREnum.h"

namespace engine
{
	namespace graphic
	{
		class IBuffer;
		class IConstant;
		class ITexture2D;
		class ITextureCubeMap;

		typedef std::map<int, ITexture2D * > Texture2dBindings;
		typedef std::map<int, ITextureCubeMap * > TextureCubeMapBindings;
		typedef std::map<int, IConstant * > ConstantBindings;

		extern enum class EDataType type;

		class IShader
		{
		public:
			virtual ~IShader() {}

			virtual void AttachAttribute(const IBuffer * buffer, const int location, const size_t offset, const size_t count, const EDataType & type) = 0;

			virtual void AttachConstant(const IConstant * constant, const int location) = 0;

			virtual void AttachTexture2d(const ITexture2D * texture, const int location) = 0;

			virtual void AttachTextureCubeMap(const ITextureCubeMap * texture, const int location) = 0;
		};
	}
}