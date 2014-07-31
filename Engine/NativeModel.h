#ifndef NATIVE_MODEL_H
#define NATIVE_MODEL_H

#include <ShapeData.h>
#include "EngineTypeDefs.h"
#include "ExportHeader.h"

class ENGINE_SHARED NativeModel
{
public:
	// TODO: rewrite neumont::shapedata as my own structure.
	Neumont::ShapeData* shapeDatas;
	uint numShapeDatas;

	NativeModel(){}
	~NativeModel();

	void Save(const char* filepath);

	static void LoadNativeModel(const char* filepath, NativeModel& m);
	static void LoadFBX(const char* filepath, NativeModel& m);
	static void LoadOBJ(const char* filepath, NativeModel& m);
};

#endif // NATIVE_MODEL_H