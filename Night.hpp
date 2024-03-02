#ifndef Night_hpp
#define Night_hpp

#include "Shader.hpp"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stdio.h>

namespace gps {
	class Night {
	public:
		Night();
		void Load(std::vector<const GLchar*> cubeMapFaces);
		void Draw(gps::Shader shader, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
		GLuint GetTextureId();
	public:
		GLuint nightVAO;
		GLuint nightVBO;
		GLuint cubemapTexture;
		GLuint LoadNightTextures(std::vector<const GLchar*> cubeMapFaces);
		void InitNight();

	};
}

#endif /* Night_hpp */
