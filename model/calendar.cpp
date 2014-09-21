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

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "plan.h"
#include "calendarsmodel.h"
#include "daysmodel.h"
#include "calendar.h"
#include "day.h"

/*************************************************************************************************/
/********************************* Single calendar for planning **********************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Calendar::Calendar()
{
  // create null calendar
  m_name        = "Null";
  m_cycleAnchor = XDate::date(2000,1,1);   // Saturday 1st Jan 2000
  m_cycleLength = 0;
  m_normal.resize( m_cycleLength );
}

/****************************************** constructor ******************************************/

Calendar::Calendar( int type )
{
  Day* working    = plan->day( Day::DEFAULT_STANDARDWORK );
  Day* nonWorking = plan->day( Day::DEFAULT_NONWORK );

  // create calendars
  if ( type == DEFAULT_CALENDAR )
  {
    // create default base calendar
    m_name        = "Standard";
    m_cycleAnchor = XDate::date(2000,1,1);   // Saturday 1st Jan 2000
    m_cycleLength = 7;                       // 7 day week
    m_normal.resize( m_cycleLength );

    for( int n=0 ; n<7 ; n++ )
    {
      if ( n < 2 )    // Sat + Sun
        m_normal[n] = nonWorking;
      else            // Mon to Fri
        m_normal[n] = working;
    }

    // add easter and some bank holidays
    m_exceptions[ XDate::date(2014,12,25) ] = nonWorking;
    m_exceptions[ XDate::date(2014,12,26) ] = nonWorking;

    m_exceptions[ XDate::date(2015, 1, 1) ] = nonWorking;
    m_exceptions[ XDate::date(2015, 4, 3) ] = nonWorking;
    m_exceptions[ XDate::date(2015, 4, 6) ] = nonWorking;
    m_exceptions[ XDate::date(2015, 5, 4) ] = nonWorking;
    m_exceptions[ XDate::date(2015, 5,25) ] = nonWorking;
    m_exceptions[ XDate::date(2015, 8,31) ] = nonWorking;
    m_exceptions[ XDate::date(2015,12,25) ] = nonWorking;
    m_exceptions[ XDate::date(2015,12,28) ] = nonWorking;
  }
  else if ( type == DEFAULT_FANCY )
  {
    // create fancy calendar
    m_name        = "Fancy";
    m_cycleAnchor = XDate::date(2012,1,1);
    m_cycleLength = 10;                 // 10 day cycle
    m_normal.resize( m_cycleLength );

    m_normal[0] = nonWorking;
    m_normal[1] = nonWorking;
    m_normal[2] = nonWorking;
    m_normal[3] = plan->day( Day::DEFAULT_SHORT );
    m_normal[4] = plan->day( Day::DEFAULT_SHORT );
    m_normal[5] = plan->day( Day::DEFAULT_EVENING );
    m_normal[6] = plan->day( Day::DEFAULT_EVENING );
    m_normal[7] = plan->day( Day::DEFAULT_TWENTYFOURHOURS );
    m_normal[8] = nonWorking;
    m_normal[9] = plan->day( Day::DEFAULT_TWENTYFOURHOURS );

    m_exceptions[ XDate::date(2014,12,25) ] = nonWorking;
    m_exceptions[ XDate::date(2014,12,26) ] = nonWorking;
  }
  else  // DEFAULT_FULLTIME
  {
    // create fulltime calendar
    m_name        = "Full Time";
    m_cycleAnchor = XDate::date(2012,1,1);
    m_cycleLength = 1;                 // same day every day
    m_normal.resize( m_cycleLength );

    m_normal[0] = plan->day( Day::DEFAULT_TWENTYFOURHOURS );
  }
}

/****************************************** constructor ******************************************/

Calendar::Calendar( QXmlStreamReader* stream ) : Calendar()
{
  // create calendar from stream
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "name" )
      m_name = attribute.value().toString();

    if ( attribute.name() == "anchor" )
      m_cycleAnchor = XDate::fromString( attribute.value().toString() );
  }

  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if normal element append new day
    if ( stream->isStartElement() && stream->name() == "normal" )
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "day" )
        {
          int dayId = attribute.value().toString().toInt();
          if ( dayId >= plan->numDays() || dayId < 0 )
          {
            stream->raiseError( QString("Calendar invalid normal day '%1'").arg(dayId) );
            return;
          }

          m_normal.append( plan->day( dayId ) );
          m_cycleLength = m_normal.size();
        }
      }

    // if exception element append new exception
    if ( stream->isStartElement() && stream->name() == "exception" )
    {
      Date   date;
      Day*   day = nullptr;
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "date" )
          date = XDate::fromString( attribute.value().toString() );

        if ( attribute.name() == "day" )
        {
          int dayId = attribute.value().toString().toInt();
          if ( dayId >= plan->numDays() || dayId < 0 )
          {
            stream->raiseError( QString("Calendar invalid exception day '%1'").arg(dayId) );
            return;
          }

          day = plan->day( dayId );
        }
      }
      m_exceptions[ date ] = day;
    }

    // when reached end of calendar return
    if ( stream->isEndElement() && stream->name() == "calendar" ) return;
  }
}

