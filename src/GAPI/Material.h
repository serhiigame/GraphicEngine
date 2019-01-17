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
			std::string Name;
			EMaterialInputType Type = EMaterialInputType::TEXTURE;
			int Binding;
		};
	

		struct ShaderDesc
		{
			std::string vertShaderPath;
			std::string fragShaderPath;

			std::vector<ShaderInputDesc> Inputs;
			std::vector<ShaderOutputDesc> Outputs;
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


		class MaterialInstance;

		class MaterialObject
		{
			friend class GApiImpl;
		public:
			bool IsValid() const { return m_materialId != NULL && !m_shader; }

			int GetMaterilId() const { return m_materialId; }

			Shader * GetMaterilShader() { return m_shader; }

			ShaderDesc GetMaterilShaderDesc() { return m_shaderDesc; }

			void AddMaterialInstance(MaterialInstance * material) { m_materials.push_back(material); }

			std::vector<MaterialInstance *> & GetMaterialInstances() { return m_materials; }

		private:
			void SetMaterilId(const int id) { m_materialId = id; }

			void SetMaterialShader(Shader * shader) { m_shader = shader; }

			void SetMaterialShaderDesc(ShaderDesc & shaderDesc) { m_shaderDesc = shaderDesc; }

			Shader * m_shader = nullptr;
			ShaderDesc m_shaderDesc;
			std::vector<MaterialInstance *> m_materials;
			int m_materialId = NULL;
		};

		class MaterialInstance final
		{
			friend class GApiImpl;
		public:
			MaterialInstance(MaterialObject * pMaterialGbufferObject, MaterialObject * pMaterialLightingObject)
				: m_pMaterialGbufferObject(pMaterialGbufferObject)
				, m_pMaterialLightingObject(pMaterialLightingObject)
			{
				m_pMaterialGbufferObject->AddMaterialInstance(this);
				m_pMaterialLightingObject->AddMaterialInstance(this);

			}
			MaterialObject * GetMaterialGbufferObject() { return m_pMaterialGbufferObject; }
			MaterialObject * GetMaterialLightingObject() { return m_pMaterialLightingObject; }

			int GetId() const { return m_materialInstanceId; }

		protected:
			std::map<int, IShaderInput * > m_materialGbufferInputs;
			std::map<int, IShaderInput * > m_materialLightingInputs;

			MaterialObject * m_pMaterialGbufferObject;
			MaterialObject * m_pMaterialLightingObject;

			int m_materialInstanceId = NULL;
		};

		bool operator<(const MaterialInstance& lhs, const MaterialInstance & rhs);
	}
}