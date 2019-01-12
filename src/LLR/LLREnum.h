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

		enum class EStencilFunc
		{
			EQUAL,
			NOT_EQUAL,
			ALWAYS,
			NEVER
		};

		enum class EStencilOp
		{
			KEEP,
			ZERO,
			REPLACE,
		};

		enum EFramebufferComponent
		{
			NONE	= 0,
			COLOR	= 1 << 0,
			DEPTH	= 1 << 1,
			STENCIL	= 1 << 2,
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