/***************************************** saveToStream ******************************************/

void  Calendar::saveToStream( QXmlStreamWriter* stream )
{
  // write calendar data to xml stream
  stream->writeAttribute( "name", m_name );
  stream->writeAttribute( "anchor", XDate::toString( m_cycleAnchor, "yyyy-MM-dd" ) );

  for( int n=0 ; n<m_cycleLength ; n++ )
  {
    stream->writeEmptyElement( "normal" );
    stream->writeAttribute( "id", QString("%1").arg(n) );
    stream->writeAttribute( "day", QString("%1").arg(plan->index(m_normal[n])) );
  }

  QHashIterator<Date, Day*> e( m_exceptions );
  while ( e.hasNext() )
  {
      e.next();
      stream->writeEmptyElement( "exception" );
      stream->writeAttribute( "date", XDate::toString( e.key(), "yyyy-MM-dd" ) );
      stream->writeAttribute( "day", QString("%1").arg(plan->index(e.value())) );
  }
}

/********************************************** data *********************************************/

QVariant  Calendar::data( int row, int role  = Qt::DisplayRole ) const
{
  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    // no special handling
    // if not handled above return DisplayRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( row == SECTION_NAME )        return m_name;
    if ( row == SECTION_ANCHOR )      return XDate::qdate( m_cycleAnchor );
    if ( row == SECTION_EXCEPTIONS )  return m_exceptions.size();
    if ( row == SECTION_CYCLELENGTH ) return m_cycleLength;

    if ( row >= m_cycleLength+SECTION_NORMAL1 ) return QVariant();

    return m_normal.at(row - SECTION_NORMAL1)->name();
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    return int( Qt::AlignVCenter | Qt::AlignLeft );
  }

  // if role is BackgroundRole, return appropriate background colour
  if ( role == Qt::BackgroundRole )
  {
    if ( row >= m_cycleLength+SECTION_NORMAL1 ) return plan->nullCellColour();
    return QVariant();
  }

  // if role is ToolTipRole, return appropriate tool tip text
  if ( role == Qt::ToolTipRole )
  {
    if ( row < m_cycleLength+SECTION_NORMAL1 &&
         row >= SECTION_NORMAL1 )
    {
      Date   anchor  = m_cycleAnchor;
      int    len     = m_cycleLength;
      int    normal  = ( XDate::currentDate() - anchor ) % len;
      QDate  example = QDate::currentDate().addDays( row - SECTION_NORMAL1 - normal );
      return example.toString( "ddd dd MMM yyyy" );
    }
    return QVariant( QVariant::String );
  }

  // otherwise return an invalid QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

void Calendar::setData( int row, const QVariant& value )
{
  // set calendar data based on row
  if ( row == SECTION_NAME ) m_name = value.toString();

  if ( row == SECTION_ANCHOR ) m_cycleAnchor = XDate::date( value.toDate() );

  if ( row == SECTION_CYCLELENGTH )
  {
    int newLength = value.toInt();

    // determine number of rows before and after
    int oldLength = m_cycleLength;
    int oldRows = plan->calendars()->rowCount();
    m_cycleLength = newLength;
    int newRows = plan->calendars()->rowCount();
    m_cycleLength = oldLength;

    if ( newRows > oldRows ) plan->calendars()->beginInsert( newRows - oldRows );
    if ( newRows < oldRows ) plan->calendars()->beginRemove( oldRows - newRows );

    m_cycleLength = newLength;
    m_normal.resize( newLength );

    for( int l = oldLength ; l < newLength ; l++ )
      m_normal[l] = plan->day( Day::DEFAULT_STANDARDWORK );

    if ( newRows > oldRows ) plan->calendars()->endInsert();
    if ( newRows < oldRows ) plan->calendars()->endRemove();
  }

  if ( row == SECTION_EXCEPTIONS ) qDebug("Calendar::setData - exceptions NOT IMPLEMENTED YET!");

  if ( row >= SECTION_NORMAL1 )
  {
    int normal = row - SECTION_NORMAL1;
    Q_ASSERT( normal < m_cycleLength );
    int i = plan->days()->namesList().indexOf( value.toString() );
    m_normal[ normal ] = plan->day( i );
  }
}

