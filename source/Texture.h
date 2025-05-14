#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
class Texture {
	const char* FILE_PATH;
	bool hasCreated = false;
	GLuint texture;
public:
  Texture(const char* filePath):FILE_PATH(filePath){}
  std::string create() {
	  std::string last;
	  if (!hasCreated)
	  {
		  stbi_set_flip_vertically_on_load(true);
		  glGenTextures(1, &texture);
		  glBindTexture(GL_TEXTURE_2D, texture);

		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		  
		  int width, height, nrChannels;
		  unsigned char* data = stbi_load(FILE_PATH, &width, &height,
			  &nrChannels, 0);
		  if (data)
		  {
			  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
				  GL_UNSIGNED_BYTE, data);
			  glGenerateMipmap(GL_TEXTURE_2D);
			  stbi_image_free(data);
			  last += "successfully created texture with file -: ";
			  last += FILE_PATH;
		  }
		  else
		  {
			  last += "[TEXTURE]: cannot create texture for -> ";
			  last += FILE_PATH;
		  }
	  }
	  else {
		  last += "already created texture -: ";
		  last += FILE_PATH;
	  }
	  return last;
  }
  void bindDefault() {
	  bind(GL_TEXTURE0);
  }
  void bind(int slot) {
	  glActiveTexture(slot);
	  glBindTexture(GL_TEXTURE_2D, texture);
  }
  void unbind() {
	  glBindTexture(GL_TEXTURE_2D,NULL);
  }
};