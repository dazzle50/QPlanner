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

#ifndef COMMANDTASKINDENT_H
#define COMMANDTASKINDENT_H

#include <QUndoCommand>
#include <QSet>

#include "model/plan.h"
#include "model/tasksmodel.h"
#include "model/task.h"

/*************************************************************************************************/
/************************** Command for indenting Tasks via QUndoStack ***************************/
/*************************************************************************************************/

class CommandTaskIndent : public QUndoCommand
{
public:
  CommandTaskIndent( QSet<int> rows )
  {
    // set private variables
    m_rows  = rows;

    // construct command description
    setText( "Indent" );
  }

  void  redo()
  {
    // indent tasks
    foreach( int row, m_rows )
      plan->task(row)->setIndent( plan->task(row)->indent() + 1 );

    // remove any forbidden predecessors
    plan->tasks()->setSummaries();
    m_old_preds.clear();
    for( int t = 1 ; t < plan->tasks()->rowCount() ; t++ )
    {
      Task*  task = plan->task(t);
      QString  old_preds = task->predecessorsString();
      QString  new_preds = task->predecessorsClean();
      if ( old_preds != new_preds ) m_old_preds.insert( t, old_preds );
    }

    plan->schedule();
  }

  void  undo()
  {
    // revert by outdenting tasks
    foreach( int row, m_rows )
      plan->task(row)->setIndent( plan->task(row)->indent() - 1 );

    // revert any predecessor changes
    QHashIterator<int, QString> i(m_old_preds);
    while( i.hasNext() )
    {
        i.next();
        plan->task( i.key() )->setPredecessors( i.value() );
    }

    plan->tasks()->setSummaries();
    plan->schedule();
  }

private:
  QSet<int>            m_rows;
  QHash<int, QString>  m_old_preds;
};

#endif // COMMANDTASKINDENT_H
