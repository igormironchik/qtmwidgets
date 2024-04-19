[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

> [!IMPORTANT]
>
> This project is not supported anymore. But can be reopened if interest will
> be in it. Please, let me know if you need something. Just write me email to
> `igor.mironchik at gmail.com`.

# About

Library with a set of widgets for mobile development.

* [Examples](#examples)
  * [Picker](#picker)
  * [DateTimePicker, DatePicker, TimePicker](#datetimepicker-datepicker-timepicker)
  * [Switch](#switch)
  * [AbstractScrollArea, ScrollArea](#abstractscrollarea-scrollarea)
  * [TableView](#tableview)
  * [Slider](#slider)

# Examples

Below you can see some widgets from this library. But I've decided to not
provide more examples on this page. This library is in development. Use it
and enjoy it.

## Picker

Picker is the list of selectable strings on the cylinder. It looks like iOS Picker but with API like
QComboBox.

![Picker]( doc/img/picker_v2.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::Picker * picker =
    new QtMWidgets::Picker( this );

picker->addItem( QLatin1String( "English" ) );
picker->addItem( QLatin1String( "Russian" ) );
picker->addItem( QLatin1String( "German" ) );
picker->addItem( QLatin1String( "Spanish" ) );
picker->addItem( QLatin1String( "Portuguese" ) );
picker->addItem( QLatin1String( "Belorussian" ) );
picker->addItem( QLatin1String( "Polish" ) );
picker->addItem( QLatin1String( "Ukrainian" ) );

QFont font = picker->font();
font.setBold( true );
font.setPointSize( 15 );

picker->setFont( font );
```

</details>

## DateTimePicker, DatePicker, TimePicker

DateTimePicker, DatePicker and TimePicker are widgets for selecting date &amp; time, date, time. They
are look similar as correspondence widgets from iOS but have interface as QDateTimeEdit.

![DateTimePicker]( doc/img/datetimepicker_v2.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::DateTimePicker * picker =
    new QtMWidgets::DateTimePicker( this );

QFont font = picker->font();
font.setPointSize( 13 );

picker->setFont( font );
```
</details>

![DatePicker]( doc/img/datepicker_v2.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::DatePicker * picker =
    new QtMWidgets::DatePicker( this );

QFont font = picker->font();
font.setPointSize( 13 );

picker->setFont( font );
```

</details>

![TimePicker]( doc/img/timepicker_v2.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::TimePicker * picker =
    new QtMWidgets::TimePicker( this );

QFont font = picker->font();
font.setPointSize( 13 );

picker->setFont( font );

picker->setFormat( QLatin1String( "hh mm a" ) );
```

</details>

## Switch

Switch is On/Off button used, for example, in Setting app for options that can be switched.

![Switch]( doc/img/switch_v2.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::Switch * s =
    new QtMWidgets::Switch( this );

QFont f = s->font();
f.setBold( true );
s->setFont( f );

s->setOnText( QLatin1String( "ON" ) );
s->setOffText( QLatin1String( "OFF" ) );
```

</details>

## AbstractScrollArea, ScrollArea

AbstractScrollArea and ScrollArea provides possibility of scrolling large widgets that
doesn't fit the screen area.

![ScrollArea]( doc/img/scrollarea.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::ScrollArea * scrollArea =
    new QtMWidgets::ScrollArea( this );

QLabel * imageLabel = new QLabel;
QImage image( ":/flower.jpg" );
imageLabel->setPixmap( QPixmap::fromImage( image ) );

scrollArea->setBackgroundRole( QPalette::Dark );
scrollArea->setWidget( imageLabel );
```

</details>

## TableView

TableView is a view with sections with rows (cells). It's very similar to UITableView
from iOS. TableView is widget based view that allows to display sections with rows
(cells). Cell can have accessory widget, for example, Switch. This class is very usefull
for settings of the application.

![TableView]( doc/img/tableview-v3.png )

## Slider

This is the most common slider, almost identical to QSlider.
But it doesn't support styles, but it acceptable to use
on mobile platforms. It has big slider's handle and highlights
position of the handle in the groove.

![Slider]( doc/img/slider_v2.png )

<details>
<summary>Code example</summary>

```cpp
QtMWidgets::Slider * volumeSlider =
    new QtMWidgets::Slider(
        Qt::Horizontal, this );
```
</details>
