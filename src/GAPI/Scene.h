#pragma once

#include "api.h"

#include "IResource.h"

#include <vector>
#include <map>

namespace engine
{
	namespace graphic
	{
		class Mesh;
		class MaterialInstance;
		class Camera;
		class PointLight;
		class TextureCubeMap;

		class Scene final : public IResource
		{
		public:
			void SetCamera(Camera * camera);

			const Camera * GetCamera() const;
			Camera * GetCamera();

			void AddMesh(Mesh * mesh);

			const std::vector< Mesh *> & GetMeshes() { return m_meshes; }

			void AddMaterialInstance(MaterialInstance * materialInstance) { m_materialInstances.push_back(materialInstance); }

			void SetMeshMaterialInstance(Mesh * mesh, MaterialInstance * materialInstance) { m_materialInstanceMeshRelationship.emplace(materialInstance, mesh); }

			const std::multimap< MaterialInstance *, Mesh *> GetMeshMaterialRelationship() const { return m_materialInstanceMeshRelationship; }

			void AddPointLight(PointLight * mesh);

			void SetSkybox(TextureCubeMap * cubemap);

			TextureCubeMap * GetSkybox() const { return m_skybox; }

			const std::vector< PointLight *> & GetPointLight() { return m_pointLights; }

		protected:
			std::vector< Mesh *> m_meshes;

			std::vector< MaterialInstance *> m_materialInstances;
			std::multimap< MaterialInstance *, Mesh *> m_materialInstanceMeshRelationship;

			std::vector< PointLight *> m_pointLights;
			TextureCubeMap * m_skybox = nullptr;
			Camera * m_camera = nullptr;
		};
	}
}