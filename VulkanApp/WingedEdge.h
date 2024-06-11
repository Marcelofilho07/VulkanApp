#pragma once
#include <vector>
#include <array>
#include <forward_list>
#include <fstream>
#include <unordered_set>
#include <glm/vec2.hpp>
#include "glm/vec3.hpp"
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan_core.h>

struct edge;
struct face;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

struct edge
{
	glm::vec3 origVec;
	glm::vec3 destVec;

	edge* ccw_l_edge;
	edge* ccw_r_edge;
	edge* cw_l_edge;
	edge* cw_r_edge;

	face* l_face;
	face* r_face;
	int index;
	bool operator==(const edge& other) const {
		return ((origVec == other.origVec || origVec == other.destVec) &&
			(destVec == other.destVec || destVec == other.origVec));
	}
};

struct face
{
	edge* e0;
	edge* e1;
	edge* e2;
	int index;
	
	bool e0R = false;
	bool e1R = false;
	bool e2R = false;
	
	glm::vec3 normal()
	{
		glm::vec3 U = (e0->destVec - e0->origVec);
		glm::vec3 V = (e1->destVec - e0->origVec);
		
		return {-((U.y * V.z) - (U.z * V.y)), -((U.z * V.x) - (U.x * V.z)),-((U.x * V.y) - (U.y * V.x))};
	}
	
	bool operator==(const face& other) const {
		return ((e0->origVec == other.e0->origVec || e0->origVec == other.e1->origVec || e0->origVec == other.e2->origVec) &&
			(e1->origVec == other.e0->origVec ||	e1->origVec == other.e1->origVec || e1->origVec == other.e2->origVec) &&
			(e2->origVec == other.e0->origVec ||	e2->origVec == other.e1->origVec || e2->origVec == other.e2->origVec));
	}
};

namespace std
{
	template<> struct hash<edge> {
		size_t operator()(edge const& vertex) const {
						return ((hash<float>()(vertex.origVec.x + vertex.destVec.x) ^
							(hash<float>()(vertex.origVec.y + vertex.destVec.y) << 1) >> 1) ^
							hash<float>()(vertex.origVec.z + vertex.destVec.z) << 1);
		}
	};
    
	template<> struct hash<face> {
		size_t operator()(face const& vertex) const {
			return ((hash<float>()(vertex.e0->origVec.x + vertex.e1->origVec.x + vertex.e2->origVec.x) ^
				(hash<float>()(vertex.e0->origVec.y + vertex.e1->origVec.y + vertex.e2->origVec.y) << 1) >> 1) ^
				hash<float>()(vertex.e0->origVec.z + vertex.e1->origVec.z + vertex.e2->origVec.z) << 1);
		}
	};
}

class WingedEdge
{
public:
	face* genFace(glm::vec3 v00, glm::vec3 v01, glm::vec3 v02);
	
	void printFace(face f);
	void printEdges();
	void printWingedEdge();
	
	std::unordered_set<glm::vec3> vertices{};
	std::unordered_set<edge> edges{};
	std::unordered_set<face> faces{};

	std::list<face> listFaces{};
	std::vector<edge*> vectorOLFEdges{};

	std::vector<Vertex> verticesVec{};
	std::unordered_map<glm::vec3, uint32_t> uniqueVertices{};

};