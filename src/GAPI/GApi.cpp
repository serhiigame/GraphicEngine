#include "GApi.h"

#include <algorithm>

#include "../LLR/Llr.h"
#include "../GU/ImageIO.h"
#include "../GU/SceneIO.h"

#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Texture2d.h"
#include "TextureCubeMap.h"
#include "Shader.h"
#include "ShaderInputs.h"
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
			constexpr int Uv = 2;
		}

		namespace globalTextureAttachment
		{
			constexpr int Color = 0;
			constexpr int Position = 1;
			constexpr int Normal = 2;
			constexpr int Uv = 3;
		}

		struct RanderStageShaderInputs
		{
			Texture2dBindings Texture2dInputs;
			TextureCubeMapBindings TextureCubeMapInputs;
			ConstantBindings ConstantInputs;
		};

		struct RanderStageShaderOutputs
		{
			Texture2dBindings Texture2dInputs;
		};

		struct RenderStage
		{
			IRenderPass * RenderPass;
			IShader * Shader;

			std::vector<RanderStageShaderInputs> ShaderInputs;
			RanderStageShaderOutputs ShaderOutputs;

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

				//if (m_gBufferShader)
				//{
				//	delete m_gBufferShader;
				//	m_gBufferShader = nullptr;
				//}
				
				//if (m_gBufferRenderPass)
				//{
				//	delete m_gBufferRenderPass;
				//	m_gBufferRenderPass = nullptr;
				//}

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

				/*if (m_diffuseTexture)
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

				if (m_uvTexture)
				{
					delete m_uvTexture;
					m_uvTexture = nullptr;
				}*/


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
				IShader * gShader = m_gBuffRenderStage.Shader;

				gShader->AttachAttribute(mesh->m_pos, globalBufferAttchment::Position, 0, 3, EDataType::FLOAT);
				gShader->AttachAttribute(mesh->m_norm, globalBufferAttchment::Normal, 0, 3, EDataType::FLOAT);
				gShader->AttachAttribute(mesh->m_uv, globalBufferAttchment::Uv, 0, 2, EDataType::FLOAT);

				IBuffer * idx = mesh->m_idx;
				size_t count = idx->GetSize() / sizeof(unsigned int);
				gShader->AttachAttribute(idx, 0, 0, count, EDataType::UNSIGNED_INT);

				gShader->AttachConstant(mesh->m_transform, globalConstAttchment::Transformation);
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
				mesh->m_uv = m_llr->CreateBuffer(sizeof(meshData.Positions[0]) * meshData.Positions.size());
				mesh->m_idx = m_llr->CreateIndexBuffer(sizeof(meshData.Indexes[0]) * meshData.Indexes.size());

				mesh->m_pos->Write(0, sizeof(meshData.Positions[0]) * meshData.Positions.size(), meshData.Positions.data());
				mesh->m_norm->Write(0, sizeof(meshData.Normals[0]) * meshData.Normals.size(), meshData.Normals.data());
				mesh->m_uv->Write(0, sizeof(meshData.Uv[0]) * meshData.Uv.size(), meshData.Uv.data());
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

			bool CreateGbuffer(ShaderDesc & shaderInfo)
			{

				Shader * shader = CreateShader(shaderInfo.vertShaderPath, shaderInfo.fragShaderPath);
				if (!shader->IsValid())
				{
					return false;
				}

				RanderStageShaderInputs shaderInputs;
				RanderStageShaderOutputs shaderOutputs;

				for (auto output : shaderInfo.Outputs)
				{
					ITexture2D * texture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);
					shaderOutputs.Texture2dInputs.emplace(output.Binding, texture);
				}

				m_materialGbufferObject.m_shaderDesc = shaderInfo;

				m_gBuffRenderStage = CreateGbufferRenderStege(shader->m_shader, shaderInputs, shaderOutputs, m_gBufferFb);
			}

			MaterialHandler CreateMaterial(ShaderDesc & shaderInfo)
			{
				//TODO: GenMaterialId
				int materialId = m_materialIdCounter++;

				Shader * shader = CreateShader(shaderInfo.vertShaderPath, shaderInfo.fragShaderPath);

				if (!shader)
				{
					return MaterialHandler();
				}

				MaterialObject materialObject;
				materialObject.SetMaterilId(materialId);

				materialObject.SetMaterialShaderDesc( shaderInfo );
				materialObject.SetMaterialShader( shader );

				RanderStageShaderInputs globalShaderInputs;

				globalShaderInputs.Texture2dInputs = m_gBuffRenderStage.ShaderOutputs.Texture2dInputs;

				RenderStage renderStage = CreateLightRenderStege(shader->m_shader, globalShaderInputs, m_acumuFb);

				m_lightRenderStages.push_back(renderStage);

				m_materialLightingObjects.push_back(materialObject);

				MaterialHandler materialHandler;
				materialHandler.m_materialId = materialId;

				return materialHandler;
			}

			MaterialInstance * CreateMaterialInstance(MaterialHandler & handler)
			{
				int materialId = handler.GetMaterialId();
				
				auto materialLightingObject = std::find_if(m_materialLightingObjects.begin(), m_materialLightingObjects.end(), [materialId](MaterialObject & mo) {return materialId == mo.GetMaterilId(); });

				if (materialLightingObject == m_materialLightingObjects.end())
				{
					return nullptr;
				}
	
				MaterialInstance * materialInstance = new MaterialInstance(&m_materialGbufferObject, &materialLightingObject[0]);

				for (auto inpudDesc : materialLightingObject->GetMaterilShaderDesc().Inputs)
				{
					if (IShaderInput * shaderInput = CreateShaderInput(inpudDesc.Type))
					{
						materialInstance->m_materialLightingInputs[inpudDesc.Binding] = shaderInput;
					}
				}

				for (auto inpudDesc : m_materialGbufferObject.GetMaterilShaderDesc().Inputs)
				{
					if (IShaderInput * shaderInput = CreateShaderInput(inpudDesc.Type))
					{
						materialInstance->m_materialGbufferInputs[inpudDesc.Binding] = shaderInput;
					}
				}

				//TODO: use material instance handlers
				return materialInstance;
			}

			void SetMaterialInstanceParameterF4(MaterialInstance * material, const std::string & paramName, const Vec4f & param)
			{
				IShaderInput * shaderInput = FindShaderInput(material, paramName);

				if (!shaderInput)
				{
					return;
				}

				if (shaderInput->GetShaderInputType() != EShaderInputType::CONSTANT)
				{
					return;
				}

				SetShaderInputConstant(shaderInput, param);
			}

			void SetMaterialInstanceParameterTex2d(MaterialInstance * material, const std::string & paramName, Texture2d * texture)
			{
				IShaderInput * shaderInput = FindShaderInput(material, paramName);

				if (!shaderInput)
				{
					return;
				}

				if (shaderInput->GetShaderInputType() != EShaderInputType::TEXTURE_2D)
				{
					return;
				}

				SetShaderInputTexture2d(shaderInput, texture);
			}

			//void DeleteMaterial(Material * material) {
				//m_resources.erase(material);
			//	delete material;
			//}

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

			Shader * CreateShader(std::string vert, std::string frag)
			{
				Shader * shader = new Shader;
				shader->m_shader = m_llr->CreateShader(vert, frag);
				m_resources.insert(shader);
				return shader;
			}

			void DeleteShader(Shader * shader)
			{
				m_resources.erase(shader);
				delete shader;
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
				IShader * shader = m_gBuffRenderStage.Shader;
				shader->AttachConstant(camera->m_cameraTransforms, globalConstAttchment::Camera);
				

				for (auto materialInstances : m_materialGbufferObject.GetMaterialInstances())
				{					
					for (auto mesh : materialInstances->m_meshes)
					{
						BingMesh(mesh);

						for (auto materialInput : materialInstances->m_materialGbufferInputs)
						{
							int binding = materialInput.first;
							IShaderInput * shaderInput = materialInput.second;

							if (shaderInput->GetShaderInputType() == EShaderInputType::TEXTURE_2D)
							{
								shader->AttachTexture2d(((ShaderInputTexture2d *)shaderInput)->GetTexture()->m_texture, binding);
							}
						}

						IRenderPass * renderPass = m_gBuffRenderStage.RenderPass;
						IFramebuffer * framebuffer = m_gBuffRenderStage.OutputFramebuffer;
						renderPass->Execute(shader, framebuffer);
					}
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
						// TODO: set global constant
						shader->AttachConstant(pointLight->m_pointLight, 1);

						for (auto materialObjectIt : m_materialLightingObjects) {
							for (auto materialInstances : materialObjectIt.GetMaterialInstances())
							{
								for (auto materialInput : materialInstances->m_materialLightingInputs)
								{
									int binding = materialInput.first;
									IShaderInput * shaderInput = materialInput.second;

									if (shaderInput->GetShaderInputType() == EShaderInputType::CONSTANT)
									{
										shader->AttachConstant(((ShaderInputConstant *)shaderInput)->m_constant, binding);
									}
									else if (shaderInput->GetShaderInputType() == EShaderInputType::TEXTURE_2D)
									{
										shader->AttachTexture2d(((ShaderInputTexture2d *)shaderInput)->GetTexture()->m_texture, binding);
									}
								}

								renderPass->Execute(shader, acumulator);
							}
						}
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
				m_gBufferFb = m_llr->CreateFramebuffer(w, h);
				m_renderbuffer = m_llr->CreateRenderbuffer(w, h);

				m_gBufferFb->SetRenderbuffer(m_renderbuffer);
			}

			void InitLightRenderState() {
				m_acumuFb = m_llr->CreateFramebuffer(w, h);

				m_acumTexture = m_llr->CreateTexture2d(w, h, ETextureFormat::RGBAf, EDataType::FLOAT);

				//TODO: use ShaderOutput for m_acumTexture
				m_acumuFb->AttachTextures2d(Texture2dBindings({ { 0, m_acumTexture } }));
				
			}

			void InitPostEffectsRenderState() {
				IShader * PEffectShader = m_llr->CreateShader("../res/shaders/BlackWhite.vrt", "../res/shaders/BlackWhite.pxl");

				RanderStageShaderInputs globalShaderInputs;

				// TODO: use global
				globalShaderInputs.Texture2dInputs[0] = m_acumTexture;

				RenderStage renderStage = CreatePEffectRenderStage(PEffectShader
					, globalShaderInputs
					, nullptr);

				m_pEffectRenderStages.push_back(renderStage);
			}


			RenderStage CreateGbufferRenderStege(IShader * shader
				, const RanderStageShaderInputs & shaderInputs
				, const RanderStageShaderOutputs & shaderOutputs
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
				renderStage.OutputFramebuffer = output;
				renderStage.RenderPass = m_llr->CreateRenderPass();


				//for (auto texture2dInput : shaderInputs.Texture2dInputs)
				//{
				//	const uint32_t textureLocation = texture2dInput.first;
				//	const ITexture2D * texture = texture2dInput.second;

				//	renderStage.Shader->AttachTexture2d(texture, textureLocation);
				//}

				if(!shaderOutputs.Texture2dInputs.empty())
				{
					renderStage.ShaderOutputs.Texture2dInputs = shaderOutputs.Texture2dInputs;
					output->AttachTextures2d(shaderOutputs.Texture2dInputs);
				}

				IBuffer * rectIndexBuffer = GetRectIndexBuffer();
				uint32_t rectIndexCount = GetRectIndexCount();
				EDataType rectIndexType = GetRectIndexType();
				renderStage.Shader->AttachAttribute(rectIndexBuffer, 0, 0, rectIndexCount, rectIndexType);

				return renderStage;
			}

			RenderStage CreateLightRenderStege(IShader * shader
				, const RanderStageShaderInputs & globalShaderInputs
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
				renderStage.OutputFramebuffer = output;
				renderStage.RenderPass = m_llr->CreateRenderPass();


				for (auto texture2dInput : globalShaderInputs.Texture2dInputs)
				{
					const uint32_t textureLocation = texture2dInput.first;
					const ITexture2D * texture = texture2dInput.second;

					renderStage.Shader->AttachTexture2d(texture, textureLocation);
				}

				for (auto textureCubeMapInput : globalShaderInputs.TextureCubeMapInputs)
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

			RenderStage CreatePEffectRenderStage(IShader * shader, const RanderStageShaderInputs & inputs, IFramebuffer * output)
			{
				RenderStage renderStage;

				renderStage.Shader = shader;
				renderStage.RenderPass = m_llr->CreateRenderPass();

				for (auto textureInput : inputs.Texture2dInputs)
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

			template<class T>
			IShaderInput * CreateShaderInputConstant()
			{
				IConstant * constant = m_llr->CreateConatant(sizeof(T));
				ShaderInputConstant * shaderInput = new ShaderInputConstant;

				shaderInput->m_constant = constant;

				return shaderInput;
			}

			IShaderInput * CreateShaderInputTexture2d()
			{
				// TODO create via factory
				ShaderInputTexture2d * shaderInput = new ShaderInputTexture2d;
				return shaderInput;
			}

			IShaderInput * CreateShaderInput(const EMaterialInputType type)
			{
				
				switch (type)
				{
				case EMaterialInputType::VEC4F:
					return CreateShaderInputConstant<Vec4f>();

				case EMaterialInputType::TEXTURE:
					return CreateShaderInputTexture2d();

				default:
					break;
				}

				return nullptr;
			}


			//XXX: fix me
			IShaderInput * FindShaderInput(MaterialInstance * material, const std::string & paramName)
			{
				{
					const std::vector<ShaderInputDesc> & descriptions = material->GetMaterialLightingObject()->GetMaterilShaderDesc().Inputs;

					auto findedMatDesc = std::find_if(descriptions.begin(), descriptions.end(), [&paramName](const ShaderInputDesc & decs)->bool {return decs.Name == paramName; });

					if (findedMatDesc != descriptions.end())
					{
						auto materialInputs = material->m_materialLightingInputs;
						auto findedShaderInput = materialInputs.find(findedMatDesc->Binding);

						if (findedShaderInput != materialInputs.end())
						{
							return findedShaderInput->second;
						}
					}
				}
				{
					const std::vector<ShaderInputDesc> & descriptions = material->GetMaterialGbufferObject()->GetMaterilShaderDesc().Inputs;

					auto findedMatDesc = std::find_if(descriptions.begin(), descriptions.end(), [&paramName](const ShaderInputDesc & decs)->bool {return decs.Name == paramName; });

					if (findedMatDesc != descriptions.end())
					{
						auto materialInputs = material->m_materialGbufferInputs;
						auto findedShaderInput = materialInputs.find(findedMatDesc->Binding);

						if (findedShaderInput != materialInputs.end())
						{
							return findedShaderInput->second;
						}
					}
				}
				
			}

			template<class T>
			void SetShaderInputConstant(IShaderInput * shaderInput, const T & data)
			{
				if (!shaderInput)
				{
					return;
				}

				if (shaderInput->GetShaderInputType() != EShaderInputType::CONSTANT)
				{
					return;
				}

				ShaderInputConstant * constant = ((ShaderInputConstant *)shaderInput);
				constant->m_constant->Write(0, sizeof(T), &data);
			}

			void SetShaderInputTexture2d(IShaderInput * shaderInput, Texture2d * texture)
			{
				if (!shaderInput)
				{
					return;
				}

				if (shaderInput->GetShaderInputType() != EShaderInputType::TEXTURE_2D)
				{
					return;
				}

				ShaderInputTexture2d * texture2d = ((ShaderInputTexture2d *)shaderInput);
				texture2d->m_texture = texture;
			}

			void DeleteShaderInput(IShaderInput * shaderInput)
			{
				delete shaderInput;
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

			IFramebuffer * m_gBufferFb = nullptr;
			IFramebuffer * m_acumuFb = nullptr;

			IRenderbuffer * m_renderbuffer = nullptr;

			IBuffer * m_rectIndexBuffer = nullptr;

			ITexture2D * m_acumTexture = nullptr;

			RenderStage m_gBuffRenderStage;
			std::vector<RenderStage> m_lightRenderStages;
			std::vector<RenderStage> m_pEffectRenderStages;

			MaterialObject m_materialGbufferObject;
			std::vector<MaterialObject> m_materialLightingObjects;

			int m_materialIdCounter = 1;
			int m_materialInstanceIdCounter = 1;


			std::set<IResource *> m_resources;
			//std::map<int, MaterialObject> m_materialObjects; // materialId / materialObject
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
		bool GApi::CreateGbuffer(ShaderDesc & shaderDesc)
		{
			return m_impl->CreateGbuffer(shaderDesc);
		}

		MaterialHandler GApi::CreateMaterial(ShaderDesc & shaderDesc)
		{
			return m_impl->CreateMaterial(shaderDesc);
		}

		MaterialInstance * GApi::CreateMaterialInstance(MaterialHandler & handler)
		{
			return m_impl->CreateMaterialInstance(handler);
		}

		void GApi::SetMaterialParameterF4(MaterialInstance * material, const std::string & paramName, const Vec4f & param)
		{
			m_impl->SetMaterialInstanceParameterF4(material, paramName, param);
		}

		void GApi::SetMaterialParameterTex2d(MaterialInstance * material, const std::string & paramName, Texture2d * texture)
		{
			m_impl->SetMaterialInstanceParameterTex2d(material, paramName, texture);
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

		void GApi::SetMeshMaterialInstance(Mesh * mesh, MaterialInstance * material)
		{
			material->AddMesh(mesh);
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
