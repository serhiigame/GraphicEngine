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


		enum class ETextureFormat
		{
			RGB,
			RGBA,
			RGBf,
			RGBAf,
			NONE = -1
		};

		enum class ETextureCubeMapFace
		{
			NONE = -1
			, BACK
			, FRONT
			, LEFT
			, RIGHT
			, TOP
			, BUTTOM
		};

	}
}