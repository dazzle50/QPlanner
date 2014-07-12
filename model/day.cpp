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

#include "day.h"
#include "plan.h"
#include "command/commanddaysetdata.h"

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
    if ( column == SECTION_PARTS )  return m_periods;

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

bool Day::setData( int col, const QVariant& value )
{
  // TODO some checks that set data will be allowed, return false if not allowed

  qDebug("%p Day::setData %i '%s'",this,col,qPrintable(value.toString()));

  // set data via undo/redo command
  //plan->undostack()->push( new CommandDaySetData( row, col, new_value, old_value ) );
  return true;
}

/****************************************** headerData *******************************************/

QVariant  Day::headerData( int section )
{
  // return section horizontal header title text
  if ( section == SECTION_NAME )  return "Name";
  if ( section == SECTION_WORK )  return "Work";
  if ( section == SECTION_PARTS ) return "Periods";

  if ( (section-SECTION_START) % 2 == 0 )
    return QString("Start %1").arg( (section-SECTION_START) / 2 + 1 );
  else
    return QString("End %1").arg( (section-SECTION_END) / 2 + 1 );
}
