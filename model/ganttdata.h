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

#ifndef GANTTDATA_H
#define GANTTDATA_H

#include <QVector>

#include "datetime.h"

class QPainter;

/*************************************************************************************************/
/************************** Data to support the drawing of a gantt task **************************/
/*************************************************************************************************/

class GanttData
{
public:
  GanttData();                                           // constructor

  void        setMilestone( DateTime );                  // set task data to milestone
  void        setTask( DateTime, DateTime );             // set task data to simple gantt bar
  void        setSummary( DateTime, DateTime );          // set task data to summary gantt bar

  DateTime    start() const;                             // return task gantt start date-time
  DateTime    end() const;                               // return task gantt start date-time
  int         startX( DateTime, double ) const;          // return task gantt start x coordinate
  int         endX( DateTime, double ) const;            // return task gantt end x coordinate
  int         x( DateTime, DateTime, double ) const;     // return x-coord for dt given start & minspp
  int         taskBarHeight( QPainter* ) const;          // max height of task on gantt
  int         milestoneHeight( QPainter* ) const;        // max height of milestones on gantt

  void        drawTask( QPainter*, int, DateTime, double, QString );  // draw task data on gantt
  void        drawTaskBar( QPainter*, int, DateTime, double );        // draw task bar on gantt
  void        drawMilestone( QPainter*, int, DateTime, double );      // draw milestone on gantt
  void        drawSummary( QPainter*, int, DateTime, double );        // draw milestone on gantt
  void        verticalArrow( QPainter*, int, int, int );              // draw vertical arrow for dependency links
  void        horizontalArrow( QPainter*, int, int, int );            // draw horizontal arrow for dependency links

  void        drawDependencyFS( QPainter*, int, int, int,
                                DateTime, double );      // draw dependency FINISH_START line on gantt
  void        drawDependencySF( QPainter*, int, int, int,
                                DateTime, double );      // draw dependency START_FINISH line on gantt
  void        drawDependencySS( QPainter*, int, int, int,
                                DateTime, double );      // draw dependency START_START line on gantt
  void        drawDependencyFF( QPainter*, int, int, int,
                                DateTime, double );      // draw dependency FINISH_FINISH line on gantt

private:
  DateTime             m_start;    // start of gantt task
  QVector<DateTime>    m_end;      // span end
  QVector<float>       m_value;    // span value (-ve for summaries, +ve for tasks)
};

#endif // GANTTDATA_H
