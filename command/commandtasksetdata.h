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

#ifndef COMMANDTASKSETDATA_H
#define COMMANDTASKSETDATA_H

#include <QUndoCommand>

#include "model/plan.h"
#include "model/tasksmodel.h"
#include "model/task.h"

/*************************************************************************************************/
/************************* Command for setting Task data via QUndoStack **************************/
/*************************************************************************************************/

class CommandTaskSetData : public QUndoCommand
{
public:
  CommandTaskSetData( const QModelIndex& index, const QVariant& value )
  {
    // set private variables for new and old values
    m_row      = index.row();
    m_column   = index.column();
    m_old_task = *( plan->task( m_row ) );
    m_value    = value;

    // construct command description
    setText( QString("Task %1 %2 = %3")
             .arg( m_row+1 )
             .arg( Task::headerData( m_column ).toString() )
             .arg( value.toString() ) );
  }

  void  redo()
  {
    // update task with new value
    bool wasNull = plan->task( m_row )->isNull();
    plan->task( m_row )->setData( m_column, m_value );

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->tasks()->emitDataChangedRow( m_row );
    if ( m_column != Task::SECTION_TITLE &&
         m_column != Task::SECTION_COMMENT &&
         m_column != Task::SECTION_DEADLINE ) plan->schedule();

    // update plan tab to reflect increase in number of tasks
    if ( wasNull ) plan->signalPlanUpdated();
  }

  void  undo()
  {
    // revert task back to old values
    *( plan->task( m_row ) ) = m_old_task;

    // ensure table row is refreshed, and plan re-scheduled if needed
    plan->tasks()->emitDataChangedRow( m_row );
    if ( m_column != Task::SECTION_TITLE &&
         m_column != Task::SECTION_COMMENT &&
         m_column != Task::SECTION_DEADLINE ) plan->schedule();

    // update plan tab to reflect decrease in number of tasks & re-schedule
    if ( plan->task( m_row )->isNull() )
    {
      plan->signalPlanUpdated();
      plan->schedule();
    }
  }

private:
  Task      m_old_task;
  int       m_row;
  int       m_column;
  QVariant  m_value;
};

#endif // COMMANDTASKSETDATA_H
