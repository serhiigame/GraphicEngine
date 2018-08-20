#include "RenderPassGl.h"

#include "ShaderOpenGL.h"
#include "FramebufferGL.h"

#include <GLEW/glew.h>

namespace engine
{
	namespace graphic
	{
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

			for (const auto & texAttach : shaderGl->GetTextureAttach())
			{
				const GLuint attachmenyId = texAttach.first;
				const GLuint id = texAttach.second;

				glActiveTexture(GL_TEXTURE0 + attachmenyId);
				glBindTexture(GL_TEXTURE_2D, id);
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
	}
}