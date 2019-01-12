#include "RenderPassGl.h"

#include "ShaderOpenGL.h"
#include "FramebufferGL.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{
		
		inline GLenum AdapterGlStencilFunc(EStencilFunc func)
		{
			switch (func)
			{
			case EStencilFunc::ALWAYS:		return GL_ALWAYS;
			case EStencilFunc::NEVER:		return GL_NEVER;
			case EStencilFunc::EQUAL:		return GL_EQUAL;
			case EStencilFunc::NOT_EQUAL:	return GL_NOTEQUAL;
			default:
				break;
			}

			return NULL;
		}
		
		inline GLenum AdapterGlStencilOp(EStencilOp op)
		{
			switch (op)
			{
			case EStencilOp::KEEP:		return GL_KEEP;
			case EStencilOp::ZERO:		return GL_ZERO;
			case EStencilOp::REPLACE:	return GL_REPLACE;
			default:
				break;
			}

			return NULL;
		} 
		
		
		IRenderPass * RenderPassGL::CreateRenderPass()
		{
			return new RenderPassGL;
		}

		EApiRenderPassStatus RenderPassGL::Execute(IShader * shader, IFramebuffer * outputBuffer )
		{
			const ShaderOpenGL * shaderGl = (const ShaderOpenGL *)shader;

			GLuint buffId = (outputBuffer) ? ((const FramebufferGL *)outputBuffer)->GetId() : 0;

			glBindFramebuffer(GL_FRAMEBUFFER, buffId);

			glUseProgram(shaderGl->GetProgramId());

			// TODO: move to shader options
			glEnable(GL_DEPTH_TEST);

			glEnable(GL_STENCIL_TEST);

		//	glBindVertexArray(shaderGl->GetVaoId());

			for (const GLuint & id : shaderGl->GetVertexAttributeIds())
			{
				glEnableVertexAttribArray(id);
			}

			for (auto uniformBlock : shaderGl->GetUniformBlock())
			{
				const int bindingId = uniformBlock.first;
				const GLuint bufferId = uniformBlock.second;
				glBindBufferBase(shaderGl->GetProgramId(), bindingId, bufferId);

			}

			for (const auto & texAttach : shaderGl->GetTexture2dAttach())
			{
				const GLuint attachmenyId = texAttach.first;
				const GLuint id = texAttach.second;

				glActiveTexture(GL_TEXTURE0 + attachmenyId);
				glBindTexture(GL_TEXTURE_2D, id);
			}


			for (const auto & texAttach : shaderGl->GetTextureCubeMapAttach())
			{
				const GLuint attachmenyId = texAttach.first;
				const GLuint id = texAttach.second;

				

				glActiveTexture(GL_TEXTURE0 + attachmenyId);
				//glBindTexture(GL_TEXTURE_2D, 0);

				glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			}

			for (const auto & indexBuffInfo : shaderGl->GetIndexIds())
			{
				const GLuint id = indexBuffInfo.first;
				const size_t count = indexBuffInfo.second;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
				glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);
			}

			for (const GLuint & id : shaderGl->GetVertexAttributeIds())
			{
				glDisableVertexAttribArray(id);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(0);

			return EApiRenderPassStatus::SUCCESS;
		}
		void RenderPassGL::EnableStencilWriting()
		{
			glStencilMask(0xFF);
		}
		void RenderPassGL::DisableStencilWriting()
		{
			glStencilMask(0x00);
		}
		void RenderPassGL::SetStencilParams(EStencilFunc func, const int ref, EStencilOp passOp, EStencilOp failOp)
		{
			glStencilFunc(AdapterGlStencilFunc(func), 1, 0xFF);
			glStencilOp(AdapterGlStencilOp(passOp), AdapterGlStencilOp(failOp), AdapterGlStencilOp(failOp));
		}
	}
}