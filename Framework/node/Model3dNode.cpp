#include "Model3dNode.h"
#include "FreeImage/TextureManager.h"
#include "../Director.h"

#include <sstream>
using namespace std;

namespace opengl_framework
{
	void Mesh3d::setupMesh()
	{
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex3d), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3d), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3d), (GLvoid*)offsetof(Vertex3d, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3d), (GLvoid*)offsetof(Vertex3d, TexCoords));

		glBindVertexArray(0);
	}

	void Mesh3d::draw(ShaderProgram *shader)
	{
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();

			glUniform1f(glGetUniformLocation(shader->getShaderID(), ("material." + name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}

		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Model3d::loadModel(std::string path)
	{
		Assimp::Importer import;
		scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/')+1);

		this->processNode(scene->mRootNode, scene);
	}

	void Model3d::processNode(aiNode* node, const aiScene* scene)
	{
		// Process all the node's meshes (if any)
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		// Then do the same for each of its children
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh3d Model3d::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex3d> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture3d> textures;

		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex3d vertex;

			// Process vertex positions, normals and texture coordinates
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// Process indices
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Process material
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture3d> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture3d> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh3d(vertices, indices, textures);
	}

	vector<Texture3d> Model3d::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		vector<Texture3d> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			Texture3d texture;
			auto myTexture = TextureCache::getInstance()->getTextureFromFile((directory + str.C_Str()).c_str());
			texture.id = myTexture->getTextureID();
			texture.type = typeName;
			textures.push_back(texture);
		}
		return textures;
	}

	Model3dNode::Model3dNode()
		: model(nullptr)
	{
		//
	}

	Model3dNode::~Model3dNode()
	{
		delete model;
	}

	bool Model3dNode::init()
	{
		auto shaderProgram = ShaderProgramCache::getInstance()->getShaderProgram(ShaderProgramCache::Model3d_Shader);
		setShaderProgram(shaderProgram);

		model = new Model3d("tortoise/tortoise.c3b");

		return true;
	}

	void Model3dNode::draw()
	{
		NODE_DRAW_SETUP();

		//glActiveTexture(GL_TEXTURE0);
		//getTexture()->bindTexture();
		//getShaderProgram()->setUniformTexture("ourTexture", 0);

		//glBindVertexArray(vao);

		glm::mat4 modelmat4;
		getShaderProgram()->updateMVP(modelmat4);

		this->model->draw(getShaderProgram());

		//glBindVertexArray(0);
	}
}