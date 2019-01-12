#pragma once
#include <vector>
#include <string>

#include "LLREnum.h"

#include "IWindow.h"
#include "IShader.h"
#include "IBuffer.h"
#include "IConstant.h"
#include "ITexture2D.h"
#include "ITextureCubeMap.h"
#include "IFramebuffer.h"
#include "IRenderbuffer.h"
#include "IRenderPass.h"

namespace engine
{
	namespace graphic
	{
		class Llr
		{
		public:

			__declspec(dllexport)
			static Llr * CreateLlr(ELlrType type);

			__declspec(dllexport)
			static void DeleteLlr(Llr *& llr);

			__declspec(dllexport)
			virtual void Init(IWindow * window) {};

			__declspec(dllexport)
			virtual IShader * CreateShader(const std::string & vertexPass
				, const std::string & fragmentPass
				, const std::string & tesselationPass = std::string()
				, const std::vector<std::string> define = std::vector<std::string>()) = 0;

			__declspec(dllexport)
			virtual IBuffer * CreateBuffer(size_t size) = 0;

			__declspec(dllexport)
			virtual IBuffer * CreateIndexBuffer(size_t size) = 0;

			__declspec(dllexport)
			virtual IConstant * CreateConatant(const size_t size) = 0;

			__declspec(dllexport)
			virtual ITexture2D * CreateTexture2d(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType) = 0;

			__declspec(dllexport)
				virtual ITextureCubeMap * CreateTextureCubeMap(const size_t width, const size_t heigth, const ETextureFormat format, const EDataType dataType) = 0;

			__declspec(dllexport)
			virtual IFramebuffer * CreateFramebuffer(const size_t width, const size_t heigth) = 0;

			__declspec(dllexport)
			virtual IRenderbuffer * CreateRenderbuffer(const size_t width, const size_t heigth) = 0;

			__declspec(dllexport)
			virtual IRenderPass * CreateRenderPass() = 0;
		protected:
			Llr() = default;
			virtual ~Llr() = default;
			Llr(Llr &) = default;
			Llr & operator=(Llr &) = default;
		};
	}
}
