#pragma once
#include "IShader.h"

#include <GLEW/glew.h>

#include <set>
#include <map>

namespace engine
{
	namespace graphic
	{
		class ShaderOpenGL : public IShader
		{
			friend class LlrOpenGL;
		public:
			static ShaderOpenGL * CreateShader(const char * vertCodeSrc, const char * fragCodeSrc, const char * tessCodeSrc);

		public:
			virtual ~ShaderOpenGL();

			virtual void AttachAttribute(const IBuffer * buffer, const int location, const size_t offset, const size_t count, const EDataType & type) override;

			virtual  void AttachConstant(const IConstant * constant, const int location)  override;

			virtual  void AttachTexture2d(const ITexture2D * texture, const int location)  override;

			virtual  void AttachTextureCubeMap(const ITextureCubeMap * texture, const int location) override;

			GLuint GetProgramId() const { return m_programId; }

			GLuint GetVaoId() const { return m_vao; }

			const std::set<GLuint> & GetVertexAttributeIds() const { return m_vertexAtributeIds; }

			const std::map<int, GLuint> & GetUniformBlock() const { return m_uniformBlock; }

			const std::map<GLuint, GLuint> & GetTexture2dAttach() const { return m_texture2dAttachment; }

			const std::map<GLuint, GLuint> & GetTextureCubeMapAttach() const { return m_textureCubeMapAttachment; }

			const std::map<GLuint, size_t> & GetIndexIds() const { return m_indexIds; }

		private:
			ShaderOpenGL() {}

			GLuint m_programId = ((GLuint)-1);
			GLuint m_vao = ((GLuint)-1);

			std::set<GLuint> m_vertexAtributeIds;
			std::map<int, GLuint> m_uniformBlock;
			std::map<GLuint, GLuint> m_texture2dAttachment; // attachment index / texture id
			std::map<GLuint, GLuint> m_textureCubeMapAttachment; // attachment index / texture id
			std::map<GLuint, size_t> m_indexIds;
		};
	}
}