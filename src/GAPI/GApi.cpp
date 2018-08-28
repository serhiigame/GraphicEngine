#include "GApi.h"

#include "../LLR/Llr.h"
#include "../GU/ImageIO.h"
#include "../GU/SceneIO.h"

#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture2d.h"
#include "TextureCubeMap.h"
#include "PointLight.h"

namespace engine
{
	namespace graphic
	{
		const size_t w = 640;
		const size_t h = 480;

		namespace globalConstAttchment
		{
			constexpr int Transformation = 0;
			constexpr int Camera = 8;
		}

		namespace globalBufferAttchment
		{
			constexpr int Position = 0;
			constexpr int Normal = 1;
		}

		namespace globalTextureAttachment
		{
			constexpr int Color = 0;
			constexpr int Position = 1;
			constexpr int Normal = 2;
		}

		struct RenderStage
		{
			IRenderPass * RenderPass;
			IShader * Shader;
			Texture2dBindings Texture2dInputs;
			TextureCubeMapBindings TextureCubeMapInputs;
			ConstantBindings ConstantInputs;
			// BufferInputs

			IFramebuffer * OutputFramebuffer;

		};

		class GApiImpl
		{
		public:
			void Init()
			{
				m_llr = Llr::CreateLlr(ELlrType::OPEN_GL);
				
				this->InitGBufferRenderState();
				this->InitLightRenderState();
				this->InitPostEffectsRenderState();
			}

			void Deinit()
			{
				for (IResource * resource : m_resources)
				{
					delete resource;
				}

				if (m_gBufferShader)
				{
					delete m_gBufferShader;
					m_gBufferShader = nullptr;
				}
				
				if (m_gBufferRenderPass)
				{
					delete m_gBufferRenderPass;
					m_gBufferRenderPass = nullptr;
				}

				if (m_gBufferFb)
				{
					delete m_gBufferFb;
					m_gBufferFb = nullptr;
				}

				if (m_acumuFb)
				{
					delete m_acumuFb;
					m_acumuFb = nullptr;
				}

				if (m_diffuseTexture)
				{
					delete m_diffuseTexture;
					m_diffuseTexture = nullptr;
				}

				if (m_positionTexture)
				{
					delete m_positionTexture;
					m_positionTexture = nullptr;
				}


				if (m_normalTexture)
				{
					delete m_normalTexture;
					m_normalTexture = nullptr;
				}


				if (m_rectIndexBuffer)
				{
					delete m_rectIndexBuffer;
					m_rectIndexBuffer = nullptr;
				}


				if (m_acumTexture)
				{
					delete m_acumTexture;
					m_acumTexture = nullptr;
				}

				Llr::DeleteLlr(m_llr);
			}

			void BingMesh(Mesh * mesh) {
				m_gBufferShader->AttachAttribute(mesh->m_pos, globalBufferAttchment::Position, 0, 3, EDataType::FLOAT);
				m_gBufferShader->AttachAttribute(mesh->m_norm, globalBufferAttchment::Normal, 0, 3, EDataType::FLOAT);
				//m_gBufferShader->AttachConstant(mesh->m_transform, 0);

				IBuffer * idx = mesh->m_idx;
				size_t count = idx->GetSize() / sizeof(unsigned int);
				m_gBufferShader->AttachAttribute(idx, 0, 0, count, EDataType::UNSIGNED_INT);

				m_gBufferShader->AttachConstant(mesh->m_transform, globalConstAttchment::Transformation);
			}

			Scene * CreareScene()
			{
				Scene * scene = new Scene;
				m_resources.insert(scene);

				return scene;
			}

			void DeleteScene(Scene * scene)
			{
				m_resources.erase(scene);
				delete scene;
			}

			Mesh * CreateMesh(const RawMeshData & meshData)
			{
				Mesh * mesh = new Mesh;
				m_resources.insert(mesh);

				mesh->m_pos = m_llr->CreateBuffer(sizeof(meshData.Positions[0]) * meshData.Positions.size());
				mesh->m_norm = m_llr->CreateBuffer(sizeof(meshData.Normals[0]) * meshData.Normals.size());
				mesh->m_idx = m_llr->CreateIndexBuffer(sizeof(meshData.Indexes[0]) * meshData.Indexes.size());

				mesh->m_pos->Write(0, sizeof(meshData.Positions[0]) * meshData.Positions.size(), meshData.Positions.data());
				mesh->m_norm->Write(0, sizeof(meshData.Normals[0]) * meshData.Normals.size(), meshData.Normals.data());
				mesh->m_idx->Write(0, sizeof(meshData.Indexes[0]) * meshData.Indexes.size(), meshData.Indexes.data());

				Mat4f transformMat;
				transformMat.MakeIdentity();
				mesh->m_transform = m_llr->CreateConatant(sizeof(Mat4f));
				mesh->m_transform->Write(0, sizeof(transformMat), &transformMat);

				return mesh;
			}

