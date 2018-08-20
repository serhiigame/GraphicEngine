#include "SceneIO.h"

#include <fstream>
#include <string>
#include <algorithm>

#include <set>
#include <vector>

#include "../GAPI/Math.h"


namespace engine
{
	namespace utility
	{
		
		typedef std::vector<Vec3f> Vec3fArray;

		class MeshIOBase {
		public:
			virtual const float * Load(const std::string & filename, std::vector<graphic::RawMeshData> & out_meshDatas) = 0;
		};

		class MeshObjIOBase : public MeshIOBase {
		public:
			virtual const float * Load(const std::string & filename, std::vector<graphic::RawMeshData> & out_meshDatas)
			{
				graphic::RawMeshData meshData;

				FILE* file = fopen(filename.c_str(), "r");

				if (file == 0)
				{
					// LOG
					return false;
				}

				char* token;
				char currentName[512] = "";
				char charLine[1024 * 4] = { 0 };

				while (fgets(charLine, 1024 * 4, file))
				{
					token = strtok(charLine, " \t\n\r");

					if (token == 0) continue;

					if (strcmp(token, "v") == 0)
					{
						Vec3f position;
						char * token = strtok(NULL, " \t\n\r");
						position[0] = (float)atof(token);
						token = strtok(NULL, " \t\n\r");
						position[1] = (float)atof(token);
						token = strtok(NULL, " \t\n\r");
						position[2] = (float)atof(token);

						meshData.Positions.push_back(position);
					}
					else if (strcmp(token, "vn") == 0)
					{
						Vec3f normal;

						char * token = strtok(NULL, " \t\n\r");
						normal[0] = (float)atof(token);
						token = strtok(NULL, " \t\n\r");
						normal[1] = (float)atof(token);
						token = strtok(NULL, " \t\n\r");
						normal[2] = (float)atof(token);

						meshData.Normals.push_back(normal);
					}
					else if (strcmp(token, "vt") == 0)
					{
						Vec2f uv;

						char * token = strtok(NULL, " \t\n\r");
						uv[0] = (float)atof(token);
						token = strtok(NULL, " \t\n\r");
						uv[1] = (float)atof(token);

						meshData.Uv.push_back(uv);
					}

					else if (strcmp(token, "f") == 0)
					{
						std::vector<unsigned int> indexes;
						indexes.reserve(4);

						while ((token = strtok(0, " \t\n\r")) != 0)
						{
							indexes.push_back(atoi(token) - 1);
						}

						std::vector<unsigned int> & meshIndexes = meshData.Indexes;
						for (int i = 0; i < indexes.size() - 2; ++i)
						{
							meshIndexes.push_back(indexes[0]);
							meshIndexes.push_back(indexes[i + 1]);
							meshIndexes.push_back(indexes[i + 2]);
						}
					}
					else
					{
						if (strcmp(token, "g") == 0)
						{
							const char* name = strtok(0, " \t\n\r");
							meshData.Name = name;
							strcpy(currentName, name);
						}
					}
				}

				out_meshDatas.push_back(meshData);
			}
		private:

		};

		static void ereseSymbol(std::string str, char symbol)
		{
			str.erase(std::remove(str.begin(), str.end(), symbol), str.end());
		}
		
		static std::string getTag(std::string str)
		{
			size_t offset = str.find(':');

			if (offset == std::string::npos)
			{
				return std::string();
			}

			return std::string(str.begin(), str.begin() + offset);
		}

		SceneIo::SceneIo()
		{

		}

		SceneIo::~SceneIo()
		{
		}

		SceneIo & SceneIo::Get()
		{
			static SceneIo imageIo;
			return imageIo;
		}

		bool SceneIo::LoadMesh(const std::string & filename, std::vector<graphic::RawMeshData> & out_meshDatas)
		{
			MeshIOBase * meshIo = nullptr;
			if (true/*check type*/)
			{
				meshIo = new MeshObjIOBase;
			}
			else
			{
				return false;
			}

			if (meshIo)
			{
				meshIo->Load(filename, out_meshDatas);
				delete meshIo;
				meshIo = nullptr;
			}
			else
			{
				return false;
			}

			return true;
		}
	}
}