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

		enum class EDataType : int
		{
			FLOAT,
			UNSIGNED_INT,
			NONE = -1
		};



		enum class ETextureFormat : int
		{
			RED,
			RED_INTEGER,
			RGB,
			RGB_INTEGER,
			RGBA,
			RGBA_INTEGER,
			NONE = -1
		};

		enum class ETextureSizedFormat : int
		{
			RED_UNSIGNED_INT_32,
			RED_FLOAT,
			RGB_UNSIGNED_INT_32,
			RGB_FLOAT,
			RGBA_UNSIGNED_INT_32,
			RGBA_FLOAT,
			NONE = -1
		};

		enum class ETextureCubeMapFace : int
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