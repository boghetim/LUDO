#include "ludo.h"
#include <QCoreApplication>

int main( int argc, char *argv[] )
{

	QCoreApplication a(argc, argv);
    Ludo *ludo = new Ludo(&a);
	return a.exec();
}
