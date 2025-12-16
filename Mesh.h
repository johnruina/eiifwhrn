#ifndef MESH_CLASS
#define MESH_CLASS

#include <iostream>
#include <random>
#include <vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <assimp/ai_assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "t.h"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};



class Mesh : public t_package {
protected:
public:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	VAO vao;
	VBO vbo;
	EBO ebo;

	Mesh(std::vector<GLfloat>& meshvertices, std::vector<GLuint>& meshindices) :

		vertices(meshvertices),
		indices(meshindices),
		vao(),
		vbo(&vertices[0], vertices.size() * sizeof(GLfloat)),
		ebo(&indices[0], indices.size() * sizeof(GLuint))

	{

		vbo.Bind();
		ebo.Bind();

		vao.LinkVBO(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.LinkVBO(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		vao.LinkVBO(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		vao.Unbind();
		vbo.Unbind();


		ebo.Unbind();

		//rest

		SetMatrix( glm::mat4(1.0f) );
	};

	void Clear() {
		vertices = std::vector<GLfloat>();
		indices = std::vector<GLuint>();
	}

	bool LoadMesh(const std::string& Filename) {
		Clear();

		bool Ret = false;
		Assimp::Importer Importer;

		const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		if (pScene) {
			Ret = InitFromScene(pScene, Filename);
		}
		else {
			printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
		}

		return Ret;
	}

	bool InitFromScene(const aiScene* pScene, const std::string& Filename)
	{
		m_Entries.resize(pScene->mNumMeshes);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			InitMesh(i, paiMesh);
		}
	}

	void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
	{
		m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

		std::vector Vertices;
		std::vector Indices;
		...
			We start the initialization of the mesh by storing its material index.This will be used during rendering to bind the proper texture.Next we create two STL vectors to store the contents of the vertex and index buffers.A STL vector has a nice property of storing its contents in a continuous buffer.This makes it easy to load the data into the OpenGL buffer(using the glBufferData() function).

			(mesh.cpp:118)

			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]) : &Zero3D;
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

			Vertex v(Vector3f(pPos->x, pPos->y, pPos->z),
				Vector2f(pTexCoord->x, pTexCoord->y),
				Vector3f(pNormal->x, pNormal->y, pNormal->z));

			Vertices.push_back(v);
		}

			for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
				const aiFace& Face = paiMesh->mFaces[i];
				assert(Face.mNumIndices == 3);
				Indices.push_back(Face.mIndices[0]);
				Indices.push_back(Face.mIndices[1]);
				Indices.push_back(Face.mIndices[2]);
			}
		+
			m_Entries[Index].Init(Vertices, Indices);
	}

	virtual void Render(Shader ShaderProgram) {
		glm::mat4 topass = GetMatrix();
		ShaderProgram.Activate();
		ShaderProgram.SetMat4("modl",  topass);
		vao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		vao.Unbind();
	};


private:

};


#endif