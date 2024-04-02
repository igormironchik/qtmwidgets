
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>

#include "window.hpp"


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Window w;

	w.resize( 320, 480 );
	w.show();

	QObject::connect( &app, &QApplication::applicationStateChanged,
		&w, &Window::startAndFinish );

	return app.exec();
}
