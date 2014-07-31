#include "OfflineLoader.h"
#include <FbxLoader.h>
#include <ShapeData.h>

#include <fstream>
using std::ios;

void OfflineLoader::OpenFile_FBX(const char* infilename, const char* outfilename)
{
	Neumont::ShapeData* datas;
	uint numShapeDatas;

	Neumont::FbxLoader::loadFbxFile(infilename, datas, numShapeDatas);
	WriteBinaryFile_FBX(datas, numShapeDatas, outfilename);

	for(unsigned int i = 0; i < numShapeDatas; i++)
		datas[i].cleanUp();
	delete [] datas;
}
void OfflineLoader::WriteBinaryFile_FBX(Neumont::ShapeData* datas, unsigned int numShapes, const char* filepath)
{
	const unsigned int SHAPE_DATA_ARRAY_SIZE = sizeof(Neumont::ShapeData) * numShapes;

	std::ofstream out(filepath, ios::out | ios::binary);

	out.write(reinterpret_cast<char*>(&numShapes), sizeof(numShapes));
	
	out.seekp(sizeof(numShapes) + SHAPE_DATA_ARRAY_SIZE, ios::beg);
	Neumont::ShapeData* shapeDataCopy = new Neumont::ShapeData[numShapes];
	memcpy(shapeDataCopy, datas, SHAPE_DATA_ARRAY_SIZE);
	
	for(unsigned int i = 0; i < numShapes; i++)
	{
		Neumont::ShapeData& d = shapeDataCopy[i];

		for(uint k = 0; k < d.numVerts; k++)
		{
			d.verts[k].color.x /= 255.0f;
			d.verts[k].color.y /= 255.0f;
			d.verts[k].color.z /= 255.0f;
		}

		unsigned int temp = out.tellp();
		out.write(reinterpret_cast<char*>(d.verts), d.vertexBufferSize());
		memcpy(&d.verts, &temp, sizeof(temp));

		temp = out.tellp();
		out.write(reinterpret_cast<char*>(d.indices), d.indexBufferSize());
		memcpy(&d.indices, &temp, sizeof(temp));		
	}

	out.seekp(sizeof(numShapes), ios::beg);
	out.write(reinterpret_cast<char*>(shapeDataCopy), SHAPE_DATA_ARRAY_SIZE);

	out.close();

	delete [] shapeDataCopy;
}