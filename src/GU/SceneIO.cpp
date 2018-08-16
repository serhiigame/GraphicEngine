#include "SceneIO.h"

#include <fstream>
#include <string>
#include <algorithm>

#include <set>
#include <vector>




namespace engine
{
	namespace utility
	{
		
		typedef std::vector<gte::Vector3<float>> Vec3fArray;
		typedef gte::Vector3<float> Vec3f;

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

		
		SceneIo & SceneIo::Get()
		{
			static SceneIo imageIo;
			return imageIo;
		}

		const float * SceneIo::LoadScene(const std::string & filePath)
		{
			return nullptr;
		}


		SceneIo::SceneIo()
		{
			
		}

		SceneIo::~SceneIo()
		{
		}
	}
}