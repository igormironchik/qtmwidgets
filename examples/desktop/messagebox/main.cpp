
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

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

// Qt include.
#include <QApplication>
#include <QObject>
#include <QTimer>

// QtMWidgets include.
#include <QtMWidgets/MessageBox>


class Object
	:	public QObject
{
	Q_OBJECT

public:
	Object()
	{
	}

public slots:
	void start()
	{
		QString translatedTextAboutQtText;
		translatedTextAboutQtText = tr(
			"<p>Qt is a C++ toolkit for cross-platform application "
			"development.</p>"
			"<p>Qt provides single-source portability across all major desktop "
			"operating systems. It is also available for embedded Linux and other "
			"embedded and mobile operating systems.</p>"
			"<p>Qt is available under three different licensing options designed "
			"to accommodate the needs of our various users.</p>"
			"<p>Qt licensed under our commercial license agreement is appropriate "
			"for development of proprietary/commercial software where you do not "
			"want to share any source code with third parties or otherwise cannot "
			"comply with the terms of the GNU LGPL version 3 or GNU LGPL version 2.1.</p>"
			"<p>Qt licensed under the GNU LGPL version 3 is appropriate for the "
			"development of Qt applications provided you can comply with the terms "
			"and conditions of the GNU LGPL version 3.</p>"
			"<p>Qt licensed under the GNU LGPL version 2.1 is appropriate for the "
			"development of Qt applications provided you can comply with the terms "
			"and conditions of the GNU LGPL version 2.1.</p>"
			"<p>Please see <a href=\"http://%2/\">%2</a> "
			"for an overview of Qt licensing.</p>"
			"<p>Copyright (C) %1 Digia Plc and/or its subsidiary(-ies) and other "
			"contributors.</p>"
			"<p>Qt and the Qt logo are trademarks of Digia Plc and/or its subsidiary(-ies).</p>"
			"<p>Qt is a Digia product developed as an open source project. See <a href=\"http://%3/\">%3</a> "
			"for more information.</p>"
			).arg( QStringLiteral( "2014" ),
				  QStringLiteral( "qt.io/licensing" ),
				  QStringLiteral( "qt.io" ) );

		QtMWidgets::MessageBox msg( tr( "About Qt..." ),
			translatedTextAboutQtText );

		msg.exec();

		QApplication::quit();
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QPalette p = app.palette();
	p.setColor( QPalette::Text, p.color( QPalette::WindowText ) );

	app.setPalette( p );

	Object o;

	QTimer::singleShot( 0, &o, &Object::start );

	return app.exec();
}

#include "main.moc"
