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

#include <QVariant>

#include "plan.h"
#include "resource.h"
#include "calendar.h"
#include "calendarsmodel.h"

/*************************************************************************************************/
/************************************* Single plan resource **************************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Resource::Resource()
{
  // set resource variables to default/null values
  m_availability = 1.0;
  m_cost         = 0.0;
  m_calendar     = nullptr;
  m_start        = XDate::NULL_DATE;
  m_end          = XDate::NULL_DATE;
}

/****************************************** constructor ******************************************/

Resource::Resource( bool unassigned )
{
  Q_UNUSED( unassigned )

  // set resource variables for unassigned resource
  m_availability = 1e10;
  m_cost         = 0.0;
  m_calendar     = nullptr;
  m_start        = XDate::NULL_DATE;
  m_end          = XDate::NULL_DATE;
  m_comment      = "Unassigned";
}

/****************************************** headerData *******************************************/

QVariant  Resource::headerData( int column )
{
  // return section horizontal header title text
  if ( column == SECTION_INITIALS )   return "Initials";
  if ( column == SECTION_NAME )       return "Name";
  if ( column == SECTION_ORG )        return "Organisation";
  if ( column == SECTION_GROUP )      return "Group";
  if ( column == SECTION_ROLE )       return "Role";
  if ( column == SECTION_ALIAS )      return "Alias";
  if ( column == SECTION_START )      return "Start";
  if ( column == SECTION_END )        return "End";
  if ( column == SECTION_AVAIL )      return "Available";
  if ( column == SECTION_COST )       return "Cost";
  if ( column == SECTION_CALENDAR )   return "Calendar";
  if ( column == SECTION_COMMENT )    return "Comment";
  return QVariant();
}

/********************************************* data **********************************************/

QVariant  Resource::data( int column, int role )
{
  // if initials are blank, other sections are grayed (as not editable)
  if ( role == Qt::BackgroundRole )
  {
    if ( column != SECTION_INITIALS && isNull() )
      return plan->nullCellColour();
  }

  // if initials are blank don't show anything
  if ( isNull() ) return QVariant();

  // if role is EditRole, return appropriate edit value
  if ( role == Qt::EditRole )
  {
    if ( column == SECTION_START )    return XDate::qdate( m_start );
    if ( column == SECTION_END )      return XDate::qdate( m_end );
    if ( column == SECTION_AVAIL )    return m_availability;
    if ( column == SECTION_COST )     return m_cost;

    // for all other columns return the DisplayRole for EditRole
    role = Qt::DisplayRole;
  }

  // if role is DisplayRole, return appropriate display value
  if ( role == Qt::DisplayRole )
  {
    if ( column == SECTION_INITIALS ) return m_initials;
    if ( column == SECTION_NAME )     return m_name;
    if ( column == SECTION_ORG )      return m_org;
    if ( column == SECTION_GROUP )    return m_group;
    if ( column == SECTION_ROLE )     return m_role;
    if ( column == SECTION_ALIAS )    return m_alias;
    if ( column == SECTION_START )    return XDate::toString( m_start );
    if ( column == SECTION_END )      return XDate::toString( m_end );
    if ( column == SECTION_AVAIL )    return QString("%1").arg( m_availability );
    if ( column == SECTION_COST )     return QString("%1").arg( m_cost, 0, 'f', 1 );;
    if ( column == SECTION_CALENDAR ) return m_calendar->name();
    if ( column == SECTION_COMMENT )  return m_comment;
  }

  // if role is TextAlignmentRole, return appropriate display alignment
  if ( role == Qt::TextAlignmentRole )
  {
    if ( column == SECTION_NAME ||
         column == SECTION_ORG  ||
         column == SECTION_GROUP ||
         column == SECTION_CALENDAR ||
         column == SECTION_COMMENT )   return int( Qt::AlignVCenter | Qt::AlignLeft );
    return Qt::AlignCenter;
  }

  // otherwise return null QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

void  Resource::setData( int col, const QVariant& value )
{
  // update resource (should only be called by undostack)
  if ( col == SECTION_INITIALS ) m_initials     = value.toString();
  if ( col == SECTION_NAME )     m_name         = value.toString();
  if ( col == SECTION_ORG )      m_org          = value.toString();
  if ( col == SECTION_GROUP )    m_group        = value.toString();
  if ( col == SECTION_ROLE )     m_role         = value.toString();
  if ( col == SECTION_ALIAS )    m_alias        = value.toString();
  if ( col == SECTION_AVAIL )    m_availability = value.toFloat();
  if ( col == SECTION_START )    m_start        = XDate::date( value.toDate() );
  if ( col == SECTION_END )      m_end          = XDate::date( value.toDate() );
  if ( col == SECTION_COST )     m_cost         = value.toFloat();
  if ( col == SECTION_COMMENT )  m_comment      = value.toString();

  if ( col == SECTION_CALENDAR )
  {
    int i      = plan->calendars()->namesList().indexOf( value.toString() );
    m_calendar = plan->calendar( i );
  }

  if ( m_calendar == nullptr ) m_calendar = plan->calendar();
  if ( isNull() ) m_calendar = nullptr;
  //plan->resources()->updateAssignable();
}

/********************************************* start *********************************************/

Date Resource::start() const
{
  // if start is not set, return min date
  if ( m_start == XDate::NULL_DATE ) return XDate::MIN_DATE;
  return m_start;
}

/********************************************** end **********************************************/

Date Resource::end() const
{
  // if end is not set, return max date
  if ( m_end == XDate::NULL_DATE ) return XDate::MAX_DATE;
  return m_end;
}
