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

#include "tasksmodel.h"
#include "task.h"
#include "command/commandtasksetdata.h"

/*************************************************************************************************/
/**************************** Table model containing all plan tasks ******************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TasksModel::TasksModel() : QAbstractTableModel()
{
  // create plan summary task, also known as task zero, usually hidden
  m_tasks.append( new Task(true) );
}

/****************************************** destructor *******************************************/

TasksModel::~TasksModel()
{
  // delete all tasks in model
  foreach( Task* t, m_tasks ) delete t;
}

/****************************************** initialise *******************************************/

void TasksModel::initialise()
{
  // create initial plan blank tasks
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
  m_tasks.append( new Task() );
}

/********************************************* task **********************************************/

Task* TasksModel::task( int n )
{
  // return pointer to n'th task, checking n is in range first
  Q_ASSERT( n >= 0 );
  Q_ASSERT( n < m_tasks.size() );
  return m_tasks.at(n);
}

/******************************************** number *********************************************/

int TasksModel::number()
{
  // return number of non-null tasks in plan
  int  count = 0;
  foreach( Task* t, m_tasks )
    if ( !t->isNull() ) count++;

  return count;
}

/******************************************* schedule ********************************************/

void TasksModel::schedule()
{
  // re-schedule tasks - first construct list of tasks in correct order
  qDebug("TasksModel::schedule()");
}

/***************************************** planBeginning *****************************************/

DateTime TasksModel::planBeginning()
{
  // return start of earliest starting task
  DateTime  first = XDateTime::NULL_DATETIME;
  foreach( Task* t, m_tasks )
  {
    if ( !t->isNull() && first == XDateTime::NULL_DATETIME ) first = t->start();
    if ( !t->isNull() && t->start() < first ) first = t->start();
  }

  return first;
}

/******************************************** planEnd ********************************************/

DateTime TasksModel::planEnd()
{
  // return finish of latest finishing task
  DateTime  end = XDateTime::NULL_DATETIME;
  foreach( Task* t, m_tasks )
  {
    if ( !t->isNull() && end == XDateTime::NULL_DATETIME ) end = t->end();
    if ( !t->isNull() && t->end() > end ) end = t->end();
  }

  return end;
}

/******************************************** rowCount *******************************************/

int TasksModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return m_tasks.size();
}

/****************************************** columnCount ******************************************/

int TasksModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED(parent);
  return Task::SECTION_MAXIMUM + 1;
}

/********************************************** data *********************************************/

QVariant TasksModel::data( const QModelIndex& index, int role  = Qt::DisplayRole ) const
{
  // if index matches override index, return override value
  if ( index == m_overrideIndex )
    if ( role == Qt::DisplayRole || role == Qt::EditRole )
      return m_overrideValue;

  // if index is not valid, return an invalid QVariant
  if ( !index.isValid() ) return QVariant();

  // if index row is out of bounds, return an invalid QVariant
  int row = index.row();
  if ( row<0 || row>=m_tasks.size() ) return QVariant();

  int col = index.column();
  if ( role == Qt::DisplayRole )         return m_tasks.at(row)->dataDisplayRole( col );
  if ( role == Qt::ToolTipRole )         return m_tasks.at(row)->dataToolTipRole( col );
  if ( role == Qt::EditRole )            return m_tasks.at(row)->dataEditRole( col );
  if ( role == Qt::TextAlignmentRole )   return m_tasks.at(row)->dataTextAlignmentRole( col );
  if ( role == Qt::BackgroundColorRole ) return m_tasks.at(row)->dataBackgroundColorRole( col );
  if ( role == Qt::FontRole )            return m_tasks.at(row)->dataFontRole( col );

  // otherwise return an invalid QVariant
  return QVariant();
}

/******************************************** setData ********************************************/

bool TasksModel::setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
{
  // if index is not valid, return false - can't set data
  if ( !index.isValid() ) return false;

  // if role is not Qt::EditRole, return false - can't set data
  if ( role != Qt::EditRole ) return false;

  // if value hasn't changed, don't proceed
  if ( value == data( index, role ) ) return false;

  // set data via undo/redo command
  //plan->undostack()->push( new CommandTaskSetData( index, value ) );
}

/****************************************** headerData *******************************************/

QVariant TasksModel::headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
{
  // if role is not DisplayRole, return an invalid QVariant
  if ( role != Qt::DisplayRole ) return QVariant();

  // if horizontal header, return task header, otherwise row section number
  if ( orientation == Qt::Horizontal ) return Task::headerData( section );
  return QString("%1").arg( section );
}

/********************************************* flags *********************************************/

Qt::ItemFlags TasksModel::flags( const QModelIndex& index ) const
{
  // if task is null (blank), then only title is editable, others are not
  int row = index.row();
  int col = index.column();
  if ( m_tasks.at(row)->isNull() &&  col != Task::SECTION_TITLE )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // if task is summary, then some cells not editable
  if ( m_tasks.at(row)->isSummary() &&
       col != Task::SECTION_TITLE &&
       col != Task::SECTION_PREDS &&
       col != Task::SECTION_DEADLINE &&
       col != Task::SECTION_RES &&
       col != Task::SECTION_PRIORITY &&
       col != Task::SECTION_COMMENT )
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  // otherwise cell is enabled, selectable, editable
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
