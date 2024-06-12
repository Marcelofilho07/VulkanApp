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

	std::vector<edge*> vEdges{};
	std::vector<bool> vIsReversed{};

	edge getEdge(int index)
	{
		if(vIsReversed[index])
		{
			edge toReturn = *vEdges[index];
			glm::vec3 sup = toReturn.origVec;
			toReturn.origVec = toReturn.destVec;
			toReturn.destVec = sup;
			return toReturn;
		}

		return *vEdges[index];
	}
	
	glm::vec3 normal()
	{
		glm::vec3 U = (e0->destVec - e0->origVec);
		glm::vec3 V = (e1->destVec - e0->origVec);
		
		return {-((U.y * V.z) - (U.z * V.y)), -((U.z * V.x) - (U.x * V.z)),-((U.x * V.y) - (U.y * V.x))};
	}

	glm::vec3 normalPrototype()
	{
		auto e0 = *vEdges.begin();
		auto e1 = *(vEdges.rbegin() + 1);
		glm::vec3 U = (e0->destVec - e0->origVec);
		glm::vec3 V = (e1->destVec - e0->origVec);
		
		return {-((U.y * V.z) - (U.z * V.y)), -((U.z * V.x) - (U.x * V.z)),-((U.x * V.y) - (U.y * V.x))};
	}
	
	bool operator==(const face& other) const {
		if(vEdges.size() != other.vEdges.size())
		{
			return false;
		}
		
		int index = 0;
		uint32_t equalsQuantity = 0;
		for(auto e : vEdges)
		{
			int otherIndex = 0;
			for(auto otherE : other.vEdges)
			{
				if(vIsReversed[index])
				{
					if(other.vIsReversed[otherIndex])
					{
						if(e->destVec == otherE->destVec)
						{
							++equalsQuantity;
						}
					}
					else
					{
						if(e->destVec == otherE->origVec)
						{
							++equalsQuantity;
						}
					}
				}
				else
				{
					if(other.vIsReversed[otherIndex])
					{
						if(e->origVec == otherE->destVec)
						{
							++equalsQuantity;
						}
					}
					else
					{
						if(e->origVec == otherE->origVec)
						{
							++equalsQuantity;
						}
					}
				}
				++otherIndex;
			}
			++index;
		}

		if(equalsQuantity == static_cast<uint32_t>(vEdges.size()))
		{
			return true;
		}
		return false;
		
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
		size_t operator()(face const& face) const {
			auto h = hash<float>()(1.f);
			int index = 0;
			for(auto e : face.vEdges)
			{
				if(face.vIsReversed[index])
				{
					h = h ^ hash<float>()(e->destVec.x);
					h = h ^ ((hash<float>()(e->destVec.y) << 1) >> 1);
					h = h ^ (hash<float>()(e->destVec.z) << 1);
				}
				else
				{
					h = h ^ hash<float>()(e->origVec.x);
					h = h ^ ((hash<float>()(e->origVec.y) << 1) >> 1);
					h = h ^ (hash<float>()(e->origVec.z) << 1);
				}
				++index;
			}

			return h;
			return ((hash<float>()(face.e0->origVec.x + face.e1->origVec.x + face.e2->origVec.x) ^
				(hash<float>()(face.e0->origVec.y + face.e1->origVec.y + face.e2->origVec.y) << 1) >> 1) ^
				hash<float>()(face.e0->origVec.z + face.e1->origVec.z + face.e2->origVec.z) << 1);
		}
	};
}

class WingedEdge
{
public:
	face* genFace(glm::vec3 v00, glm::vec3 v01, glm::vec3 v02);
	face* genFace(std::vector<glm::vec3> vVertices);
	
	void printFace(face f);
	void printFacePrototype(face f);
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