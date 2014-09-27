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

#include "predecessors.h"
#include "plan.h"
#include "tasksmodel.h"
#include "task.h"
#include "calendar.h"

/*************************************************************************************************/
/********************** Task predecessors shows dependencies on other tasks **********************/
/*************************************************************************************************/

const char*  Predecessors::LABEL_FINISH_START  = "FS";
const char*  Predecessors::LABEL_START_START   = "SS";
const char*  Predecessors::LABEL_START_FINISH  = "SF";
const char*  Predecessors::LABEL_FINISH_FINISH = "FF";

/****************************************** constructor ******************************************/

Predecessors::Predecessors()
{
}

/****************************************** constructor ******************************************/

Predecessors::Predecessors( QString text )
{
  // split text into individual predecessors
  m_preds.clear();
  foreach( QString part, text.split( ',', QString::SkipEmptyParts ) )
  {
    // split part into task, predecessor type and lag
    part = part.trimmed();
    int digit = 0;
    while ( part.length() > digit && part.at(digit).isDigit() ) digit++;

    int       task = part.left(digit).toInt();
    char      type = Predecessors::TYPE_DEFAULT;
    TimeSpan  lag  = QString("0");

    part.remove( 0, digit );
    part = part.trimmed();
    if ( !part.isEmpty() )
    {
      if ( part.startsWith( LABEL_FINISH_START,  Qt::CaseInsensitive ) ) type = TYPE_FINISH_START;
      if ( part.startsWith( LABEL_START_START,   Qt::CaseInsensitive ) ) type = TYPE_START_START;
      if ( part.startsWith( LABEL_START_FINISH,  Qt::CaseInsensitive ) ) type = TYPE_START_FINISH;
      if ( part.startsWith( LABEL_FINISH_FINISH, Qt::CaseInsensitive ) ) type = TYPE_FINISH_FINISH;

      part.remove( 0, 2 );
      part = part.trimmed();
      if ( !part.isEmpty() ) lag = part;
    }

    Predecessor  pred;
    pred.task = plan->task( task );
    pred.type = type;
    pred.lag  = lag;
    m_preds.append( pred );
  }
}

/******************************************** toString *******************************************/

QString Predecessors::toString() const
{
  QString str;

  // build up string equivalent
  foreach( Predecessor pred, m_preds )
  {
    str += QString("%1").arg( plan->index( pred.task ) );

    if ( pred.type != TYPE_DEFAULT ||
         pred.lag.number() != 0.0 )
    {
      if ( pred.type == TYPE_FINISH_START )  str += LABEL_FINISH_START;
      if ( pred.type == TYPE_START_FINISH )  str += LABEL_START_FINISH;
      if ( pred.type == TYPE_START_START )   str += LABEL_START_START;
      if ( pred.type == TYPE_FINISH_FINISH ) str += LABEL_FINISH_FINISH;
      if ( pred.lag.number() >  0.0 ) str += "+";
      if ( pred.lag.number() != 0.0 ) str += pred.lag.toString();
    }

    str += ", ";
  }

  // remove final ", " and return string equivalent
  str.chop(2);
  return str;
}

/**************************************** hasPredecessor *****************************************/

bool  Predecessors::hasPredecessor( Task* task ) const
{
  // return true if task is a predecessor
  foreach( Predecessor pred, m_preds )
  {
    if ( pred.task == task ) return true;
    if ( pred.task->hasPredecessor( task ) ) return true;
  }

  return false;
}

/********************************************* clean *********************************************/

QString Predecessors::clean( int thisTaskNum )
{
  // remove forbidden and then return string
  QMutableListIterator<Predecessor> i(m_preds);
  while ( i.hasNext() )
  {
    Predecessor pred = i.next();

    // sub-tasks not allowed to depend on their summaries
    int  predTaskNum = plan->index(pred.task);
    if ( pred.task->isSummary()                 &&
         predTaskNum             <  thisTaskNum &&
         pred.task->summaryEnd() >= thisTaskNum ) i.remove();

    // summaries not allowed to depend on a sub-task
    Task*  thisTask = plan->task(thisTaskNum);
    if ( thisTask->isSummary()                 &&
         thisTaskNum            <  predTaskNum &&
         thisTask->summaryEnd() >= predTaskNum ) i.remove();
  }

  return toString();
}

/********************************************* areOK *********************************************/

bool Predecessors::areOK( int thisTaskNum ) const
{
  // return true if no forbidden predecessors
  foreach( Predecessor pred, m_preds )
  {
    // sub-tasks not allowed to depend on their summaries
    int  predTaskNum = plan->index(pred.task);
    if ( pred.task->isSummary()                 &&
         predTaskNum             <  thisTaskNum &&
         pred.task->summaryEnd() >= thisTaskNum ) return false;

    // summaries not allowed to depend on a sub-task
    Task*  thisTask = plan->task(thisTaskNum);
    if ( thisTask->isSummary()                 &&
         thisTaskNum            <  predTaskNum &&
         thisTask->summaryEnd() >= predTaskNum ) return false;
  }

  return true;
}

