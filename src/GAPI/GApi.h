#pragma once

#include "api.h"

#include "EGApiEnums.h"
#include "RawMeshData.h"
#include "Math.h"

#include <string>

namespace engine
{
	namespace graphic
	{
		class GApiImpl;
	
		class Scene;

		class Mesh;
		class Camera;

		class Texture2d;
		class TextureCubeMap;

		class PointLight;
		

		class GApi final
		{
		public:
			GAPI_EXPORT
			GApi();

			GAPI_EXPORT
			void Init();

			GAPI_EXPORT
			void Deinit();

			GAPI_EXPORT
			Scene * CreateScene();

			GAPI_EXPORT
			void DeleteScene(Scene * scene);

			GAPI_EXPORT
			Camera * CreateCamera();

			GAPI_EXPORT
			void DeleteCamera(Camera * camera);

			GAPI_EXPORT
			Texture2d * CreateTexture2d(const size_t width, const size_t heigth);

			GAPI_EXPORT
			void DeleteTexture2d(Texture2d * texture2d);

			GAPI_EXPORT
			TextureCubeMap * CreateTextureCubeMap(const size_t size);

			GAPI_EXPORT
			void DeleteTextureCubeMap(TextureCubeMap * texture2d);

			GAPI_EXPORT
			PointLight * CreatePointLight();

			GAPI_EXPORT
			void DeletePointLight(PointLight * pointLight);

			GAPI_EXPORT
			Mesh * CreateMesh(const RawMeshData & meshData);

			GAPI_EXPORT
			void DeleteMesh(Mesh * mesh);

			GAPI_EXPORT
			void SetCameraView(Camera * camera, const Mat4f & view);

			GAPI_EXPORT
			void SetCameraProjection(Camera * camera, const Mat4f & projection);

			GAPI_EXPORT
			void SetMeshTransform(Mesh * mesh, const Mat4f & transform);

			GAPI_EXPORT
			void SetPointLightPosition(PointLight * light, const Vec3f & position);

			GAPI_EXPORT
			void SetPointLightIntensity(PointLight * light, const float intensity);

			GAPI_EXPORT
			void SetSceneCamera(Scene * scene, Camera * camera);

			GAPI_EXPORT
			void SetSceneSkybox(Scene * scene, TextureCubeMap * skybox);

			GAPI_EXPORT
			void AddSceneMesh(Scene * scene, Mesh * mesh);

			GAPI_EXPORT
			void AddScenePointLight(Scene * scene, PointLight * mesh);

			GAPI_EXPORT
			ERenderStatus Render(Scene * scene, ERenderMode mode = ERenderMode::DRAW_ALL);

		private:
			GApiImpl * m_impl;
		};
	}
}
