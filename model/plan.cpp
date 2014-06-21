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

#include "plan.h"

#include <QUndoStack>
#include <QXmlStreamWriter>
#include <QFileInfo>

/*************************************************************************************************/
/************************** Holds the complete data model for the plan ***************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

Plan::Plan()
{
  // create blank models and set private variables
  //m_days       = new DaysModel();
  //m_calendars  = new CalendarsModel();
  //m_resources  = new ResourcesModel();
  //m_tasks      = new TasksModel();
  m_undostack  = new QUndoStack();

  //m_datetime_format = "ddd dd/MM/yyyy hh:mm:ss";
  //m_calendar        = nullptr;
  //stretchTasks      = true;
}

/****************************************** destructor *******************************************/

Plan::~Plan()
{
  // delete models and undostack
  //delete m_tasks;
  //delete m_resources;
  //delete m_calendars;
  //delete m_days;
  delete m_undostack;
}

/****************************************** initialise *******************************************/

void  Plan::initialise()
{
  // initialise the models
  //m_days->initialise();
  //m_calendars->initialise();

  //m_calendar = calendar( Calendar::DEFAULT_CALENDAR );
  //m_start    = m_calendar->workUp( QDateTime( QDate::currentDate(), QTime(0,0,0) ) );

  //m_resources->initialise();
  //m_tasks->initialise();
}

