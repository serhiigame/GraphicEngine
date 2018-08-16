#include "ImageIO.h"

#include <set>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb\stb_image_write.h>



namespace engine
{
	namespace utility
	{
		inline std::string retrieveExtention(const std::string& path)
		{
			return std::string(path.begin() + path.find_last_of('.'), path.end());
		}
		
		class ImageIOBase {
		public:
			virtual const float * Load(const std::string & filename, int & o_width, int & o_height) = 0;
			virtual void Write(const std::string & filename, const int width, const int heigh, const float * data) = 0;
		};

		class ImageIOBaseStbi : public ImageIOBase {
		public:

			virtual const float * Load(const std::string & filename, int & o_width, int & o_height) override
			{

				unsigned char* pixels;
				int bpp;

				pixels = stbi_load(filename.c_str(), &o_width, &o_height, &bpp, 0);

				if (pixels == 0)
				{
					//TODO: trace error
					return nullptr;
				}

				const size_t dataSize = o_width * o_height * 4;
				float * ptr = new float[dataSize];
				memset(ptr, 0, dataSize * sizeof(float));


				for (int i = 0; i <  o_width * o_height; ++i)
				{
						if (bpp >= 1)
							ptr[i * 4 + 0] = pixels[i * bpp + 0] / 255.f;
						if (bpp >= 2)
							ptr[i * 4 + 1] = pixels[i * bpp + 1] / 255.f;
						if (bpp >= 3)
							ptr[i * 4 + 2] = pixels[i * bpp + 2] / 255.f;
						if (bpp == 4)
							ptr[i * 4 + 3] = pixels[i * bpp + 3] / 255.f;
				}

				stbi_image_free(pixels);
				return ptr;
			}

			virtual void Write(const std::string & filename, const int width, const int heigh, const float * data) override
			{
				const size_t outBPP = 3;
				const size_t inBPP = 4;
				std::vector<char> convertedData(width * heigh * outBPP);

				
				for (int i = 0; i < width * heigh; ++i)
				{
					convertedData[i * outBPP + 0] = static_cast<char>(data[i * inBPP + 0] * 255.f);
					convertedData[i * outBPP + 1] = static_cast<char>(data[i * inBPP + 1] * 255.f);
					convertedData[i * outBPP + 2] = static_cast<char>(data[i * inBPP + 2] * 255.f);
				}
				
				std::string extention = retrieveExtention(filename);
				if (extention == ".png")
				{
					stbi_write_png(filename.c_str(), width, heigh, outBPP, convertedData.data(), width * outBPP);
				}
			}
		};

		ImageIo & ImageIo::Get()
		{
			static ImageIo imageIo;
			return imageIo;
		}

		ImageIo::ImageIo()
		{
			
			ImageIOBase * stb = new ImageIOBaseStbi;
			m_imgIoImpl[".png"] = stb;
		}

		ImageIo::~ImageIo()
		{
			ImageIOBase * stb = m_imgIoImpl[".png"];
			delete stb;
		}

		const float * ImageIo::Load(const std::string & filePath, int & o_width, int & o_height)
		{
			std::string extention = retrieveExtention(filePath);
			
			auto finded = m_imgIoImpl.find(extention);
			if (finded != m_imgIoImpl.end())
			{
				return finded->second->Load(filePath, o_width, o_height);
			}

			return nullptr;
		}

		void ImageIo::Write(const std::string & filename, const int width, const int heigh, const float * data)
		{
			std::string extention = retrieveExtention(filename);

			auto finded = m_imgIoImpl.find(extention);
			if (finded != m_imgIoImpl.end())
			{
				return finded->second->Write(filename, width, heigh, data);
			}
		}

	}
}