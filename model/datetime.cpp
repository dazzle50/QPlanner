/***************************************************************************
 *   Copyright (C) 2014 by Richard Crook                                   *
 *   http://code.google.com/p/projectplanner                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "datetime.h"

/*************************************************************************************************/
/********************** XTime provides static methods to support Time type ***********************/
/*************************************************************************************************/

/********************************************* time **********************************************/

Time XTime::time( int hours, int mins )
{
  // return qint16 Time from hours & minutes
  Q_ASSERT( hours >= 0 );
  Q_ASSERT( hours <= 24 );
  Q_ASSERT( mins >= 0 );
  Q_ASSERT( mins <= 59 );
  Q_ASSERT( hours < 24 || mins == 0 );
  return hours*60 + mins;
}

/******************************************* toString ********************************************/

QString XTime::toString( Time t )
{
  // return HH:MM string from Time
  if ( t == XTime::NULL_TIME ) return QString( "Null" );

  Q_ASSERT( t >= 0 );
  Q_ASSERT( t <= 24*60 );
  return QString( "%1:%2" ).arg( t/60, 2, 10, QLatin1Char('0') )
                           .arg( t%60, 2, 10, QLatin1Char('0') );
}

/*************************************************************************************************/
/********************** XDate provides static methods to support Date type ***********************/
/*************************************************************************************************/

const QDate  XDate::ANCHOR_QDATE = QDate( 1, 1, 1 );
const qint64 XDate::ANCHOR_JULIAN = ANCHOR_QDATE.toJulianDay();

/********************************************* date **********************************************/

Date XDate::date( int year, int mon, int day )
{
  // return qint32 Date from year, month, day
  Q_ASSERT( year >= 1 );
  Q_ASSERT( year <= 8000 );
  Q_ASSERT( mon >= 1 );
  Q_ASSERT( mon <= 12 );
  Q_ASSERT( day >= 1 );
  Q_ASSERT( day <= 31 );

  return QDate( year, mon, day ).toJulianDay() - ANCHOR_JULIAN;
}

Date XDate::date( QDate qd )
{
  // return qint32 Date from QDate
  if ( qd.isNull() ) return NULL_DATE;
  return qd.toJulianDay() - ANCHOR_JULIAN;
}

/******************************************** qdate **********************************************/

QDate XDate::qdate( Date d )
{
  // return QDate from qint32 Date
  if ( d == NULL_DATE ) return QDate();
  return ANCHOR_QDATE.addDays( d );
}

/****************************************** currentDate ******************************************/

Date XDate::currentDate()
{
  // return qint32 Date for current date
  return QDate::currentDate().toJulianDay() - ANCHOR_JULIAN;
}

/******************************************* toString ********************************************/

QString XDate::toString( Date d )
{
  // return dd/MM/yyyy string from Date
  return toString( d, "dd/MM/yyyy" );
}

QString XDate::toString( Date d, QString format )
{
  // return string in specified format from Date
  if ( d == XDate::NULL_DATE ) return QString( "Null" );

  QDate qdate = ANCHOR_QDATE.addDays( d );
  return qdate.toString( format );
}
