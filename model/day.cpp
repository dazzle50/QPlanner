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

#include "day.h"
#include "plan.h"
#include "daysmodel.h"

/*************************************************************************************************/
/**************************** Single day type used in plan calendars *****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Day::Day()
{
  // create null day
  m_name    = "Null";
  m_work    = 0.0f;
  m_periods = 0;
  m_minutes = 0;
}

/****************************************** constructor ******************************************/

Day::Day( int type )
{
  // create template day
  if ( type == DEFAULT_STANDARDWORK )
  {
    // default working day
    m_name     = "Standard work day";
    m_work     = 1.0f;
    m_periods  = 2;
    m_start.resize(2);
    m_end.resize(2);
    m_start[0] = XTime::time( 9,0);
    m_end[0]   = XTime::time(13,0);
    m_start[1] = XTime::time(14,0);
    m_end[1]   = XTime::time(18,0);
  }
  else if ( type == DEFAULT_TWENTYFOURHOURS )
  {
    // default 24H day
    m_name     = "24H day";
    m_work     = 1.5f;
    m_periods  = 1;
    m_start.resize(1);
    m_end.resize(1);
    m_start[0] = XTime::time( 0,0);
    m_end[0]   = XTime::time(24,0);
  }
  else if ( type == DEFAULT_SHORT )
  {
    // default short day
    m_name     = "Morning only";
    m_work     = 0.5f;
    m_periods  = 1;
    m_start.resize(1);
    m_end.resize(1);
    m_start[0] = XTime::time( 9,0);
    m_end[0]   = XTime::time(13,0);
  }
  else if ( type == DEFAULT_EVENING )
  {
    // default evening shift
    m_name     = "Evening shift";
    m_work     = 0.6f;
    m_periods  = 1;
    m_start.resize(1);
    m_end.resize(1);
    m_start[0] = XTime::time(18,0);
    m_end[0]   = XTime::time(22,0);
  }
  else  // DEFAULT_NONWORK
  {
    // default non-working day
    m_name    = "Non working";
    m_work    = 0.0f;
    m_periods = 0;
  }

  // ensure cached worked minutes in day is set correctly
  calcMinutes();
}

/****************************************** constructor ******************************************/

Day::Day( QXmlStreamReader* stream ) : Day()
{
  // create day from stream
  foreach( QXmlStreamAttribute attribute, stream->attributes() )
  {
    if ( attribute.name() == "name" )
      m_name = attribute.value().toString();

    if ( attribute.name() == "work" )
      m_work = attribute.value().toString().toFloat();
  }

  while ( !stream->atEnd() )
  {
    stream->readNext();

    // if period element append new day period
    if ( stream->isStartElement() && stream->name() == "period" )
    {
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "start" )
          m_start.append( XTime::time( attribute.value().toString() ) );

        if ( attribute.name() == "end" )
          m_end.append( XTime::time( attribute.value().toString() ) );
      }

      m_periods = m_start.size();
      if ( m_periods != m_end.size() )
        qWarning("Day::Day( QXmlStreamReader* ) WARNING size of period starts & ends not equal!");
    }

    // when reached end of day return
    if ( stream->isEndElement() && stream->name() == "day" ) break;
  }

  // ensure cached worked minutes in day is set correctly
  calcMinutes();
}

/***************************************** saveToStream ******************************************/

void  Day::saveToStream( QXmlStreamWriter* stream )
{
  // write day data to xml stream
  stream->writeAttribute( "name", m_name );
  stream->writeAttribute( "work", QString("%1").arg(m_work) );

  for( int p=0 ; p<m_periods ; p++ )
  {
    stream->writeEmptyElement( "period" );
    stream->writeAttribute( "id", QString("%1").arg(p) );
    stream->writeAttribute( "start", XTime::toString( m_start[p] ) );
    stream->writeAttribute( "end", XTime::toString( m_end[p] ) );
  }
}

