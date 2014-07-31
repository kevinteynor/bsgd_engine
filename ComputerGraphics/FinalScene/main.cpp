#include <Qt\qapplication.h>
#include "SceneWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	int s = sizeof(Renderer);

	SceneWindow window;
	window.Initialize();
	window.show();

	return app.exec();
}