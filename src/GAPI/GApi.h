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

		class ShaderDesc;
		class MaterialHandler;
		class MaterialInstance;
		
		class TextureCubeMap;

		class Shader;
		class IShaderInput;

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
			GeCamera CreateCamera();

			GAPI_EXPORT
			void DeleteCamera(GeCamera camera);

			GAPI_EXPORT
			GeTexture2d CreateTexture2d(const size_t width, const size_t heigth);

			GAPI_EXPORT
			void DeleteTexture2d(GeTexture2d texture2d);

			GAPI_EXPORT
			void Texture2dWriteImage(GeTexture2d texture2d, int xOffset, int yOffset, int w, int h, const void * data);

			GAPI_EXPORT
			TextureCubeMap * CreateTextureCubeMap(const size_t size);

			GAPI_EXPORT
			void DeleteTextureCubeMap(TextureCubeMap * texture2d);

			GAPI_EXPORT
			PointLight * CreatePointLight();

			GAPI_EXPORT
			void DeletePointLight(PointLight * pointLight);

			GAPI_EXPORT
			GeMesh CreateMesh(const RawMeshData & meshData);

			GAPI_EXPORT
			void DeleteMesh(GeMesh mesh);

			GAPI_EXPORT
			bool CreateGbuffer(ShaderDesc & shaderDesc);

			GAPI_EXPORT
			MaterialHandler CreateMaterial(ShaderDesc & shaderDesc);

			GAPI_EXPORT
			MaterialInstance * CreateMaterialInstance(MaterialHandler & handler);

			GAPI_EXPORT
			void SetMaterialParameterF4(MaterialInstance * material, const std::string & paramName, const Vec4f & param);

			GAPI_EXPORT
			void SetMaterialParameterTex2d(MaterialInstance * material, const std::string & paramName, GeTexture2d texture);

			GAPI_EXPORT
			void SetCameraView(GeCamera camera, const Mat4f & view);

			GAPI_EXPORT
			void SetCameraProjection(GeCamera camera, const Mat4f & projection);

			GAPI_EXPORT
			void SetMeshTransform(GeMesh mesh, const Mat4f & transform);

			GAPI_EXPORT
			void SetMeshMaterialInstance(GeMesh mesh, MaterialInstance * material);

			GAPI_EXPORT
			void SetPointLightPosition(PointLight * light, const Vec3f & position);

			GAPI_EXPORT
			void SetPointLightIntensity(PointLight * light, const float intensity);

			GAPI_EXPORT
			void SetSceneCamera(Scene * scene, GeCamera camera);

			GAPI_EXPORT
			void SetSceneSkybox(Scene * scene, TextureCubeMap * skybox);

			GAPI_EXPORT
			void AddSceneMesh(Scene * scene, GeMesh mesh);

			GAPI_EXPORT
			void AddScenePointLight(Scene * scene, PointLight * mesh);

			GAPI_EXPORT
			ERenderStatus Render(Scene * scene, ERenderMode mode = ERenderMode::DRAW_ALL);

		private:
			GApiImpl * m_impl;
		};
	}
}
