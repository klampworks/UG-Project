#include <global.h>
#include "gui.h"
#include <QApplication>
 
/*
 * Entry point into the application. Starts the gui.
 */

int main(int argc, char **argv) {

	//Pass command line arguments through to the QT layers.
	QApplication app(argc, argv);
	
	global g;
	
	global::get_window()->resize(800, 750);
	global::get_window()->setWindowTitle("Crosswords Demo");
	global::get_window()->setWindowIcon(QIcon("../logo.png"));
	
	global::get_window()->show();

	app.exec();
	return 0;

}

