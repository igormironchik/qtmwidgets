
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef WINDOW_HPP__INCLUDED
#define WINDOW_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


//
// Window
//

class WindowPrivate;

//! Main window of the application.
class Window
	:	public QWidget
{
	Q_OBJECT

public:
	Window();
	virtual ~Window();

public slots:
	//! Start and finish.
	void startAndFinish( Qt::ApplicationState state );

private slots:
	//! Image loaded.
	void _q_imageLoaded();
	//! Files found.
	void _q_filesFound();

private:
	friend class WindowPrivate;

	Q_DISABLE_COPY( Window )

	QScopedPointer< WindowPrivate > d;
}; // class Window

#endif // WINDOW_HPP__INCLUDED
