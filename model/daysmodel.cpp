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

#include "daysmodel.h"
#include "day.h"
#include "plan.h"
#include "command/commanddaysetdata.h"

/*************************************************************************************************/
/************************ Table model containing all calendar day types **************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

DaysModel::DaysModel() : QAbstractTableModel()
{
}

/****************************************** destructor *******************************************/

DaysModel::~DaysModel()
{
  // delete all day types in model
  foreach( Day* d, m_days ) delete d;
}

/****************************************** initialise *******************************************/

void DaysModel::initialise()
{
  // create initial default day types
  for ( int day=0 ; day<=Day::DEFAULT_MAX ; day++ )
    m_days.append( new Day(day) );
}

/********************************************* day ***********************************************/

Day* DaysModel::day( int n )
{
  // return pointer to n'th day type
  Q_ASSERT( n >= 0 );
  Q_ASSERT( n < m_days.size() );
  return m_days.at(n);
}

/******************************************** rowCount *******************************************/

int DaysModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_days.size();
}

/****************************************** columnCount ******************************************/

int DaysModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);

  // table column count is max number of periods * 2 + SECTION_START
  int max = 0;
  foreach( Day* day, m_days )
    if ( max < day->periods() ) max = day->periods();

  return max*2 + Day::SECTION_START;
}

/********************************************** data *********************************************/

QVariant DaysModel::data( const QModelIndex& index, int role  = Qt::DisplayRole ) const
{
  // if index matches override index, return override value
  if ( index == m_overrideIndex )
    if ( role == Qt::DisplayRole || role == Qt::EditRole )
      return m_overrideValue;

  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_days.size() ) return QVariant();

  return m_days.at(row)->data( index.column(), role );
}

/******************************************** setData ********************************************/

bool DaysModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;

  // if value hasn't changed, don't proceed
  if ( value.toString() == data( index, role ).toString() ) return false;

  // set data via undo/redo command
  plan->undostack()->push( new CommandDaySetData( index, value ) );
  return true;
}

/****************************************** headerData *******************************************/

QVariant DaysModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return resource header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Day::headerData( section );
  return QString("%1").arg( section+1 );
}

/********************************************* flags *********************************************/

Qt::ItemFlags DaysModel::flags( const QModelIndex& index ) const
{
  // if cell refers to non-existing working period, then cell is not selectable, etc
  int row = index.row();
  int col = index.column();
  if ( col >= m_days.at(row)->periods()*2+Day::SECTION_START ) return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise all cells are selectable, editable, etc
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

/******************************************* namesList *******************************************/

QStringList  DaysModel::namesList() const
{
  // return list of day names
  QStringList  list;
  foreach( Day* d, m_days )
    list << d->name();

  return list;
}

/**************************************** nameIsDuplicate ****************************************/

bool DaysModel::nameIsDuplicate( const QString& name, int row )
{
  // if name matches a day type name on any other row return true
  for ( int d = 0 ; d < m_days.size() ; d++ )
    if ( d != row && name == m_days.at(d)->name() ) return true;

  // no match found, so return false
  return false;
}

/************************************** emitDataChangedRow ***************************************/

void DaysModel::emitDataChangedRow( int row )
{
  // emit data changed signal for row
  emit dataChanged( QAbstractTableModel::index( row, 0 ),
                    QAbstractTableModel::index( row, columnCount() ) );
}

/**************************************** emitNameChanged ****************************************/

void DaysModel::emitNameChanged() const
{
  // emit name changed signal
  emit nameChanged();
}

/****************************************** beginInsert ******************************************/

void DaysModel::beginInsert( int num )
{
  // begin process of appending number of columns to table model
  beginInsertColumns( QModelIndex(), columnCount(), columnCount()+num-1 );
}

/****************************************** beginRemove ******************************************/

void DaysModel::beginRemove( int num )
{
  // begin process of removing number of columns to table model
  beginRemoveColumns( QModelIndex(), columnCount()-num, columnCount()-1 );
}
