#pragma once

namespace engine
{
	namespace graphic
	{
		// Api

		enum class ELlrType
		{
			OPEN_GL,
			VULKAN
		};

		enum class EApiRenderPassStatus
		{
			SUCCESS,
			FAIL
		};

		enum class EDataType
		{
			FLOAT,
			UNSIGNED_INT,
			NONE = -1
		};


		// TEXTURE

		enum class ETextureType
		{
			TEXTURE_2D,
			TEXTURE_CUBE,
			NONE = -1
		};

		enum class ETextureFormat
		{
			RGB,
			RGBA,
			NONE = -1
		};

	}
}