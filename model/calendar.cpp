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

#include "plan.h"
#include "daysmodel.h"
#include "calendar.h"
#include "day.h"
#include "command/commandcalendarsetdata.h"

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

/********************************************** data *********************************************/

QVariant  Calendar::data( int row, int role  = Qt::DisplayRole ) const
{
  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    if ( row >= ROW_NORMAL1 )
    {
      QString  name = m_normal.at(row - ROW_NORMAL1)->name();
      return   plan->days()->namesList().indexOf( name );
    }

    // if not handled above return DisplayRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( row == ROW_NAME )        return m_name;
    if ( row == ROW_ANCHOR )      return XDate::qdate( m_cycleAnchor );
    if ( row == ROW_EXCEPTIONS )  return m_exceptions.size();
    if ( row == ROW_CYCLELENGTH ) return m_cycleLength;

    if ( row >= m_cycleLength+ROW_NORMAL1 ) return QVariant();

    return m_normal.at(row - ROW_NORMAL1)->name();
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    return int( Qt::AlignVCenter | Qt::AlignLeft );
  }

  // if role is BackgroundRole, return appropriate background colour
  if ( role == Qt::BackgroundRole )
  {
    if ( row >= m_cycleLength+ROW_NORMAL1 ) return plan->nullCellColour();
    return QVariant();
  }

  // if role is ToolTipRole, return appropriate tool tip text
  if ( role == Qt::ToolTipRole )
  {
    if ( row < m_cycleLength+ROW_NORMAL1 &&
         row >= ROW_NORMAL1 )
    {
      Date   anchor  = m_cycleAnchor;
      int    len     = m_cycleLength;
      int    normal  = ( XDate::currentDate() - anchor ) % len;
      QDate  example = QDate::currentDate().addDays( row - ROW_NORMAL1 - normal );
      return example.toString( "ddd dd MMM yyyy" );
    }
    return QVariant( QVariant::String );
  }

  // otherwise return an invalid QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

bool Calendar::setData( int row, const QVariant& value )
{
  // TODO some checks that set data will be allowed, return false if not allowed

  // set data via undo/redo command
  //plan->undostack()->push( new CommandCalendarSetData( row, col, new_value, old_value ) );
  return true;
}
