#ifndef __Model3dNode_H__
#define __Model3dNode_H__

#include "OpenglNode.h"
#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace opengl_framewordk
{
	struct Vertex3d
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture3d
	{
		GLuint id;
		std::string type;
	};

	class Mesh3d
	{
	public:
		std::vector<Vertex3d> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture3d> textures;
		Mesh3d(std::vector<Vertex3d> vertices, std::vector<GLuint> indices, std::vector<Texture3d> texture)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			this->setupMesh();
		}
		void draw(ShaderProgram *shader);

	private:
		GLuint VAO, VBO, EBO;
		void setupMesh();
	};

	class Model3d
	{
	public:
		Model3d(GLchar* path)
		{
			this->loadModel(path);
		}

		void draw(ShaderProgram *shader)
		{
			for (GLuint i = 0; i < this->meshes.size(); i++)
				this->meshes[i].draw(shader);
		}

	private:
		std::vector<Mesh3d> meshes;
		std::string directory;

		void loadModel(std::string path);

		void processNode(aiNode* node, const aiScene* scene);

		Mesh3d processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture3d> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	private:
		const aiScene* scene;
	};

	//盒子节点，使用额外的一个vbo
	class Model3dNode : public OpenglNode
	{
	public:
		Model3dNode();
		~Model3dNode();

		CREATE_FUNC(Model3dNode);

	public:
		virtual bool init();
		virtual void draw();

	private:
		Model3d* model;
	};
}

#endif