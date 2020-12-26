#include <QtWidgets/QApplication>
#include"ModelShader.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

	ModelShader w(nullptr);
	w.show();
	w.setModel("./3DModelFile/Moon/Moon2K.obj");

	return a.exec();
}
 