			void DeleteMesh(Mesh * mesh)
			{
				m_resources.erase(mesh);
				delete mesh;
			}

			Camera * CreateCamera()
			{
				Camera * camera = new Camera;
				m_resources.insert(camera);

				Mat4f viewMatrix;
				Mat4f projectionMatrix;

				viewMatrix.MakeIdentity();
				projectionMatrix.MakeIdentity();

				camera->m_cameraTransforms = m_llr->CreateConatant(sizeof(viewMatrix) + sizeof(projectionMatrix));

				camera->m_cameraTransforms->Write(0, sizeof(viewMatrix), &viewMatrix[0]);
				camera->m_cameraTransforms->Write(sizeof(projectionMatrix), sizeof(projectionMatrix), &projectionMatrix[0]);

				return camera;
			}

			void DeleteCamera(Camera * camera)
			{
				m_resources.erase(camera);
				delete camera;
			}

			Texture2d * CreateTexture2d(size_t w, size_t h)
			{
				Texture2d * texture2d = new Texture2d;
				m_resources.insert(texture2d);
				texture2d->m_texture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);
				return texture2d;
			}

			void DeleteTexture2d(Texture2d * texture2d)
			{
				m_resources.erase(texture2d);
				delete texture2d;
			}

			TextureCubeMap * CreateTextureCubeMap(size_t size)
			{
				TextureCubeMap * textureCubeMap = new TextureCubeMap;
				m_resources.insert(textureCubeMap);
				textureCubeMap->m_texture = m_llr->CreateTextureCubeMap(size, size, ETextureFormat::RGBAf, EDataType::FLOAT);
				textureCubeMap->m_size = size;
				return textureCubeMap;
			}

			void DeleteTextureCubeMap(TextureCubeMap * textureCubeMap)
			{
				m_resources.erase(textureCubeMap);
				delete textureCubeMap;
			}

			PointLight * CreatePointLight()
			{
				PointLight * pointLight = new PointLight;
				m_resources.insert(pointLight);

				Vec3f lightPosition({ 0.f, 0.f, 0.f });

				pointLight->m_pointLight = m_llr->CreateConatant(sizeof(lightPosition));
				pointLight->m_pointLight->Write(0, sizeof(lightPosition), &lightPosition[0]);

				return pointLight;
			}

			void DeletePointLight(PointLight * pointLight)
			{
				m_resources.erase(pointLight);
				delete pointLight;
			}

			void ClearFremebuffers()
			{
				m_gBufferFb->Cleare();
				m_acumuFb->Cleare();
			}

			void RenderGeometry(const std::vector<Mesh *> meshes, const Camera * camera)
			{
				m_gBufferShader->AttachConstant(camera->m_cameraTransforms, globalConstAttchment::Camera);
				
				for (Mesh * mesh : meshes)
				{
					BingMesh(mesh);
					RenderGBuffer();
				}
			}

			void RenderLights(const std::vector< PointLight *> & lights, TextureCubeMap * skybox = nullptr)
			{
				for (auto lightRenderStage : m_lightRenderStages)
				{
					IRenderPass * renderPass = lightRenderStage.RenderPass;
					IShader * shader = lightRenderStage.Shader;
					IFramebuffer * acumulator = lightRenderStage.OutputFramebuffer;

					if (skybox)
					{
						shader->AttachTextureCubeMap(skybox->m_texture, 10);
					}

					for (PointLight * pointLight : lights)
					{
						shader->AttachConstant(pointLight->m_pointLight, 1);
						renderPass->Execute(shader, acumulator);
					}
				}
			}

			void RenderPEffects()
			{
				for (auto pEffectRenderStage : m_pEffectRenderStages)
				{
					IRenderPass * renderPass = pEffectRenderStage.RenderPass;
					IShader * shader = pEffectRenderStage.Shader;
					renderPass->Execute(shader);
				}
			}

			Llr * GetLlr() { return m_llr; }


		private:

