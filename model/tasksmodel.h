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

#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractTableModel>
#include <QSet>

#include "datetime.h"

class Task;
class QXmlStreamWriter;
class QXmlStreamReader;

/*************************************************************************************************/
/**************************** Table model containing all plan tasks ******************************/
/*************************************************************************************************/

class TasksModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  TasksModel();                                                   // constructor
  ~TasksModel();                                                  // destructor

  void           initialise();                                    // create initial default contents
  DateTime       planBeginning();                                 // return start of earliest starting task
  DateTime       planEnd();                                       // return finish of latest finishing task
  int            number();                                        // return number of non-null tasks in plan
  void           schedule();                                      // re-schedule tasks
  void           saveToStream( QXmlStreamWriter* );               // write tasks data to xml stream
  void           loadFromStream( QXmlStreamReader* );             // load tasks data from xml stream

  Task*          task( int n );                                   // return pointer to n'th task
  int            index( Task* t ) { return m_tasks.indexOf(t); }  // return index of task, or -1

  void           emitDataChangedRow( int );                       // emit data changed signal for row
  bool           canIndent( int );                                // return true if task can be indented
  bool           canOutdent( int );                               // return true if task can be outdented
  bool           indentRows( QSet<int> );                         // indent selected rows
  bool           outdentRows( QSet<int> );                        // outdent selected rows
  Task*          nonNullTaskAbove( Task* );                       // returns task ptr or nullptr if none
  void           setSummaries();                                  // recalc summaries for all tasks
  void           setOverride( QModelIndex i, QVariant v )
                   { m_overrideIndex = i; m_overrideValue = v; }  // set model override values

  /********************* methods to support QAbstractTableModel ************************/

  int            rowCount( const QModelIndex& parent = QModelIndex() ) const;     // implement virtual row count
  int            columnCount( const QModelIndex& parent = QModelIndex() ) const;  // implement virtual column count
  QVariant       data( const QModelIndex&, int ) const;                           // implement virtual return data
  bool           setData( const QModelIndex&, const QVariant&, int );             // implement virtual set data
  QVariant       headerData( int, Qt::Orientation, int ) const;                   // implement virtual header data
  Qt::ItemFlags  flags( const QModelIndex& ) const;                               // implement virtual return flags

signals:
  void           ganttChanged();                                  // signal tasks could have changed so redraw gantt chart
  void           editCell( const QModelIndex&,
                           const QString& ) const;                // signal that cell editing needs to continue
private:
  QList<Task*>    m_tasks;             // list of tasks in plan

  QModelIndex     m_overrideIndex;     // with value can override model for edits in progress
  QVariant        m_overrideValue;     // with index can override model for edits in progress
};

#endif // TASKSMODEL_H
