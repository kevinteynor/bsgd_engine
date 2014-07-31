#include <ctime>
#include <Qt\qapplication.h>
#include "CaptureTheFlag.h"

int main(int argc, char* argv[])
{
	std::srand(std::time(0));

	QApplication app(argc, argv);

	CaptureTheFlag game;
	game.show();

	app.exec();
}