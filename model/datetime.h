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

#ifndef DATETIME_H
#define DATETIME_H

#include <QtGlobal>
#include <QDateTime>

typedef  quint32 DateTime;   // simple date-time between 1AD and ~8000AD with one minute resolution
typedef  qint32  Date;       // simple date between 1AD and ~8000AD with one day resolution
typedef  qint16  Time;       // simple time between 00:00H and *24:00H* with one minute resolution

/*************************************************************************************************/
/********************** XTime provides static methods to support Time type ***********************/
/*************************************************************************************************/

class XTime
{
public:
  const static Time   NULL_TIME = -1;          // Time value that represents null

  static Time      time( int, int );           // return Time from hours & minutes
  static Time      time( QString );            // return Time from HH:MM string
  static QString   toString( Time );           // return HH:MM string from Time

  // CANNOT convert Time to QTime because Time can represent *24:00* unlike QTime
};

/*************************************************************************************************/
/********************** XDate provides static methods to support Date type ***********************/
/*************************************************************************************************/

class XDate
{
public:
  const static Date    NULL_DATE = -1;         // Date value that represents null
  const static QDate   ANCHOR_QDATE;           // anchor QDate for Date zero
  const static qint64  ANCHOR_JULIAN;          // anchor QDate julian for Date zero

  static Date      date( int, int, int );      // return Date from year, month, day
  static Date      date( QDate );              // return Date from QDate
  static QDate     qdate( Date );              // return QDate from Date
  static Date      currentDate();              // return Date for current date
  static QString   toString( Date );           // return dd/MM/yyyy string from Date
  static QString   toString( Date, QString );  // return string in format from Date
};

/*************************************************************************************************/
/****************** XDateTime provides static methods to support DateTime type *******************/
/*************************************************************************************************/

class XDateTime
{
public:
  const static DateTime    NULL_DATETIME = -1;      // DateTime value that represents null
  const static DateTime    MAX_DATETIME = -2;       // max permissible DateTime value
  const static DateTime    MIN_DATETIME = 0;        // min permissible DateTime value
  const static QDateTime   ANCHOR_QDATETIME;        // anchor QDateTime for DateTime zero

  enum Interval
  {
    INTERVAL_YEAR,                     // Interval is one year
    INTERVAL_HALFYEAR,                 // Interval is half a year
    INTERVAL_QUARTERYEAR,              // Interval is quarter year
    INTERVAL_MONTH,                    // Interval is one month
    INTERVAL_WEEK,                     // Interval is one week
    INTERVAL_DAY                       // Interval is one day
  };

  static DateTime    datetime( QDateTime );         // return DateTime from QDateTime
  static QDateTime   qdatetime( DateTime );         // return QDateTime from DateTime
  static DateTime    currentDateTime();             // return DateTime for current date-time
  static QString     toString( DateTime );          // return dd/MM/yyyy hh:mm string from DateTime
  static QString     toString( DateTime, QString ); // return string in format from DateTime
};

#endif // DATETIME_H
