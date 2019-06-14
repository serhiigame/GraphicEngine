#pragma once

#include "api.h"

#include "EGApiEnums.h"

#include "IResource.h"

#include <vector>
#include <set>
#include <map>

namespace engine
{
	namespace graphic
	{
		class MaterialInstance;
		class Camera;
		class PointLight;
		class TextureCubeMap;

		class Scene final : public IResource
		{
		public:
			void SetCamera(GeCamera camera);

			const GeCamera GetCamera() const;

			void AddMesh(GeMesh mesh);

			const std::set<GeMesh > & GetMeshes() { return m_meshes; }

			//void AddMaterialInstance(MaterialInstance * materialInstance) { m_materialInstances.push_back(materialInstance); }

			//void SetMeshMaterialInstance(GeMesh mesh, MaterialInstance * materialInstance) { m_materialInstanceMeshRelationship.emplace(materialInstance, mesh); }

			//const std::multimap< MaterialInstance *, GeMesh> GetMeshMaterialRelationship() const { return m_materialInstanceMeshRelationship; }

			void AddPointLight(PointLight * mesh);

			void SetSkybox(TextureCubeMap * cubemap);

			TextureCubeMap * GetSkybox() const { return m_skybox; }

			const std::vector< PointLight *> & GetPointLight() { return m_pointLights; }

		protected:
			std::set<GeMesh> m_meshes;

			//std::vector< MaterialInstance *> m_materialInstances;
			//std::multimap< MaterialInstance *, GeMesh> m_materialInstanceMeshRelationship;

			std::vector< PointLight *> m_pointLights;
			TextureCubeMap * m_skybox = nullptr;
			GeCamera m_camera;
		};
	}
}