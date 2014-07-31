#include <Qt\qapplication.h>
#include "EditorHostWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	EditorHostWindow window;
	window.Initialize();
	window.show();

	app.exec();
}