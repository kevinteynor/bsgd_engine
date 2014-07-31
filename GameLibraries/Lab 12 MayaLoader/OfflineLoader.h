#ifndef OFFLINE_LOADER_H
#define OFFLINE_LOADER_H

namespace Neumont { struct ShapeData; }

class OfflineLoader
{
private:
	void WriteBinaryFile_FBX(Neumont::ShapeData* datas, unsigned int numShapes, const char* filepath);

public:
	OfflineLoader(void){}
	~OfflineLoader(void){}

	void OpenFile_FBX(const char* infilename, const char* outfilename);
	
};

#endif // OFFLINE_LOADER_H