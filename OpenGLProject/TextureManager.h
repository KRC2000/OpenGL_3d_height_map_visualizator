#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "stb_image.h"
#include "lodepng.h"

namespace fwork
{
	class TextureManager
	{
		// name == path
		struct Texture
		{
			unsigned int id;
			std::string name;
		};

		std::vector<Texture> textures;

	public:
		TextureManager()
		{}

		unsigned int loadTexture(std::string path)
		{
			Texture t;
			t.name = path;

			glGenTextures(1, &t.id);
			glBindTexture(GL_TEXTURE_2D, t.id);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			unsigned int error;
			unsigned char* data = 0;
			unsigned int width, height;

			error = lodepng_decode32_file(&data, &width, &height, path.c_str());
			if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "TextureManager::Failed to load texture image" << std::endl;
			}

			free(data);


			textures.push_back(t);



			return t.id;
		}

		unsigned int getTexture(std::string name)
		{
			for (Texture& t : textures)
			{
				if (t.name == name) return t.id;
			}
			std::cout << "TextureManager::Can't get texture with such name!" << std::endl;
			return 0;
		}
	};
}

