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

#ifndef CALENDAR_H
#define CALENDAR_H

#include <QString>
#include <QVector>
#include <QHash>

#include "datetime.h"
#include "timespan.h"

class Day;

/*************************************************************************************************/
/********************************* Single calendar for planning **********************************/
/*************************************************************************************************/

class Calendar
{
  friend class CommandCalendarSetData;
public:
  Calendar();                                                  // constructor
  Calendar( int );                                             // constructor for initial default calendars

  QString       name() const { return m_name; }                // return calendar name
  int           cycleLength() const { return m_cycleLength; }  // return calendar cycle length
  bool          isWorking( Date ) const;

  Day*          day( Date ) const;                             // return day type for given date
  Day*          day( DateTime ) const;                         // return day type for given date-time
  DateTime      workUp( DateTime ) const;                      // return date-time now or future when working
  DateTime      workDown( DateTime ) const;                    // return date-time now or past when working

  DateTime      addTimeSpan( DateTime, TimeSpan );             // return date-time moved by TimeSpan
  DateTime      addMinutes( DateTime, int );                   // return date-time moved by minutes
  DateTime      addDays( DateTime, float );                    // return date-time moved by days
  DateTime      addWeeks( DateTime, float );                   // return date-time moved by weeks
  DateTime      addMonths( DateTime, float );                  // return date-time moved by months
  DateTime      addYears( DateTime, float );                   // return date-time moved by years
  TimeSpan      workBetween( DateTime, DateTime );             // return timespan between two date-times

  QVariant      data( int, int ) const;                        // return data for row & role
  void          setData( int, const QVariant& );               // set data value for column

  static QVariant   headerData( int );                         // return row header data

  enum DefaultCalendarTypes
  {
    DEFAULT_CALENDAR = 0,
    DEFAULT_FULLTIME = 1,
    DEFAULT_FANCY    = 2,
    DEFAULT_MAX      = 2
  };

  enum Sections
  {
    SECTION_NAME        = 0,
    SECTION_ANCHOR      = 1,
    SECTION_EXCEPTIONS  = 2,
    SECTION_CYCLELENGTH = 3,
    SECTION_NORMAL1     = 4
  };

private:
  QString             m_name;            // name of calendar
  Date                m_cycleAnchor;     // anchor date of calendar cycle
  quint8              m_cycleLength;     // length of basic cycle (eg 7)
  QVector<Day*>       m_normal;          // normal basic cycle days
  QHash<Date, Day*>   m_exceptions;      // exceptions override normal days
};

#endif // CALENDAR_H
