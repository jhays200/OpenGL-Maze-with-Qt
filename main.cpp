#include "GLScene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	GLScene test;
	test.show();
	return app.exec();
}