/******************************************* headerData ******************************************/

QVariant Calendar::headerData( int section )
{
  // return section vertical header title text
  if ( section == Calendar::SECTION_NAME )        return "Name";
  if ( section == Calendar::SECTION_ANCHOR )      return "Anchor";
  if ( section == Calendar::SECTION_EXCEPTIONS )  return "Exceptions";
  if ( section == Calendar::SECTION_CYCLELENGTH ) return "Cycle";

  return QString("Normal %1").arg( section - Calendar::SECTION_NORMAL1 + 1 );
}

/********************************************** day **********************************************/

Day*  Calendar::day( Date date ) const
{
  // if exception exists return it, otherwise return normal cycle day
  if ( m_exceptions.contains( date ) ) return m_exceptions.value( date );

  int normal = ( date - m_cycleAnchor ) % m_cycleLength;
  if ( normal < 0 ) normal += m_cycleLength;

  return m_normal.at( normal );
}

/****************************************** isWorking ********************************************/

bool Calendar::isWorking( Date date ) const
{
  // return whether date is working or not
  return day( date )->isWorking();
}

/******************************************** workUp *********************************************/

DateTime Calendar::workUp( DateTime dt ) const
{
  // return date-time now or future when working
  Date  date = dt / 1440u;
  Day*  day  = Calendar::day( date );
  Time  time = day->workUp( dt % 1440u );

  // if day work-up time is null, need to move forward a day until a working day is found
  if ( time == XTime::NULL_TIME )
  {
    do
      day = Calendar::day( ++date );
    while ( !day->isWorking() );
    return date*1440u + day->start();
  }

  return date*1440u + time;
}

/******************************************* workDown ********************************************/

DateTime Calendar::workDown( DateTime dt ) const
{
  // return date-time now or past when working
  Date  date = dt / 1440u;
  Day*  day  = Calendar::day( date );
  Time  time = day->workDown( dt % 1440u );

  // if day work-down time is null, need to move back a day until a working day is found
  if ( time == XTime::NULL_TIME )
  {
    do
      day = Calendar::day( --date );
    while ( !day->isWorking() );
    return date*1440u + day->end();
  }

  return date*1440u + time;
}

/********************************************* addXXX ********************************************/

DateTime  Calendar::addTimeSpan( DateTime start, TimeSpan ts )
{
  // check if start is valid date-time
  if ( start == XDateTime::NULL_DATETIME )
  {
    qWarning("Calendar::addTimeSpan - invalid start");
    return start;
  }

  // if timespan is zero length return original start
  if ( ts.number() == 0.0f ) return start;

  // return date-time moved by TimeSpan
  if ( ts.units() == TimeSpan::UNIT_MINUTES ) return addMinutes( start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_HOURS )   return addMinutes( start, ts.number()*60.0 );
  if ( ts.units() == TimeSpan::UNIT_DAYS )    return addDays(    start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_WEEKS )   return addWeeks(   start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_MONTHS )  return addMonths(  start, ts.number() );
  if ( ts.units() == TimeSpan::UNIT_YEARS )   return addYears(   start, ts.number() );

  // unknown timespace units - should never happen!
  qWarning("Calendar::addTimeSpan - unknown timespace units '%c'",ts.units());
  return start;
}

DateTime  Calendar::addMinutes( DateTime start, int mins )
{
  // check that secs is not zero
  if ( mins == 0 ) return start;         // nothing to add

  // if secs is positive go forward in time, else go backwards
  if ( mins > 0 )
  {
    // use up any remaining working minutes on start date
    Date  date  = start / 1440u;
    Day*  today = day( date );
    int toGo  = today->minsToGo( start % 1440u );
    if ( toGo == mins ) return date*1440u + today->end();
    if ( toGo >  mins ) return date*1440u + today->doMins( start % 1440u, mins );

    // to go was insufficient so move to next day
    date++;
    mins -= toGo;

    // repeat forever until no need to move to next day
    while ( true )
    {
      // check if found finish date
      today = day( date );
      if ( today->minutes() == mins ) return date*1440u + today->end();
      if ( today->minutes() >  mins ) return date*1440u + today->doMins( 0, mins );

      // not finished so move to next day
      date++;
      mins -= today->minutes();
    }
  }
  else
  {
    // work backwards on any minutes done on start date
    mins = -mins;
    Date  date  = start / 1440u;
    Day*  today = day( date );
    int done  = today->minsDone( start % 1440u );
    if ( done == mins ) return date*1440u + today->start();
    if ( done >  mins ) return date*1440u + today->doMins( today->start(), done - mins );

    // done was insufficient so move to previous day
    date++;
    mins -= done;

    // repeat forever until no need to move to previous day
    while ( true )
    {
      // check if found finish date
      today = day( date );
      if ( today->minutes() == mins ) return date*1440u + today->start();
      if ( today->minutes() >  mins ) return date*1440u + today->doMins( today->start(), today->minutes() - mins );

      // not finished so move to previous day
      date--;
      mins -= today->minutes();
    }
  }
}

