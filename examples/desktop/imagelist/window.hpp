
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2015 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
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
