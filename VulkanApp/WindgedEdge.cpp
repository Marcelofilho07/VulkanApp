#include <iostream>

#include "WingedEdge.h"

void WingedEdge::addVertex(glm::vec3 v)
{
	//vertices.insert(v);
}

void WingedEdge::addEdge(edge e)
{
	//edges.insert(e);
}

void WingedEdge::addFace(face f)
{
	faces.insert(f);
}

face* WingedEdge::genFace(glm::vec3 v00, glm::vec3 v01, glm::vec3 v02)
{
	if(vertices.count(v00) == 0)
	{
		vertices.insert(v00);
		Vertex newV{};
		newV.pos = v00;
		newV.color = {0.f,1.f,0.f};
		newV.texCoord = {0.f,0.f};
		verticesVec.push_back(newV);
		uniqueVertices[newV.pos] = static_cast<uint32_t>(verticesVec.size());
		
	}
	if(vertices.count(v01) == 0)
	{
		vertices.insert(v01);
		Vertex newV{};
		newV.pos = v01;
		newV.color = {1.f,0.f,0.f};
		newV.texCoord = {0.f,0.f};
		verticesVec.push_back(newV);
		uniqueVertices[newV.pos] = static_cast<uint32_t>(verticesVec.size());
	}
	if(vertices.count(v02) == 0)
	{
		vertices.insert(v02);
		Vertex newV{};
		newV.pos = v02;
		newV.color = {0.f,0.f,1.f};
		newV.texCoord = {0.f,0.f};
		verticesVec.push_back(newV);
		uniqueVertices[newV.pos] = static_cast<uint32_t>(verticesVec.size());
	}
	
	face* newFace = new face();
	edge e0{};
	e0.origVec = v00;
	e0.destVec = v01;
	if(edges.count(e0))
	{
		auto e = edges.extract(e0);
		e.value().r_face = newFace;
		edges.insert(e.value());
	}
	else
	{
		e0.l_face = newFace;
		edges.insert(e0);
	}
	edge e1{};
	e1.origVec = v01;
	e1.destVec = v02;
	if(edges.count(e1))
	{
		auto e = edges.extract(e1);
		e.value().r_face = newFace;
		edges.insert(e.value());
	}
	else
	{
		e1.l_face = newFace;
		edges.insert(e1);
	}
	edge e2{};
	e2.origVec = v02;
	e2.destVec = v00;
	if(edges.count(e2))
	{
		auto e = edges.extract(e2);
		e.value().r_face = newFace;
		edges.insert(e.value());
	}
	else
	{
		e2.l_face = newFace;
		edges.insert(e2);
	}
	
	newFace->e0 = e0;
	newFace->e1 = e1;
	newFace->e2 = e2;

	if(faces.count(*newFace) == 0)
	{
		listFaces.push_back(*newFace);
		faces.insert(*newFace);
		//printFace(*newFace);
	}

	return newFace;
}

void WingedEdge::printFace(face f)
{
	std::cout << "------ PRINTING FACE -----" << std::endl;
	std::cout << "v0: " << f.e0.origVec.x << " " << f.e0.origVec.y << " " << f.e0.origVec.z << std::endl;
	std::cout << "v1: " << f.e1.origVec.x << " " << f.e1.origVec.y << " " << f.e1.origVec.z << std::endl;
	std::cout << "v2: " << f.e2.origVec.x << " " << f.e2.origVec.y << " " << f.e2.origVec.z << std::endl;
	
}
