#ifndef MODEL_H
#define MODEL_H

#include <sb7.h>
#include <vmath.h>
#include <string>

class Model {
public:
	std::vector<vmath::vec3> vPositions;
	std::vector<vmath::vec2> vTexCoords;
	std::vector<vmath::vec3> vNormals;
	std::vector<GLuint> vIndices;

	GLuint diffuseMap, specularMap, normalMap;
	float shininess;
	vmath::vec3 defaultAmbient, defaultDiffuse, defaultSpecular;

	GLuint VAO;

	std::vector<vmath::vec3> tangents;
	std::vector<vmath::vec3> bitangents;
	std::vector<unsigned short> indices;
	std::vector<vmath::vec3> indexed_vertices;
	std::vector<vmath::vec2> indexed_uvs;
	std::vector<vmath::vec3> indexed_normals;
	std::vector<vmath::vec3> indexed_tangents;
	std::vector<vmath::vec3> indexed_bitangents;
	GLuint tangentbuffer;
	GLuint bitangentbuffer;

	Model() {
		shininess = 32.f;
		useDiffuseMap = false;
		useSpecularMap = false;
		useNormalMap = false;

		defaultAmbient = vmath::vec3(1.0f, 1.0f, 1.0f);
		defaultDiffuse = vmath::vec3(1.0f, 1.0f, 1.0f);
		defaultSpecular = vmath::vec3(0.0f, 0.0f, 0.0f);
	}

