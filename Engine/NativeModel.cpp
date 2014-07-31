#pragma warning( disable : 4996)	// this warning is given when using std::copy inside the LoadOBJ function. 
									// It warns about the possibility of writing off the end of an array, but that will not happen in this code.

#include "NativeModel.h"

#include <FbxLoader.h>
#include <fstream>
#include <ShapeData.h>
#include <vector>
#include "EngineTypeDefs.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

NativeModel::~NativeModel()
{
	//for(uint i = 0; i < numShapeDatas; i++)
	//	shapeDatas[i].cleanUp();

	//delete shapeDatas;
	//free(shapeDatas);
}

void NativeModel::Save(const char* filepath)
{
	const uint SHAPE_DATA_ARRAY_SIZE = sizeof(Neumont::ShapeData) * numShapeDatas;

	std::ofstream out(filepath, std::ios::out | std::ios::binary);

	out.write(reinterpret_cast<char*>(&numShapeDatas), sizeof(numShapeDatas));	// write the number of shapes first.
	out.seekp(sizeof(numShapeDatas) + SHAPE_DATA_ARRAY_SIZE, std::ios::beg);

	Neumont::ShapeData* shapeDatasCopy = new Neumont::ShapeData[numShapeDatas];
	memcpy(shapeDatasCopy, shapeDatas, SHAPE_DATA_ARRAY_SIZE);

	for(uint i = 0; i < numShapeDatas; i++)
	{
		Neumont::ShapeData& d = shapeDatasCopy[i];

		for(uint k = 0; k < d.numVerts; k++)
		{
			d.verts[k].color.x /= 255.0f;
			d.verts[k].color.y /= 255.0f;
			d.verts[k].color.z /= 255.0f;
		}

		#pragma warning( disable : 4244)
		uint temp = out.tellp();
		out.write(reinterpret_cast<char*>(d.verts), d.vertexBufferSize());
		memcpy(&d.verts, &temp, sizeof(temp));

		temp = out.tellp();
		out.write(reinterpret_cast<char*>(d.indices), d.indexBufferSize());
		memcpy(&d.indices, &temp, sizeof(temp));
	}

	out.seekp(sizeof(numShapeDatas), std::ios::beg);
	out.write(reinterpret_cast<char*>(shapeDatasCopy), SHAPE_DATA_ARRAY_SIZE);

	out.close();
	
	delete [] shapeDatasCopy;
}

