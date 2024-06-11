#include <iostream>

#include "WingedEdge.h"

face* WingedEdge::genFace(glm::vec3 v00, glm::vec3 v01, glm::vec3 v02)
{
	if(vertices.count(v00) == 0)
	{
		vertices.insert(v00);
		Vertex newV{};
		newV.pos = v00;
		newV.color = {1.f,0.f,0.f};
		newV.texCoord = {0.f,0.f};
		verticesVec.push_back(newV);
		uniqueVertices[newV.pos] = static_cast<uint32_t>(verticesVec.size());
		
	}
	if(vertices.count(v01) == 0)
	{
		vertices.insert(v01);
		Vertex newV{};
		newV.pos = v01;
		newV.color = {0.f,1.f,0.f};
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
	edge* e0 = new edge();
	e0->origVec = v00;
	e0->destVec = v01;
	e0->index = static_cast<int>(edges.size());
	if(edges.count(*e0))
	{
		for(auto olE : vectorOLFEdges)
		{
			if(*olE == *e0)
			{
				olE->r_face = newFace;
				if(e0->origVec == olE->destVec)
				{
					newFace->e0R = true;
				}
				e0 = olE;
			}
		}
	}
	else
	{
		e0->l_face = newFace;
		edges.insert(*e0);
		vectorOLFEdges.push_back(e0);
	}
	edge* e1 = new edge();
	e1->origVec = v01;
	e1->destVec = v02;
	e1->index = static_cast<int>(edges.size());
	if(edges.count(*e1))
	{
		for(auto olE : vectorOLFEdges)
		{
			if(*olE == *e1)
			{
				olE->r_face = newFace;
				if(e1->origVec == olE->destVec)
				{
					newFace->e1R = true;
				}
				e1 = olE;
			}
		}
	}
	else
	{
		e1->l_face = newFace;
		edges.insert(*e1);
		vectorOLFEdges.push_back(e1);
	}
	edge* e2 = new edge();
	e2->origVec = v02;
	e2->destVec = v00;
	e2->index = static_cast<int>(edges.size());
	if(edges.count(*e2))
	{
		for(auto olE : vectorOLFEdges)
		{
			if(*olE == *e2)
			{
				olE->r_face = newFace;
				if(e2->origVec == olE->destVec)
				{
					newFace->e2R = true;
				}
				e2 = olE;
			}
		}
	}
	else
	{
		e2->l_face = newFace;
		edges.insert(*e2);
		vectorOLFEdges.push_back(e2);
	}
	
	newFace->e0 = e0;
	newFace->e1 = e1;
	newFace->e2 = e2;
	if(faces.count(*newFace) == 0)
	{
		newFace->index = static_cast<int>(faces.size());
		listFaces.push_back(*newFace);
		faces.insert(*newFace);
	}
	else
	{
		delete newFace;
	}

	return newFace;
}

void WingedEdge::printFace(face f)
{
	std::cout << "------ PRINTING FACE -----" << std::endl;
	std::cout << "INDEX: " << f.index << " Has R: " << (f.e0R || f.e1R || f.e2R ) << std::endl;
	std::cout << "v0: x: " << f.e0->origVec.x << " y: " << f.e0->origVec.y << " z: " << f.e0->origVec.z << std::endl;
	std::cout << "v1: x: " << f.e1->origVec.x << " y: " << f.e1->origVec.y << " z: " << f.e1->origVec.z << std::endl;
	std::cout << "v2: x: " << f.e2->origVec.x << " y: " << f.e2->origVec.y << " z: " << f.e2->origVec.z << std::endl;
}

void WingedEdge::printEdges()
{
	std::cout << "------ PRINTING EDGES -----" << std::endl;
	for(auto e : vectorOLFEdges)
	{
		if(e->r_face != nullptr)
		std::cout << "EDGE ----> OV: " << e->origVec.x  << " " << e->origVec.y  << " " << e->origVec.z  << " DV:" << e->destVec.x  << " " << e->destVec.y  << " " << e->destVec.z  << " l_f: " << e->l_face->index << " r_f: " << e->r_face->index << std::endl;
		else
		{
			std::cout << "EDGE ----> OV: " << e->origVec.x  << " " << e->origVec.y  << " " << e->origVec.z  << " DV:" << e->destVec.x  << " " << e->destVec.y  << " " << e->destVec.z  << " l_f: " << e->l_face->index << " r_f: null" << std::endl;
		}
	}
	std::cout << "---------------------------" << std::endl;
}

void WingedEdge::printWingedEdge()
{
	for(auto f : faces)
	{
		
	}
}