/****************************************** calcMinutes ******************************************/

void Day::calcMinutes()
{
  // calculate number of worked minutes in day
  m_minutes = 0;
  for( int p=0 ; p<m_periods ; p++ )
    m_minutes += m_end[p] - m_start[p];
}

/********************************************* data **********************************************/

QVariant  Day::data( int column, int role )
{
  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    if ( column == SECTION_WORK )   return m_work;

    // for all other columns return the DisplayRole for EditRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( column == SECTION_NAME )   return m_name;
    if ( column == SECTION_WORK )   return QString("%1").arg( m_work, 0, 'f', 2 );
    if ( column == SECTION_PERIODS )  return m_periods;

    if ( column >= m_periods*2+SECTION_START ) return QVariant();

    if ( (column-SECTION_START) % 2 == 0 )
      return XTime::toString( m_start.at(( column-SECTION_START) / 2 ) );
    else
      return XTime::toString( m_end.at( (column-SECTION_END) / 2 ) );
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    if ( column == SECTION_NAME )   return int( Qt::AlignVCenter | Qt::AlignLeft );
    return Qt::AlignCenter;
  }

  // if role is BackgroundRole, return appropriate background colour
  if ( role == Qt::BackgroundRole )
  {
    if ( column >= m_periods*2+SECTION_START ) return plan->nullCellColour();
    return QVariant();
  }

  // otherwise return an invalid QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

void Day::setData( int col, const QVariant& value )
{
  // set day type data based on column
  if ( col == SECTION_NAME ) m_name = value.toString();

  if ( col == SECTION_WORK ) m_work = value.toFloat();

  if ( col == SECTION_PERIODS )
  {
   int newPeriods = value.toInt();

   // determine number of columns before and after
   int oldValue   = m_periods;
   int oldColumns = plan->days()->columnCount();
   m_periods      = newPeriods;
   int newColumns = plan->days()->columnCount();
   m_periods      = oldValue;

   if ( newColumns > oldColumns ) plan->days()->beginInsert( newColumns - oldColumns );
   if ( newColumns < oldColumns ) plan->days()->beginRemove( oldColumns - newColumns );

   Time time = 0;
   if ( m_periods > 0 ) time = m_end.at( m_periods-1 );
   Time increment = ( 24*60 - time ) / ( 2 * ( newPeriods - m_periods ) );
   if ( increment > 4*60 && increment < 8*60 ) increment = 4*60;
   if ( increment > 2*60 && increment < 4*60 ) increment = 2*60;
   if ( increment >   60 && increment < 2*60 ) increment =   60;
   if ( increment >   30 && increment <   60 ) increment =   30;
   if ( increment >   10 && increment <   30 ) increment =   10;

   m_start.resize( newPeriods );
   m_end.resize( newPeriods );

   for( int p = m_periods ; p < newPeriods ; p++ )
   {
     time += increment;
     m_start[p] = time;
     time += increment;
     m_end[p] = time;
   }

   m_periods = newPeriods;
   calcMinutes();

   if ( newColumns > oldColumns ) plan->days()->endInsert();
   if ( newColumns < oldColumns ) plan->days()->endRemove();
  }

  if ( col >= SECTION_START )
  {
    // check if a start or an end
    if ( (col-SECTION_START) % 2 == 0 )
      m_start[ (col-SECTION_START) / 2 ] = XTime::time( value.toString() );
    else
      m_end[ (col-SECTION_END) / 2 ] = XTime::time( value.toString() );

    calcMinutes();
  }
}

/****************************************** headerData *******************************************/

QVariant  Day::headerData( int section )
{
  // return section horizontal header title text
  if ( section == SECTION_NAME )  return "Name";
  if ( section == SECTION_WORK )  return "Work";
  if ( section == SECTION_PERIODS ) return "Periods";

  if ( (section-SECTION_START) % 2 == 0 )
    return QString("Start %1").arg( (section-SECTION_START) / 2 + 1 );
  else
    return QString("End %1").arg( (section-SECTION_END) / 2 + 1 );
}