void NativeModel::LoadNativeModel(const char* filepath, NativeModel& m)
{
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios::end);

	const uint FILE_SIZE = in.tellg();

	char* buffer = (char*)malloc(FILE_SIZE);
	in.seekg(0, std::ios::beg);
	in.read(buffer, FILE_SIZE);
	in.close();

	{
		NativeModel sourceModel;

		sourceModel.numShapeDatas = *reinterpret_cast<uint*>(buffer);
		sourceModel.shapeDatas = reinterpret_cast<Neumont::ShapeData*>(buffer + sizeof(uint));

		m.numShapeDatas = sourceModel.numShapeDatas;
		m.shapeDatas = new Neumont::ShapeData[m.numShapeDatas];

		for(uint i = 0; i < sourceModel.numShapeDatas; i++)
		{
			Neumont::ShapeData& sourceShapeData = sourceModel.shapeDatas[i];
			Neumont::ShapeData& targetShapeData = m.shapeDatas[i];
		
			targetShapeData.numVerts = sourceShapeData.numVerts;
			targetShapeData.numIndices = sourceShapeData.numIndices;

			sourceShapeData.verts = reinterpret_cast<Neumont::Vertex*>(buffer + (uint)(sourceShapeData.verts));
			sourceShapeData.indices = reinterpret_cast<unsigned short*>(buffer + (uint)(sourceShapeData.indices));
			
			targetShapeData.verts = new Neumont::Vertex[sourceShapeData.numVerts];
			targetShapeData.indices = new ushort[sourceShapeData.numIndices];

			memcpy(targetShapeData.verts, sourceShapeData.verts, sourceShapeData.numVerts * sizeof(Neumont::Vertex));
			memcpy(targetShapeData.indices, sourceShapeData.indices, sourceShapeData.numIndices * sizeof(ushort));
		}
	}

	free(buffer);
}
void NativeModel::LoadFBX(const char* filepath, NativeModel& m)
{
	Neumont::FbxLoader::loadFbxFile(filepath, m.shapeDatas, m.numShapeDatas);
}
void NativeModel::LoadOBJ(const char* filepath, NativeModel& m)
{
	// http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ

	// read the file
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<ushort> indices;

	std::vector<Neumont::Vertex> vertices;	

	std::ifstream in(filepath, std::ios::in);

//	std::string contents;
//	in.seekg(0, std::ios::end);
//	contents.resize(in.tellg());
//	in.seekg(0, std::ios::beg);
//	in.read(&contents[0], contents.size());
//	in.close();
//	std::cout << contents << std::endl;


	// if(!in) // break. can't open file

	std::string line;
	while(std::getline(in, line))
	{
		std::string sub = line.substr(0, 2);
		if(sub == "v ")	// position
		{
			std::istringstream s(line.substr(2));
			glm::vec3 p;
			s >> p.x;
			s >> p.y;
			s >> p.z;
			positions.push_back(p);
		}
		else if(sub == "vt")	// uv
		{
			std::istringstream s(line.substr(2));
			glm::vec2 uv;
			s >> uv.x;
			s >> uv.y;
			uvs.push_back(uv);
		}
		else if(sub == "vn")	// normal
		{
			std::istringstream s(line.substr(2));
			glm::vec3 n;
			s >> n.x;
			s >> n.y;
			s >> n.z;
			normals.push_back(n);
		}
		else if(sub == "f ")
		{
			std::istringstream s(line.substr(2));
			//std::string st;
			//std::getline(in, st, '/');
			//std::cout << st << std::endl;


			struct vertexDataIndices
			{
				uint posIndex;
				uint uvIndex;
				uint normIndex;				
			} a,b,c;
			char delim;

			// TODO: Gracefully handle obj files that DON'T have all 3 of these defined. 
			//		(get substrings based on whitespace, then deliminate based on the slashes, 
			//		and determine what data is there.)
			s >> a.posIndex >> delim >> a.uvIndex >> delim >> a.normIndex;
			s >> b.posIndex >> delim >> b.uvIndex >> delim >> b.normIndex;
			s >> c.posIndex >> delim >> c.uvIndex >> delim >> c.normIndex;
			
			// TODO: look through the already defined vertices, if the same one exists don't add it again.
			//		http://stackoverflow.com/questions/571394/how-to-find-an-item-in-a-stdvector
			Neumont::Vertex v;

			// grab the right vertex data and add it to the shape data.
			v.position = positions[a.posIndex - 1];	// subtract 1 from all these because OBJ is 1 based not 0 based.
			if(uvs.size() > 0 && a.uvIndex >= 0)
				v.uv = uvs[a.uvIndex - 1];
			if(normals.size() > 0 && a.normIndex >= 0)
				v.normal = normals[a.normIndex - 1];
			v.color = glm::vec4(1,0,0,1);
			vertices.push_back(v);
			indices.push_back(vertices.size() - 1);

			v.position = positions[b.posIndex - 1];	// subtract 1 from all these because OBJ is 1 based not 0 based.
			if(uvs.size() > 0 && b.uvIndex >= 0)
				v.uv = uvs[b.uvIndex - 1];
			if(normals.size() > 0 && b.normIndex >= 0)
				v.normal = normals[b.normIndex - 1];
			v.color = glm::vec4(0,1,0,1);
			vertices.push_back(v);
			indices.push_back(vertices.size() - 1);

			v.position = positions[c.posIndex - 1];	// subtract 1 from all these because OBJ is 1 based not 0 based.
			if(uvs.size() > 0 && c.uvIndex >= 0)
				v.uv = uvs[c.uvIndex - 1];
			if(normals.size() > 0 && c.normIndex >= 0)
				v.normal = normals[c.normIndex - 1];
			v.color = glm::vec4(0,0,1,1);
			vertices.push_back(v);
			indices.push_back(vertices.size() - 1);
		}
		else if(line[0] == '#') { /* comment line in obj, do nothing */ }
		else { /* didn't know what the line was */ }
	}
	in.close();

	// calculate normals if needed.
	//	norms.resize(verts.size(), glm::vec3(0.0, 0.0, 0.0));	// initialize the normals
	//	for(uint i = 0; i < norms.size(); i += 3) 
	//	{
	//		ushort ia = indices[i];
	//		ushort ib = indices[i + 1];
	//		ushort ic = indices[i + 2];
	//	
	//		glm::vec3 normal = glm::normalize(glm::cross(
	//			glm::vec3(verts[ib]) - glm::vec3(verts[ia]),
	//			glm::vec3(verts[ic]) - glm::vec3(verts[ia])));
	//		norms[ia] = norms[ib] = norms[ic] = normal;
	//	}

	m.numShapeDatas = 1;
	m.shapeDatas = new Neumont::ShapeData[1];
	Neumont::ShapeData& data = m.shapeDatas[0];

	data.numVerts = vertices.size();
	data.numIndices = indices.size();

	data.verts = new Neumont::Vertex[data.numVerts];
	data.indices = new ushort[data.numIndices];
	
	std::copy(vertices.begin(), vertices.end(), data.verts);
	std::copy(indices.begin(), indices.end(), data.indices);


	std::cout << "Finished loading OBJ file: " << filepath << " | #vertices:" << vertices.size() << "  #indices:" << indices.size() << std::endl;
}