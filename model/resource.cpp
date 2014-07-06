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
    if ( column == SECTION_CALENDAR ) return plan->index( m_calendar );

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
    if ( column == SECTION_AVAIL )    return QString("%1").arg( m_availability, 0, 'f', 1 );
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

bool  Resource::setData( int row, int col, const QVariant& new_value )
{
  // TODO some checks that set data will be allowed, return false if not allowed

  // if value hasn't changed, don't proceed
  QVariant old_value = data( col, Qt::EditRole );
  if ( new_value == old_value ) return false;

  // set data via undo/redo command
  //plan->undostack()->push( new CommandResourceSetData( row, col, new_value, old_value ) );
  return true;
}