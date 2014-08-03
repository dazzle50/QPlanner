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

#ifndef GANTTCHART_H
#define GANTTCHART_H

class Calendar;
class QTableView;
class PlanModel;

#include <QWidget>

#include "model/datetime.h"

/*************************************************************************************************/
/*********************** GanttChart provides a view of the plan gantt chart **********************/
/*************************************************************************************************/

class GanttChart : public QWidget
{
  Q_OBJECT
public:
  GanttChart( QWidget* );                          // constructor
  int  chartWidth();                               // return width in pixels of the gantt
  void setStart( DateTime );                       // set start date-time for the gantt
  void setEnd( DateTime );                         // set end date-time for the gantt
  void setMinsPerPixel( double );                  // set minutes per pixel scale
  void setTable( QTableView* );                    // set associated table view & plan for the gantt

public slots:
  void slotTasksScrolled( int );                   // receive vertical scroll events from table
  void slotTaskHeightChanged( int, int, int );     // receive row height change events from table
  void slotTaskMoved( int, int, int );             // receive task row moved events from table
  void slotTasksChanged();                         // receive task change events from model

protected:
  void paintEvent( QPaintEvent* );                 // draw gantt contents

private:
  DateTime       m_start;                          // start date-time for GanttChart
  DateTime       m_end;                            // start date-time for GanttChart
  double         m_minsPP;                         // minutes per pixel
  QTableView*    m_table;                          // table view associated with the gantt

  void shadeNonWorkingDays( QPainter*,
         int, int, int, int );                     // shade gantt chart non working days
  void drawTasks( QPainter*, int, int );           // draw gantt tasks
  void drawDependencies( QPainter*, int, int );    // draw gantt tasks dependencies
};

#endif // GANTTCHART_H