			void InitGBufferRenderState()
			{
				
				m_gBufferShader = m_llr->CreateShader("../res/shaders/GBuffer.vrt", "../res/shaders/GBuffer.pxl");

				m_gBufferFb = m_llr->CreateFramebuffer(w, h);

				m_diffuseTexture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);
				m_positionTexture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);
				m_normalTexture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);

				m_gBufferFb->AttachTextures2d(Texture2dBindings({
					{ globalTextureAttachment::Color, m_diffuseTexture }
					,{ globalTextureAttachment::Position, m_positionTexture }
					,{ globalTextureAttachment::Normal, m_normalTexture }
				}));

				m_gBufferRenderPass = m_llr->CreateRenderPass();
			}

			void InitLightRenderState() {
				IShader * diffuseShader = m_llr->CreateShader("../res/shaders/Lambert.vrt", "../res/shaders/Lambert.pxl");

				m_acumuFb = m_llr->CreateFramebuffer(w, h);

				m_acumTexture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);

				m_acumuFb->AttachTextures2d(Texture2dBindings({ { 0, m_acumTexture } }));

				RenderStage renderStage = CreateLightRenderStege(diffuseShader
					, Texture2dBindings({
						{ globalTextureAttachment::Color, m_diffuseTexture }
						,{ globalTextureAttachment::Position, m_positionTexture }
						,{ globalTextureAttachment::Normal, m_normalTexture } })
					, TextureCubeMapBindings()
					, m_acumuFb);

				m_lightRenderStages.push_back(renderStage);
			}

			void InitPostEffectsRenderState() {
				IShader * PEffectShader = m_llr->CreateShader("../res/shaders/BlackWhite.vrt", "../res/shaders/BlackWhite.pxl");

				RenderStage renderStage = CreatePEffectRenderStage(PEffectShader
					, Texture2dBindings({{ 0, m_acumTexture } })
					,					nullptr);

				m_pEffectRenderStages.push_back(renderStage);
			}

			RenderStage CreateLightRenderStege(IShader * shader
				, const Texture2dBindings & texture2dInputs
				, const TextureCubeMapBindings & textureCubeMapInputs
				, IFramebuffer * output)
			{
				if (!m_rectIndexBuffer)
				{
					const GLuint indexes[]{ 0,1,2, 3,2,0 };
					m_rectIndexBuffer = m_llr->CreateIndexBuffer(sizeof(indexes));
					m_rectIndexBuffer->Write(0, sizeof(indexes), indexes);
				}

				RenderStage renderStage;

				renderStage.Shader = shader;
				renderStage.Texture2dInputs = texture2dInputs;
				renderStage.TextureCubeMapInputs = textureCubeMapInputs;
				renderStage.OutputFramebuffer = output;
				renderStage.RenderPass = m_llr->CreateRenderPass();

				for (auto texture2dInput : renderStage.Texture2dInputs)
				{
					const uint32_t textureLocation = texture2dInput.first;
					const ITexture2D * texture = texture2dInput.second;

					renderStage.Shader->AttachTexture2d(texture, textureLocation);
				}

				for (auto textureCubeMapInput : renderStage.TextureCubeMapInputs)
				{
					const uint32_t textureLocation = textureCubeMapInput.first;
					const ITextureCubeMap * texture = textureCubeMapInput.second;

					renderStage.Shader->AttachTextureCubeMap(texture, textureLocation);
				}

				IBuffer * rectIndexBuffer = GetRectIndexBuffer();
				uint32_t rectIndexCount = GetRectIndexCount();
				EDataType rectIndexType = GetRectIndexType();
				renderStage.Shader->AttachAttribute(rectIndexBuffer, 0, 0, rectIndexCount, rectIndexType);
				
				return renderStage;
			}

			RenderStage CreatePEffectRenderStage(IShader * shader, const Texture2dBindings & inputs, IFramebuffer * output )
			{
				RenderStage renderStage;

				renderStage.Shader = shader;
				renderStage.Texture2dInputs = inputs;
				renderStage.OutputFramebuffer = output;
				renderStage.RenderPass = m_llr->CreateRenderPass();
				
				for ( auto textureInput : renderStage.Texture2dInputs)
				{
					const uint32_t textureLocation = textureInput.first;
					const ITexture2D * texture = textureInput.second;
					
					renderStage.Shader->AttachTexture2d(texture, textureLocation);
				}

				IBuffer * rectIndexBuffer = GetRectIndexBuffer();
				uint32_t rectIndexCount = GetRectIndexCount();
				EDataType rectIndexType = GetRectIndexType();
				renderStage.Shader->AttachAttribute(rectIndexBuffer, 0, 0, rectIndexCount, rectIndexType);

				return renderStage;
			}



			void RenderGBuffer() {
				m_gBufferRenderPass->Execute(m_gBufferShader, m_gBufferFb);
			}

			IBuffer * GetRectIndexBuffer()
			{
				if (!m_rectIndexBuffer)
				{
					const GLuint indexes[]{ 0,1,2, 3,2,0 };
					m_rectIndexBuffer = m_llr->CreateIndexBuffer(sizeof(indexes));
					m_rectIndexBuffer->Write(0, sizeof(indexes), indexes);
				}

				return m_rectIndexBuffer;
			}

			uint32_t GetRectIndexCount() const
			{
				return 6;
			}

			EDataType GetRectIndexType()
			{
				return  EDataType::UNSIGNED_INT;
			}

			Llr * m_llr;

			IShader * m_gBufferShader = nullptr;
			IRenderPass * m_gBufferRenderPass = nullptr;

			IFramebuffer * m_gBufferFb = nullptr;
			IFramebuffer * m_acumuFb = nullptr;

			ITexture2D * m_diffuseTexture = nullptr;
			ITexture2D * m_positionTexture = nullptr;
			ITexture2D * m_normalTexture = nullptr;

			IBuffer * m_rectIndexBuffer = nullptr;

			ITexture2D * m_acumTexture = nullptr;

			std::vector<RenderStage> m_lightRenderStages;
			std::vector<RenderStage> m_pEffectRenderStages;

			std::set<IResource *> m_resources;
		};
		
		GApi::GApi()
		{
		}
		void GApi::Init()
		{
			m_impl = new GApiImpl;
			m_impl->Init();
		}
		void GApi::Deinit()
		{
			m_impl->Deinit();
			delete m_impl;
		}
		Scene * GApi::CreateScene()
		{
			return m_impl->CreareScene();
		}

		void GApi::DeleteScene(Scene * scene)
		{
			m_impl->DeleteScene(scene);
		}

		Mesh * GApi::CreateMesh(const RawMeshData & meshData)
		{
			return m_impl->CreateMesh(meshData);
		}

		void GApi::DeleteMesh(Mesh * mesh)
		{
			m_impl->DeleteMesh(mesh);
		}

		Camera * GApi::CreateCamera()
		{
			return m_impl->CreateCamera();
		}

		void GApi::DeleteCamera(Camera * camera)
		{
			m_impl->DeleteCamera(camera);
		}

		Texture2d * GApi::CreateTexture2d(const size_t width, const size_t heigth)
		{
			return m_impl->CreateTexture2d(width, heigth);
		}

		void GApi::DeleteTexture2d(Texture2d * texture2d)
		{
			m_impl->DeleteTexture2d(texture2d);
		}

		TextureCubeMap * GApi::CreateTextureCubeMap(size_t size)
		{
			return m_impl->CreateTextureCubeMap(size);
		}

		void GApi::DeleteTextureCubeMap(TextureCubeMap * textureCubeMap)
		{
			m_impl->DeleteTextureCubeMap(textureCubeMap);
		}

		void GApi::SetCameraView(Camera * camera, const Mat4f & view)
		{
			Llr * llr = m_impl->GetLlr();
			camera->m_cameraTransforms->Write(0, sizeof(Mat4f), &view[0]);
		}

		void GApi::SetCameraProjection(Camera * camera, const Mat4f & projection)
		{
			camera->m_cameraTransforms->Write(sizeof(Mat4f), sizeof(Mat4f), &projection[0]);
		}

		PointLight * GApi::CreatePointLight()
		{
			return m_impl->CreatePointLight();
		}

		void GApi::DeletePointLight(PointLight * pointLight)
		{
			m_impl->DeletePointLight(pointLight);
		}

		void GApi::SetMeshTransform(Mesh * mesh, const Mat4f & transform)
		{
			mesh->m_transform->Write(0, sizeof(Mat4f), &transform[0]);
		}

		void GApi::SetPointLightPosition(PointLight * light, const Vec3f & position)
		{
			light->m_pointLight->Write(0, sizeof(Vec3f), &position[0]);
		}

		void GApi::SetPointLightIntensity(PointLight * light, const float intensity)
		{
			light->m_pointLight->Write(sizeof(Vec3f), sizeof(float), &intensity);
		}

		void GApi::SetSceneCamera(Scene * scene, Camera * camera)
		{
			scene->SetCamera(camera);
		}

		void GApi::SetSceneSkybox(Scene * scene, TextureCubeMap * skybox)
		{
			scene->SetSkybox(skybox);
		}

		void GApi::AddSceneMesh(Scene * scene, Mesh * mesh)
		{
			scene->AddMesh(mesh);
		}

		void GApi::AddScenePointLight(Scene * scene, PointLight * pointLight)
		{
			scene->AddPointLight(pointLight);
		}

		ERenderStatus GApi::Render(Scene * scene, ERenderMode mode)
		{
			m_impl->ClearFremebuffers();

			m_impl->RenderGeometry(scene->GetMeshes(), scene->GetCamera());

			m_impl->RenderLights(scene->GetPointLight(), scene->GetSkybox());
			
			m_impl->RenderPEffects();

			return ERenderStatus::SUCCESS;
		}
	}

}
