#pragma once

#include "IResource.h"

#include "Texture2d.h"

#include <map>
#include <vector>

namespace engine
{
	namespace graphic
	{
		class IShaderInput;
		class MaterialObject;
		class Material;
		class Shader;
		class Mesh;

		enum class EMaterialInputType
		{
			NONE = -1
			, TEXTURE = 1
			, VEC4F
			//, INT
			//, FLOAT
			//, COLOR
		};

		struct ShaderInputDesc
		{
			std::string Name;
			EMaterialInputType Type;
			int Binding;
		};

		struct ShaderOutputDesc
		{
			ShaderOutputDesc() : Type(EMaterialInputType::TEXTURE) {}
	
			std::string Name;
			const EMaterialInputType Type;
			int Binding;
		};
	

		struct ShaderInfo
		{
			std::string vertShaderPath;
			std::string fragShaderPath;

			std::vector<ShaderInputDesc> Inputs;
			std::vector<ShaderOutputDesc> Outputs;
		};

		struct MaterialDescription
		{
			Shader * Shader;
			std::vector<ShaderInputDesc> ShaderInputDescs;
		};

		class MaterialHandler
		{
			friend class GApi;
			friend class GApiImpl;
		public:
			bool IsValid() const { return m_materialId != NULL; }
			int GetMaterialId() const { return m_materialId; }
		private:
			int m_materialId = NULL;
		};

		

		class Material : public IResource
		{
			friend class GApi;
			friend class GApiImpl;
		protected:
			int m_materialId = NULL;
			std::map<int, IShaderInput * > m_materialInputs;
			std::vector<Mesh *> m_meshes;
		};
	}
}