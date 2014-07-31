#include <Qt\qapplication.h>
#include "render_widget.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	render_widget widget;
	widget.show();

	return app.exec();
}