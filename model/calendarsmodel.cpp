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

#include "calendarsmodel.h"
#include "calendar.h"

/*************************************************************************************************/
/************************** Table model containing all base calendars ****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

CalendarsModel::CalendarsModel() : QAbstractTableModel()
{
}

/****************************************** destructor *******************************************/

CalendarsModel::~CalendarsModel()
{
  // delete all calendars in model
  foreach( Calendar* c, m_calendars ) delete c;
}

/****************************************** initialise *******************************************/

void CalendarsModel::initialise()
{
  // create initial default calendars
  for ( int cal=0 ; cal<=Calendar::DEFAULT_MAX ; cal++ )
    m_calendars.append( new Calendar(cal) );
}

/******************************************* calendar ********************************************/

Calendar* CalendarsModel::calendar( int n )
{
  // return pointer to n'th calendar
  Q_ASSERT( n >= 0 );
  Q_ASSERT( n < m_calendars.size() );
  return m_calendars.at(n);
}

/********************************************** data *********************************************/

QVariant CalendarsModel::data( const QModelIndex& index, int role  = Qt::DisplayRole ) const
{
  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index column is out of bounds, return an invalid QVariant
  int column = index.column();
  if ( column<0 || column>=m_calendars.size() ) return QVariant();

  return m_calendars.at(column)->data( index.row(), role );
}

/******************************************** setData ********************************************/

bool CalendarsModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;

  // if value hasn't changed, don't proceed
  if ( value == data( index, role ) ) return false;

  // try to set data
  int row = index.row();
  int col = index.column();
  return m_calendars.at( col )->setData( row, value );
}

/****************************************** headerData *******************************************/

QVariant CalendarsModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  if ( orientation == Qt::Vertical )
  {
    if ( section == Calendar::ROW_NAME )        return "Name";
    if ( section == Calendar::ROW_ANCHOR )      return "Anchor";
    if ( section == Calendar::ROW_EXCEPTIONS )  return "Exceptions";
    if ( section == Calendar::ROW_CYCLELENGTH ) return "Cycle";

    return QString("Normal %1").arg( section - Calendar::ROW_NORMAL1 + 1 );
  }
  else
    return m_calendars.at(section)->name();
}

/******************************************** rowCount *******************************************/

int CalendarsModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);

  // table row count is largest cycle length + ROW_NORMAL1
  int max = 0;
  foreach( Calendar* cal, m_calendars )
    if ( max < cal->cycleLength() ) max = cal->cycleLength();

  return max + Calendar::ROW_NORMAL1;
}

/****************************************** columnCount ******************************************/

int CalendarsModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);

  // table column count is number of calendars in m_cals QList
  return m_calendars.size();
}

/********************************************* flags *********************************************/

Qt::ItemFlags CalendarsModel::flags( const QModelIndex& index ) const
{
  // if cell refers to non-existing working period, then cell is not selectable, etc
  int row = index.row();
  int col = index.column();
  if ( row >= m_calendars.at(col)->cycleLength()+Calendar::ROW_NORMAL1 ) return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise all cells are selectable, editable, etc
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

/******************************************* namesList *******************************************/

QStringList  CalendarsModel::namesList() const
{
  // return list of calendar names
  QStringList  list;
  foreach( Calendar* cal, m_calendars )
    list << cal->name();

  return list;
}
