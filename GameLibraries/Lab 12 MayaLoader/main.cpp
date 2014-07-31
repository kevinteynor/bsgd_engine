#include "OfflineLoader.h"

int main()
{
	OfflineLoader loader;

	loader.OpenFile_FBX("C:\\Anivia.fbx", "Anivia.bin");

	return 1;
}