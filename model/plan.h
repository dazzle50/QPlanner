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

#ifndef PLAN_H
#define PLAN_H

#include <QString>
#include <QDateTime>
#include <QColor>

class TasksModel;
class ResourcesModel;
class CalendarsModel;
class DaysModel;
class QUndoStack;

class Task;
class Resource;
class Calendar;
class Day;

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

class Plan : public QObject
{
  Q_OBJECT
public:
  Plan();                       // constructor
  ~Plan();                      // destructor

  void             initialise();                                    // create initial plan default contents
  QUndoStack*      undostack() { return m_undostack; }              // return undo stack pointer

private:
  QUndoStack*      m_undostack;         // undo stack of plan editing

};

extern Plan*        plan;

#endif // PLAN_H