/********************************************* start *********************************************/

Time Day::start()
{
  // return day start time or NULL time if non-working day
  if ( m_periods < 1 ) return XTime::NULL_TIME;
  return m_start.at( 0 );
}

/********************************************** end **********************************************/

Time Day::end()
{
  // return day end time or NULL time if non-working day
  if ( m_periods < 1 ) return XTime::NULL_TIME;
  return m_end.at( m_periods-1 );
}

/******************************************** workUp *********************************************/

Time Day::workUp( Time t )
{
  // return time now or future when working or NULL time
  for( int p=0 ; p<m_periods ; p++ )
  {
    if ( t <= m_start.at(p) ) return m_start.at(p);
    if ( t >= m_start.at(p) && t < m_end.at(p) ) return t;
  }
  return XTime::NULL_TIME;
}

/******************************************* workDown ********************************************/

Time Day::workDown( Time t )
{
  // return time now or past when working or NULL time
  for( int p=m_periods-1 ; p>=0 ; p-- )
  {
    if ( t >= m_end.at(p) ) return m_end.at(p);
    if ( t >= m_start.at(p) && t < m_end.at(p) ) return t;
  }
  return XTime::NULL_TIME;
}

/******************************************* workDone ********************************************/

float Day::workDone( Time t )
{
  // return days equivalent work done start to time
  if ( m_work == 0.0f || m_minutes == 0 ) return 0.0f;
  return ( m_work * minsDone( t ) ) / m_minutes;
}

/******************************************* workToGo ********************************************/

float Day::workToGo( Time t )
{
  // return days equivalent work togo time to end
  if ( m_work == 0.0f || m_minutes == 0 ) return 0.0f;
  return ( m_work * minsToGo( t ) ) / m_minutes;
}

/******************************************* minsDone ********************************************/

int Day::minsDone( Time t )
{
  // return number of minutes done from start to time
  int done = 0;
  for( int p=0 ; p<m_periods ; p++ )
  {
    if ( t <= m_start.at(p) ) return done;
    if ( t >= m_start.at(p) && t <= m_end.at(p) ) return done + m_end.at(p) - t;
    done += m_end.at(p) - m_start.at(p);
  }
  return done;
}

/******************************************* minsToGo ********************************************/

int Day::minsToGo( Time t )
{
  // return number of minutes togo from time to end
  return m_minutes - minsDone( t );
}

/******************************************** doWork *********************************************/

Time  Day::doWork( Time time, float work ) const
{
  // calculate number of minutes to move forward
  int mins = int( 0.5 + m_minutes * work / m_work );
  return doMins( time, mins );
}

/******************************************** doMins *********************************************/

Time  Day::doMins( Time time, int mins ) const
{
  // if no periods than can't do any work
  if ( m_periods == 0 ) return XTime::NULL_TIME;

  // move forward by number of working minutes
  for( int p=0 ; p < m_periods ; p++ )
  {
    if ( time < m_start.at(p) ) time = m_start.at(p);
    int toGo = m_end.at(p) - time;
    if ( mins <= toGo ) return time + mins;
    mins -= toGo;
    time = 0;
  }

  qWarning("Day::doMins - ERROR asked to do more minutes work than remains!!!");
  return XTime::NULL_TIME;
}

/******************************************** stretch ********************************************/

Time  Day::stretch( Time now ) const
{
  // if no working times return orginal date-time
  if ( m_periods == 0 ) return now;

  Time s = m_start.at( 0 );
  Time e = m_end.at( m_periods-1 );
  if ( now <= s ) return 0;
  if ( now >= e ) return 1439;

  // stretch time component so it uses whole 24 hours
  double scale = 1440.0 / ( e - s );
  return int( scale * ( now - s ) );
}
