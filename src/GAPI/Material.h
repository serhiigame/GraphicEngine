#pragma once

#include "api.h"

#include "EGApiEnums.h"
#include "Shader.h"

#include <map>
#include <set>
#include <vector>

namespace engine
{
	namespace graphic
	{
		class IShaderInput;
		class MaterialObject;
		class Shader;
		class ShaderInfo;
		class ShaderInputInfoBase;
		class Mesh;	

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

		struct MaterialInput
		{
			ShaderInputInfoBasePtr ShaderInputInfo;
			IShaderInput * ShaderInput;
		};

		class MaterialInstance;

		class MaterialObject
		{
			friend class GApiImpl;
		public:

			MaterialObject() {}

			MaterialObject(const MaterialObject & rv) {
				Copy(rv);
			}

			MaterialObject & operator=(const MaterialObject & rv)
			{
				Copy(rv);
				return * this;
			}

			bool IsValid() const { return m_materialId != NULL && !m_shader; }

			int GetMaterilId() const { return m_materialId; }

			Shader * GetMaterilShader() { return m_shader; }

			ShaderInfo GetMaterilShaderDesc() { return m_shaderInfo; }

			void AddMaterialInstance(MaterialInstance * material) { m_materialStorages.push_back(material); }

			const std::vector<MaterialInstance *> & GetMaterialInstances() const { return m_materialStorages; }

			//IShaderInput * GetMaterialInputFallback(const std::string & name);

			int GetId() const { return m_materialId; }

		private:
			void Copy(const MaterialObject & rv)
			{
				this->m_materialId = rv.m_materialId;
				this->m_materialStorages = rv.m_materialStorages;
				this->m_shader = rv.m_shader;
				this->m_shaderInfo = rv.m_shaderInfo;
			}

			void SetMaterilId(const int id) { m_materialId = id; }

			void SetMaterialShader(Shader * shader) { m_shader = shader; }

			void SetMaterialShaderDesc(ShaderInfo & shaderDesc) { m_shaderInfo = shaderDesc; }

			Shader * m_shader = nullptr;
			ShaderInfo m_shaderInfo;

			std::vector<MaterialInstance *> m_materialStorages;
			int m_materialId = NULL;
		};

		bool operator<(const MaterialObject& lhs, const MaterialObject & rhs);

		class MaterialInstance final
		{
			friend class GApiImpl;
			friend class MaterialManager;
		public:
			MaterialInstance(MaterialObject * material) : m_pMaterial(material){
				material->AddMaterialInstance(this);
			}
			//MaterialInstance(MaterialObject * pMaterialGbufferObject, MaterialObject * pMaterialLightingObject)
			//	: m_pMaterialGbufferObject(pMaterialGbufferObject)
			//	, m_pMaterialLightingObject(pMaterialLightingObject)
			//{
			//	m_pMaterialGbufferObject->AddMaterialInstance(this);
			//	m_pMaterialLightingObject->AddMaterialInstance(this);

			//}
			//MaterialObject * GetMaterialGbufferObject() { return m_pMaterialGbufferObject; }
			MaterialObject * GetMaterial() { return m_pMaterial; }

			void AddMaterialInput(IShaderInput * shaderInput, ShaderInputInfoBasePtr shaderInputInfo);

			const std::vector<MaterialInput> & GetMaterialInputs();

			int GetId() const { return m_materialInstanceId; }

		private:
			std::vector<MaterialInput> m_materilInputs;


			//std::map<int, IShaderInput * > m_materialGbufferInputs;
			//std::map<int, IShaderInput * > m_materialLightingInputs;

			//TODO: GeMaterial
			
			//MaterialObject * m_pMaterialGbufferObject;
			MaterialObject * m_pMaterial;

			int m_materialInstanceId = NULL;
		};

		bool operator<(const MaterialInstance& lhs, const MaterialInstance & rhs);


		struct MaterialStorage
		{
			GeMaterial Material;
			std::set<MaterialInstance *> MaterialInstances;

			bool operator<(const MaterialStorage r) const {  return this->Material < r.Material; }
		};

		class MaterialManager
		{
		public:
			GAPI_EXPORT
			static ShaderInputInfoBasePtr MakeShaderTexture2dInputDesc(const std::string & Name, const int Binding, const GeTexture2d & fallback);

			GAPI_EXPORT
			static ShaderInputInfoBasePtr MakeShaderTextureCubeMapInputDesc(const std::string & Name, const int Binding, const GeTextureCubeMap & fallback);

		public:
			void RegisterGbuffer(const GeShader & shader, const ShaderInfo & shaderInfo);

			void RegisterEnvMap(const GeShader & shader, const ShaderInfo & shaderInfo);

			void RegistreMaterial(const GeMaterial & material);

			void RegistreMaterialInstance(const GeMaterial & material, MaterialInstance * materialInstance);

			void SetMeshMaterialInstance(GeMesh mesh, MaterialInstance * materialInstance);

			void SetDefaultMaterialInstance(MaterialInstance * materialInstance);

			MaterialInstance * GetDefaultMaterialInstance();

			const ShaderInfo & GetGbufferShaderInfo() const;

			std::set<GeMaterial> GetMaterials() const;

			std::map<int, IShaderInput *> GetGbufferInputs(MaterialInstance * materialInstance);

			std::map<int, IShaderInput *> GetLightingInputs(MaterialInstance * materialInstance);

			std::map<int, IShaderInput *>  GetEnvMapInputs();

			const std::set<MaterialInstance *> & GetMaterialInstances(const GeMaterial & material) const;

			MaterialInstance * GetMeshMaterialInstance(const GeMesh & mesh);

		private:
			GeShader m_gBuffShader;
			ShaderInfo m_gBuffShaderInfo;

			GeShader m_envMapShader;
			ShaderInfo m_envMapShaderInfo;

			std::set<MaterialStorage> m_materialStorages;
			std::multimap<GeMaterial, MaterialInstance *> m_materialInstances;

			std::map<GeMesh, MaterialInstance *> m_meshMaterialRelationship;
			MaterialInstance * m_defaultMaterialInstance = nullptr;
		};
	}
}