	~Model() {
		glDeleteTextures(1, &diffuseMap);
		glDeleteTextures(1, &specularMap);
		glDeleteTextures(1, &normalMap);

		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO_positions);
		glDeleteBuffers(1, &VBO_texCoords);
		glDeleteBuffers(1, &VBO_normals);
		glDeleteBuffers(1, &tangentbuffer);
		glDeleteBuffers(1, &bitangentbuffer);
		glDeleteVertexArrays(1, &VAO);
	}

	void init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_positions);
		glGenBuffers(1, &VBO_texCoords);
		glGenBuffers(1, &VBO_normals);
		glGenBuffers(1, &tangentbuffer);
		glGenBuffers(1, &bitangentbuffer);
		glGenBuffers(1, &EBO);

		glGenTextures(1, &diffuseMap);
		glGenTextures(1, &specularMap);
		glGenTextures(1, &normalMap);
	}

	void setDefaultColor(vmath::vec3 _color) {
		defaultDiffuse = _color;
	}

	void setDefaultSpecular(vmath::vec3 _color) {
		defaultSpecular = _color;
	}

	void setupMesh(int _numVertices, GLfloat* _vPositions, GLfloat* _vTexCoords = NULL, GLfloat* _vNormals = NULL) {
		// 1. copy data from arrays
		for (int i = 0; i < _numVertices; i++) {
			vmath::vec3 position;
			position[0] = _vPositions[i * 3 + 0];
			position[1] = _vPositions[i * 3 + 1];
			position[2] = _vPositions[i * 3 + 2];
			this->vPositions.push_back(position);
		}

		if (_vTexCoords) {
			for (int i = 0; i < _numVertices; i++) {
				vmath::vec2 texCoords;
				texCoords[0] = _vTexCoords[i * 2 + 0];
				texCoords[1] = _vTexCoords[i * 2 + 1];
				this->vTexCoords.push_back(texCoords);

				//vmath::vec3 v0 = _vPositions[i * 3 + 0];
				//vmath::vec3 v1 = _vPositions[i * 3 + 1];
				//vmath::vec3 v2 = _vPositions[i * 3 + 2];

				//vmath::vec2 uv0 = _vTexCoords[i * 2 + 0];
				//vmath::vec2 uv1 = _vTexCoords[i * 2 + 1];
				//vmath::vec2 uv2 = _vTexCoords[i * 2 + 2];

				//vmath::vec3 deltaPos1 = v1 - v0;
				//vmath::vec3 deltaPos2 = v2 - v0;

				//vmath::vec2 deltaUV1 = uv1 - uv0;
				//vmath::vec2 deltaUV2 = uv2 - uv0;

				//float r = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);
				//vmath::vec3 tangentX = (deltaPos1[0] * deltaUV2[1] - deltaPos2[0] * deltaUV1[1]) * r;
				//vmath::vec3 tangentY = (deltaPos1[1] * deltaUV2[1] - deltaPos2[1] * deltaUV1[1]) * r;
				//vmath::vec3 tangentZ = (deltaPos1[2] * deltaUV2[1] - deltaPos2[2] * deltaUV1[1]) * r;
				//vmath::vec3 bitangentX = (deltaPos2[0] * deltaUV1[0] - deltaPos1[0] * deltaUV2[0]) * r;
				//vmath::vec3 bitangentY = (deltaPos2[0] * deltaUV1[0] - deltaPos1[0] * deltaUV2[0]) * r;
				//vmath::vec3 bitangentZ = (deltaPos2[0] * deltaUV1[0] - deltaPos1[0] * deltaUV2[0]) * r;
			}
		}

		if (_vNormals) {
			for (int i = 0; i < _numVertices; i++) {
				vmath::vec3 normal;
				normal[0] = _vNormals[i * 3 + 0];
				normal[1] = _vNormals[i * 3 + 1];
				normal[2] = _vNormals[i * 3 + 2];
				this->vNormals.push_back(normal);
			}
		}

		if (_vTexCoords && _vNormals) {
			computeTangentBasis(vPositions, vTexCoords, vNormals);
			indexVBO_TBN(vPositions, vTexCoords, vNormals, tangents, bitangents);
		}

		// 2. create buffers
		prepareBuffers();

	}

	void setupIndices(int _numIndices, GLuint* _indices) {
		for (int i = 0; i < _numIndices; i++) {
			vIndices.push_back(_indices[i]);
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(int), &vIndices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	bool loadDiffuseMap(std::string _filepath) {
		if (loadTextureFile(diffuseMap, _filepath)) {
			useDiffuseMap = true;
			return true;
		}

		useDiffuseMap = false;
		return false;
	}

	bool loadSpecularMap(std::string _filepath) {
		if (loadTextureFile(specularMap, _filepath)) {
			useSpecularMap = true;
			return true;
		}

		useSpecularMap = false;
		return false;
	}

	bool loadNormalMap(std::string _filepath) {
		if (loadTextureFile(normalMap, _filepath)) {
			useNormalMap = true;
			return true;
		}

		useNormalMap = false;
		return false;
	}

	void draw(GLuint _shaderID) {
		glUniform3fv(glGetUniformLocation(_shaderID, "material.defaultAmbient"), 1, defaultAmbient);
		glUniform3fv(glGetUniformLocation(_shaderID, "material.defaultDiffuse"), 1, defaultDiffuse);
		glUniform3fv(glGetUniformLocation(_shaderID, "material.defaultSpecular"), 1, defaultSpecular);
		glUniform1i(glGetUniformLocation(_shaderID, "material.useDiffuseMap"), (int)useDiffuseMap);
		glUniform1i(glGetUniformLocation(_shaderID, "material.useSpecularMap"), (int)useSpecularMap);
		glUniform1i(glGetUniformLocation(_shaderID, "material.useNormalMap"), (int)useNormalMap);

		glUniform1i(glGetUniformLocation(_shaderID, "useNormal"), (int)vNormals.size());

		if (useDiffuseMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.diffuse"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
		}

		if (useSpecularMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.specular"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);

			glUniform1f(glGetUniformLocation(_shaderID, "material.shininess"), shininess);
		}

		if (useNormalMap) {
			glUniform1i(glGetUniformLocation(_shaderID, "material.normalMap"), 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, normalMap);
		}

		glBindVertexArray(VAO);
		if (vIndices.empty())
			glDrawArrays(GL_TRIANGLES, 0, vPositions.size());
		else
			glDrawElements(GL_TRIANGLES, vIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	bool loadOBJ(const char* filepath) {
		FILE* file = fopen(filepath, "r");
		if (file == NULL)
			return false;

		bool readVT = false, readVN = false;

		std::vector<vmath::vec3> tmpPositions;
		std::vector<vmath::vec2> tmpTexCoords;
		std::vector<vmath::vec3> tmpNormals;
		std::vector<GLuint> tmpVIndices, tmpVtIndices, tmpVnIndices;

		while (true) {
			char lineBuffer[512];

			// 처음 한 단어 읽어오기
			int res = fscanf(file, "%s", lineBuffer);
			// 파일 끝이면 읽기 종료
			if (res == EOF)
				break;

			// 버텍스 3차원 포지션 읽어오기
			if (strcmp(lineBuffer, "v") == 0) {
				vmath::vec3 v;
				fscanf(file, "%f %f %f\n", &v[0], &v[1], &v[2]);
				tmpPositions.push_back(v);
			}
			// 버텍스 텍스처 좌표 읽어오기
			else if (strcmp(lineBuffer, "vt") == 0) {
				vmath::vec2 vt;
				fscanf(file, "%f %f\n", &vt[0], &vt[1]);
				tmpTexCoords.push_back(vt);
				readVT = true;
			}
			// 버텍스 노말 읽어오기
			else if (strcmp(lineBuffer, "vn") == 0) {
				vmath::vec3 vn;
				fscanf(file, "%f %f %f\n", &vn[0], &vn[1], &vn[2]);
				tmpNormals.push_back(vn);
				readVN = true;
			}
			// 페이스 인덱스들 읽어오기
			else if (strcmp(lineBuffer, "f") == 0) {
				GLuint vIndex[3], vtIndex[3], vnIndex[3];

				// 포지션만 있는 경우
				if (!readVT && !readVN) {
					fscanf(file, "%d %d %d\n", &vIndex[0], &vIndex[1], &vIndex[2]);
				}
				// 포지션/텍스쳐좌표 있는 경우
				else if (readVT && !readVN) {
					fscanf(file, "%d/%d %d/%d %d/%d\n", &vIndex[0], &vtIndex[0], &vIndex[1], &vtIndex[1], &vIndex[2], &vtIndex[2]);
				}
				// 포지션/노멀 있는 경우
				else if (!readVT && readVN) {
					fscanf(file, "%d//%d %d//%d %d//%d\n", &vIndex[0], &vnIndex[0], &vIndex[1], &vnIndex[1], &vIndex[2], &vnIndex[2]);
				}
				// 포지션/텍스쳐좌표/노멀 있는 경우
				else if (readVT && readVN) {
					fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &vtIndex[0], &vnIndex[0], &vIndex[1], &vtIndex[1], &vnIndex[1], &vIndex[2], &vtIndex[2], &vnIndex[2]);
				}

				for (int i = 0; i < 3; i++) {
					tmpVIndices.push_back(vIndex[i]);
					if (readVT)
						tmpVtIndices.push_back(vtIndex[i]);
					if (readVN)
						tmpVnIndices.push_back(vnIndex[i]);
				}
			}
			else {
				char trashBuffer[1000];
				fgets(trashBuffer, 1000, file);
			}
		}

		vPositions.clear();
		vTexCoords.clear();
		vNormals.clear();
		vIndices.clear();
		// 인덱스를 사용하지는 않고 그냥 중첩해서 정보를 저장 (OpenGL EBO는 단일 인덱스밖에 지원하지 않으므로...)
		for (int i = 0; i < tmpVIndices.size(); i++) {
			vmath::vec3 position = tmpPositions[tmpVIndices[i] - 1];
			vPositions.push_back(position);
		}
		for (int i = 0; i < tmpVtIndices.size(); i++) {
			vmath::vec2 texCoords = tmpTexCoords[tmpVtIndices[i] - 1];
			vTexCoords.push_back(texCoords);
		}
		for (int i = 0; i < tmpVnIndices.size(); i++) {
			vmath::vec3 normal = tmpPositions[tmpVnIndices[i] - 1];
			vNormals.push_back(normal);
		}

		prepareBuffers();

		return true;
	}

private:
	GLuint VBO_positions, VBO_texCoords, VBO_normals, VBO_aTangent, VBO;
	GLuint EBO;
	bool useDiffuseMap, useSpecularMap, useNormalMap;

	void prepareBuffers() {
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
		glBufferData(GL_ARRAY_BUFFER, vPositions.size() * sizeof(vmath::vec3), &vPositions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		if (!vTexCoords.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO_texCoords);
			glBufferData(GL_ARRAY_BUFFER, vTexCoords.size() * sizeof(vmath::vec2), &vTexCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		}

		if (!vNormals.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
			glBufferData(GL_ARRAY_BUFFER, vNormals.size() * sizeof(vmath::vec3), &vNormals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}

		// normal mapping
		if (!indexed_tangents.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
			glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(vmath::vec3), &indexed_tangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if (!indexed_bitangents.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
			glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(vmath::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	bool loadTextureFile(GLuint textureID, std::string filepath) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

		if (data) {
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			return true;
		}

		stbi_image_free(data);
		return false;
	}

	void computeTangentBasis(
		// inputs
		std::vector<vmath::vec3>& vertices,
		std::vector<vmath::vec2>& uvs,
		std::vector<vmath::vec3>& normals
	) {

		for (unsigned int i = 0; i < vertices.size(); i += 3) {

			// Shortcuts for vertices
			vmath::vec3& v0 = vertices[i + 0];
			vmath::vec3& v1 = vertices[i + 1];
			vmath::vec3& v2 = vertices[i + 2];

			// Shortcuts for UVs
			vmath::vec2& uv0 = uvs[i + 0];
			vmath::vec2& uv1 = uvs[i + 1];
			vmath::vec2& uv2 = uvs[i + 2];

			// Edges of the triangle : postion delta
			vmath::vec3 deltaPos1 = v1 - v0;
			vmath::vec3 deltaPos2 = v2 - v0;

			// UV delta
			vmath::vec2 deltaUV1 = uv1 - uv0;
			vmath::vec2 deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV1[1] * deltaUV2[0]);
			vmath::vec3 tangent = (deltaPos1 * deltaUV2[1] - deltaPos2 * deltaUV1[1]) * r;
			vmath::vec3 bitangent = (deltaPos2 * deltaUV1[0] - deltaPos1 * deltaUV2[0]) * r;

			// Set the same tangent for all three vertices of the triangle.
			// They will be merged later, in vboindexer.cpp
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);

			// Same thing for binormals
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);

		}

		// See "Going Further"
		for (unsigned int i = 0; i < vertices.size(); i += 1)
		{
			vmath::vec3& n = normals[i];
			vmath::vec3& t = tangents[i];
			vmath::vec3& b = bitangents[i];

			// Gram-Schmidt orthogonalize
			t = vmath::normalize(t - n * vmath::dot(n, t));

			// Calculate handedness
			if (vmath::dot(vmath::cross(n, t), b) < 0.0f) {
				t = t * -1.0f;
			}

		}


	}

	void indexVBO_TBN(
		std::vector<vmath::vec3>& in_vertices,
		std::vector<vmath::vec2>& in_uvs,
		std::vector<vmath::vec3>& in_normals,
		std::vector<vmath::vec3>& in_tangents,
		std::vector<vmath::vec3>& in_bitangents
	) {
		// For each input vertex
		for (unsigned int i = 0; i < in_vertices.size(); i++) {

			// Try to find a similar vertex in out_XXXX
			unsigned short index;
			bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], indexed_vertices, indexed_uvs, indexed_normals, index);

			if (found) { // A similar vertex is already in the VBO, use it instead !
				indices.push_back(index);

				// Average the tangents and the bitangents
				indexed_tangents[index] += in_tangents[i];
				indexed_bitangents[index] += in_bitangents[i];
			}
			else { // If not, it needs to be added in the output data.
				indexed_vertices.push_back(in_vertices[i]);
				indexed_uvs.push_back(in_uvs[i]);
				indexed_normals.push_back(in_normals[i]);
				indexed_tangents.push_back(in_tangents[i]);
				indexed_bitangents.push_back(in_bitangents[i]);
				indices.push_back((unsigned short)indexed_vertices.size() - 1);
			}
		}
	}

	// Returns true iif v1 can be considered equal to v2
	bool is_near(float v1, float v2) {
		return fabs(v1 - v2) < 0.01f;
	}

	// Searches through all already-exported vertices
	// for a similar one.
	// Similar = same position + same UVs + same normal
	bool getSimilarVertexIndex(
		vmath::vec3& in_vertex,
		vmath::vec2& in_uv,
		vmath::vec3& in_normal,
		std::vector<vmath::vec3>& out_vertices,
		std::vector<vmath::vec2>& out_uvs,
		std::vector<vmath::vec3>& out_normals,
		unsigned short& result
	) {
		// Lame linear search
		for (unsigned int i = 0; i < out_vertices.size(); i++) {
			if (
				is_near(in_vertex[0], out_vertices[i][0]) &&
				is_near(in_vertex[1], out_vertices[i][1]) &&
				is_near(in_vertex[2], out_vertices[i][2]) &&
				is_near(in_uv[0], out_uvs[i][0]) &&
				is_near(in_uv[1], out_uvs[i][1]) &&
				is_near(in_normal[0], out_normals[i][0]) &&
				is_near(in_normal[1], out_normals[i][1]) &&
				is_near(in_normal[2], out_normals[i][2])
				) {
				result = i;
				return true;
			}
		}
		// No other vertex could be used instead.
		// Looks like we'll have to add it to the VBO.
		return false;
	}

};

#endif 