/******************************************* hasToStart ******************************************/

bool Predecessors::hasToStart() const
{
  // return true has Finish-To-Start or Start-to-Start predecessor
  foreach( Predecessor pred, m_preds )
  {
    if ( pred.type == TYPE_FINISH_START ) return true;
    if ( pred.type == TYPE_START_START )  return true;
  }

  return false;
}

/****************************************** hasToFinish ******************************************/

bool Predecessors::hasToFinish() const
{
  // return true has Finish-To-Finish or Start-to-Finish predecessor
  foreach( Predecessor pred, m_preds )
  {
    if ( pred.type == TYPE_FINISH_FINISH ) return true;
    if ( pred.type == TYPE_START_FINISH )  return true;
  }

  return false;
}

/******************************************** validate *******************************************/

QString Predecessors::validate( const QString& text, int thisTaskNum )
{
  // split text into individual predecessors
  QString error;
  foreach( QString part, text.split( ',', QString::SkipEmptyParts ) )
  {
    // split part into task, predecessor type and lag
    part = part.trimmed();
    int digit = 0;
    while ( part.length() > digit && part.at( digit ).isDigit() ) digit++;

    // check start is number
    if ( digit == 0 )
    {
      error += QString( "'%1' does not start with a valid task number.\n" ).arg( part );
      continue;
    }

    // check number is non-null task
    int taskNum = part.left( digit ).toInt();
    if ( taskNum >= plan->tasks()->rowCount( QModelIndex() ) ||
         plan->task( taskNum )->isNull() )
    {
      error += QString( "'%1' is a null task.\n" ).arg( taskNum );
      continue;
    }

    // check number is not sub-task if this task is a summary
    if ( plan->task( thisTaskNum )->isSummary() &&
         taskNum >  thisTaskNum &&
         taskNum <= plan->task( thisTaskNum )->summaryEnd() )
    {
      error += QString( "'%1' is a sub-task of this summary.\n" ).arg( taskNum );
      continue;
    }

    // check number is not summary containing this task
    if ( plan->task( taskNum )->isSummary() &&
         thisTaskNum > taskNum &&
         thisTaskNum <= plan->task( taskNum )->summaryEnd() )
    {
      error += QString( "'%1' is a summary containing this sub-task.\n" ).arg( taskNum );
      continue;
    }

    // check number is not this task
    if ( taskNum == thisTaskNum )
    {
      error += QString( "'%1' is a reference to this task.\n" ).arg( taskNum );
      continue;
    }

    // check number is does not cause circular reference
    if ( plan->task( taskNum )->hasPredecessor( plan->task( thisTaskNum ) ) )
    {
      error += QString( "'%1' gives a circular reference to this task.\n" ).arg( taskNum );
      continue;
    }

    // check nothing remains or is a valid type
    part.remove( 0, digit );
    part = part.trimmed();
    if ( part.isEmpty() ) continue;
    if ( !part.startsWith( LABEL_FINISH_START,  Qt::CaseInsensitive ) &&
         !part.startsWith( LABEL_START_START,   Qt::CaseInsensitive ) &&
         !part.startsWith( LABEL_START_FINISH,  Qt::CaseInsensitive ) &&
         !part.startsWith( LABEL_FINISH_FINISH, Qt::CaseInsensitive ) )
    {
      error += QString( "'%1' is not a valid dependency type.\n" ).arg( part );
      continue;
    }

    // check nothing remains or is valid time-span
    part.remove( 0, 2 );
    part = part.trimmed();
    if ( part.isEmpty() ) continue;
    TimeSpan ts = part;
    if ( !ts.isValid() )
      error += QString( "'%1' is not a valid time span.\n" ).arg( part );
  }

  // remove final '\n' and return validation error text
  error.chop(1);
  return error;
}

/********************************************* start *********************************************/

DateTime  Predecessors::start() const
{
  // return task start based on predecessors
  DateTime  start = XDateTime::MIN_DATETIME;
  foreach( Predecessor pred, m_preds )
  {
    if ( pred.type == TYPE_FINISH_START )
    {
      DateTime check = plan->calendar()->addTimeSpan( pred.task->end(), pred.lag );
      if ( check > start ) start = check;
    }

    if ( pred.type == TYPE_START_START )
    {
      DateTime check = plan->calendar()->addTimeSpan( pred.task->start(), pred.lag );
      if ( check > start ) start = check;
    }
  }

  return start;
}

/********************************************** end **********************************************/

DateTime  Predecessors::end() const
{
  // return task end based on predecessors
  DateTime  end = XDateTime::MAX_DATETIME;
  foreach( Predecessor pred, m_preds )
  {
    if ( pred.type == TYPE_FINISH_FINISH )
    {
      DateTime check = plan->calendar()->addTimeSpan( pred.task->end(), pred.lag );
      if ( check < end ) end = check;
    }

    if ( pred.type == TYPE_START_FINISH )
    {
      DateTime check = plan->calendar()->addTimeSpan( pred.task->start(), pred.lag );
      if ( check < end ) end = check;
    }
  }

  return end;
}
