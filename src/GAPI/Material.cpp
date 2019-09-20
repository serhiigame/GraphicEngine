#include "Material.h"
#include "Shader.h"
#include "ShaderInputs.h"

#include "GApi.h"

namespace engine
{
	namespace graphic
	{
		bool operator<(const MaterialObject & lhs, const MaterialObject & rhs)
		{
			return lhs.GetId() < rhs.GetId();
		}
		bool operator<(const MaterialInstance & lhs, const MaterialInstance & rhs)
		{
			return lhs.GetId() < rhs.GetId();
		}

		/*IShaderInput * MaterialObject::GetMaterialInputFallback(const std::string & name)
		{
			
			for (auto input : m_shaderDesc.Inputs)
			{
				if (input.Name == name)
				{
					return input.Fallback;
				}
			}

			return nullptr;
		}*/

		void MaterialInstance::AddMaterialInput(IShaderInput * shaderInput, ShaderInputInfoBasePtr shaderInputInfo)
		{
			MaterialInput materialInput;
			materialInput.ShaderInput = shaderInput;
			materialInput.ShaderInputInfo = shaderInputInfo;

			m_materilInputs.push_back(materialInput);
		}
		const std::vector<MaterialInput>& MaterialInstance::GetMaterialInputs()
		{
			return m_materilInputs;
		}

		ShaderInputInfoBasePtr MaterialManager::MakeShaderTexture2dInputDesc(const std::string & Name, const int Binding, const GeTexture2d & fallback)
		{
			ShaderInputTexture2dInfo * texInputInfo = new ShaderInputTexture2dInfo;
			texInputInfo->Binding = Binding;
			texInputInfo->Type = EShaderInputType::TEXTURE_2D;
			texInputInfo->Name = Name;
			texInputInfo->FallbackTexture = fallback;

			return ShaderInputInfoBasePtr(texInputInfo);
		}

		GAPI_EXPORT
		ShaderInputInfoBasePtr MaterialManager::MakeShaderTextureCubeMapInputDesc(const std::string & Name, const int Binding, const GeTextureCubeMap & fallback)
		{
			ShaderInputTextureCubeMapInfo * texInputInfo = new ShaderInputTextureCubeMapInfo;
			texInputInfo->Binding = Binding;
			texInputInfo->Type = EShaderInputType::TEXTURE_CUBEMAP;
			texInputInfo->Name = Name;
			texInputInfo->FallbackTexture = fallback;

			return ShaderInputInfoBasePtr(texInputInfo);
		}

		void MaterialManager::RegisterGbuffer(const GeShader & shader, const ShaderInfo & shaderInfo)
		{
			m_gBuffShader = shader;
			m_gBuffShaderInfo = shaderInfo;
		}

		void MaterialManager::RegisterEnvMap(const GeShader & shader, const ShaderInfo & shaderInfo)
		{
			m_envMapShader = shader;
			m_envMapShaderInfo = shaderInfo;
		}

		void MaterialManager::RegistreMaterial(const GeMaterial & geMaterial)
		{
			MaterialStorage materialStorage;
			materialStorage.Material = geMaterial;

			m_materialStorages.emplace(materialStorage);
		}

		void MaterialManager::RegistreMaterialInstance(const GeMaterial & material, MaterialInstance * materialInstance)
		{
			for (auto it = m_materialStorages.begin(), end = m_materialStorages.end(); it != end; ++it)
			{
				if (it->Material == material)
				{
					MaterialStorage & ms = const_cast<MaterialStorage &>(*it);
					ms.MaterialInstances.emplace(materialInstance);
				}
			}


			//std::set<MaterialStorage>::iterator finded = find_if(m_materialStorages.begin(), m_materialStorages.end(), [material](const GeMaterial & m) {m.GetId() == material.GetId(); });

			//if (finded != m_materialStorages.end())
			{
				//MaterialStorage & ms = const_cast<MaterialStorage &>(*finded);
				//ms.MaterialInstances.emplace(materialInstance);
			}
			//TODO: delete
			//m_materialInstances.emplace(material, materialInstance);
		}

		void MaterialManager::SetMeshMaterialInstance(GeMesh mesh, MaterialInstance * materialInstance)
		{
			m_meshMaterialRelationship.insert_or_assign(mesh, materialInstance);
		}

		void MaterialManager::SetDefaultMaterialInstance(MaterialInstance * materialInstance)
		{
			m_defaultMaterialInstance = materialInstance;
		}

		MaterialInstance * MaterialManager::GetDefaultMaterialInstance()
		{
			return m_defaultMaterialInstance;
		}

		const ShaderInfo & MaterialManager::GetGbufferShaderInfo() const
		{
			return m_gBuffShaderInfo;
		}

		std::set<GeMaterial> MaterialManager::GetMaterials() const
		{
			std::set<GeMaterial> materials;
			
			for (auto & m : m_materialStorages)
			{
				materials.emplace(m.Material);
			}

			return materials;
		}

		std::map<int, IShaderInput *> MaterialManager::GetGbufferInputs(MaterialInstance * materialInstance)
		{
			std::map<int, IShaderInput *> gbufferShaderInputs;


			for (auto inputInfo : m_gBuffShaderInfo.Inputs)
			{
				for (auto materialInput : materialInstance->GetMaterialInputs())
				{
					if (materialInput.ShaderInputInfo->Name == inputInfo->Name)
					{
						gbufferShaderInputs.emplace(inputInfo->Binding, materialInput.ShaderInput);
					}
				}
			}

			return gbufferShaderInputs;
		}

		std::map<int, IShaderInput*> MaterialManager::GetLightingInputs(MaterialInstance * materialInstance)
		{
			std::map<int, IShaderInput *> lightingShaderInputs;
			

			for (auto inputInfo : materialInstance->GetMaterial()->GetMaterilShaderDesc().Inputs)
			{
				for (auto materialInput : materialInstance->GetMaterialInputs())
				{
					if (materialInput.ShaderInputInfo->Name == inputInfo->Name)
					{
						lightingShaderInputs.emplace(inputInfo->Binding, materialInput.ShaderInput);
					}
				}
			}

			return lightingShaderInputs;
		}

		std::map<int, IShaderInput*> MaterialManager::GetEnvMapInputs()
		{
			std::map<int, IShaderInput*> envMapShaderInputs;

			for (auto inputInfo : m_envMapShaderInfo.Inputs)
			{
				if (inputInfo->Type == EShaderInputType::TEXTURE_CUBEMAP)
				{
					ShaderInputTextureCubeMapInfoPtr texCubeMapInputInfo = std::static_pointer_cast<ShaderInputTextureCubeMapInfo>(inputInfo);

					ShaderInputTextureCubeMap * inputCubemap = new ShaderInputTextureCubeMap();
					inputCubemap->SetTexture(texCubeMapInputInfo->FallbackTexture);

					envMapShaderInputs.emplace(inputInfo->Binding, inputCubemap);
				}				
			}

			return envMapShaderInputs;
		}

		const std::set<MaterialInstance*> & MaterialManager::GetMaterialInstances(const GeMaterial & geMaterial) const
		{			
			for (auto & m : m_materialStorages)
			{
				if (m.Material == geMaterial)
				{
					return m.MaterialInstances;
				}
			}
			return std::set<MaterialInstance*>();
		}

		

		MaterialInstance * MaterialManager::GetMeshMaterialInstance(const GeMesh & mesh)
		{
			auto finded = m_meshMaterialRelationship.find(mesh);
			if (finded == m_meshMaterialRelationship.end())
			{
				return nullptr;
			}

			return m_meshMaterialRelationship[mesh];
		}

		
	}
}