DateTime  Calendar::addDays( DateTime start, float days )
{
  // check that days is not zero
  if ( days == 0.0f ) return start;         // nothing to add

  // if days is positive go forward in time, else go backwards
  if ( days > 0.0f )
  {
    // use up any remaining working time on start date
    Date  date  = start / 1440u;
    Day*  today = day( date );
    float toGo  = today->workToGo( start % 1440u );
    if ( toGo == days ) return date*1440u + today->end();
    if ( toGo >  days ) return date*1440u + today->doWork( start % 1440u, days );

    // to go was insufficient so move to next day
    date++;
    days -= toGo;

    // repeat forever until no need to move to next day
    while ( true )
    {
      // check if found finish date
      today = day( date );
      if ( today->work() == days ) return date*1440u + today->end();
      if ( today->work() >  days ) return date*1440u + today->doWork( 0, days );

      // not finished so move to next day
      date++;
      days -= today->work();
    }
  }
  else
  {
    // work backwards on any work done on start date
    days = -days;
    Date  date  = start / 1440u;
    Day*  today = day( date );
    float done  = today->workDone( start % 1440u );
    if ( done == days ) return date*1440u + today->start();
    if ( done >  days ) return date*1440u + today->doWork( today->start(), done - days );

    // done was insufficient so move to previous day
    date--;
    days -= done;

    // repeat forever until no need to move to previous day
    while ( true )
    {
      // check if found finish date
      today = day( date );
      if ( today->work() == days ) return date*1440u + today->start();
      if ( today->work() >  days ) return date*1440u + today->doWork( today->start(), today->work() - days );

      // not finished so move to previous day
      date--;
      days -= today->work();
    }
  }
}

DateTime  Calendar::addWeeks( DateTime start, float weeks )
{
  // return date-time moved by weeks (7 day week ignores non-working days)
  return start + weeks*7*1440u;
}

DateTime  Calendar::addMonths( DateTime start, float months )
{
  // return date-time moved by months
  int       whole    = floor(months);
  float     fraction = months - whole;
  QDateTime qdt      = XDateTime::qdatetime( start );
  qdt = qdt.addMonths( whole );

  if ( fraction == 0.0f ) return XDateTime::datetime( qdt );
  QDate date1 = qdt.date();
  QDate date2 = date1.addMonths(1);
  int   diff  = date2.toJulianDay() - date1.toJulianDay();
  return XDateTime::datetime( qdt.addDays( diff * fraction ) );
}

DateTime  Calendar::addYears( DateTime start, float years )
{
  // return date-time moved by years
  int       whole    = floor(years);
  float     fraction = years - whole;
  QDateTime qdt      = XDateTime::qdatetime( start );

  if ( fraction == 0.0f ) return XDateTime::datetime( qdt.addYears( whole ) );
  return addMonths( XDateTime::datetime( qdt.addYears( whole ) ), fraction*12.0f );
}

/****************************************** workBetween ******************************************/

TimeSpan Calendar::workBetween( DateTime start, DateTime end )
{
  // return work between two dates
  if ( start == XDateTime::NULL_DATETIME || end== XDateTime::NULL_DATETIME ) return TimeSpan( 0.0f, TimeSpan::UNIT_DAYS );
  if ( start == end ) return TimeSpan( 0.0f, TimeSpan::UNIT_DAYS );
  if ( start >  end ) qSwap( start, end );
  Date  sd   = start/1440u;
  Date  ed   = end/1440u;
  Day*  day  = Calendar::day( sd );
  float work = day->workToGo( start % 1440u );

  if ( sd == ed ) return TimeSpan( work - day->workToGo( end % 1440u ), TimeSpan::UNIT_DAYS );

  for( Date date = sd+1 ; date < ed ; date++ )
    work += Calendar::day( date )->work();

  work += Calendar::day( ed )->workDone( end % 1440u );

  return TimeSpan( work, TimeSpan::UNIT_DAYS );
}
