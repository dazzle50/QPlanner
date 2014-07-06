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

#ifndef GANTTVIEW_H
#define GANTTVIEW_H

#include <QScrollArea>

#include "model/datetime.h"

class GanttChart;
class GanttScale;
class QTableView;

/*************************************************************************************************/
/**************************** GanttView shows tasks in a gantt format ****************************/
/*************************************************************************************************/

class GanttView : public QScrollArea
{
  Q_OBJECT
public:
  GanttView( QWidget* );                // constructor

  void    createGantt( QWidget* );      // sets the table associated with the gantt
  void    setTable( QTableView* );      // sets the table associated with the gantt
  int     scaleHeight();                // return scale (upper+lower) height

  void    setStart( DateTime );         // set gantt start date-time
  void    setEnd( DateTime );           // set gantt end date-time
  void    setMinsPP( double );          // set gantt minutes per pixel
  void    setWidth();                   // ensure view width is never less than chart width

public slots:
  void    contextMenu( QPoint );        // slot to receive context menu signals
  void    slotZoomIn();                 // slot to receive zoom in action signal
  void    slotZoomOut();                // slot to receive zoom out action signal
  void    slotZoomFit();                // slot to receive zoom fit action signal

protected:
  void resizeEvent( QResizeEvent* );    // resize the GanttView contents

private:
  QWidget*        m_view;               // widget to display the gantt scales + chart
  GanttScale*     m_upperScale;         // gantt upper scale view
  GanttScale*     m_lowerScale;         // gantt lower scale view
  GanttChart*     m_chart;              // gantt chart view
  double          m_minsPP;             // minutes per pixel
  DateTime        m_start;              // start date-time
  DateTime        m_end;                // end date-time
};

#endif // GANTTVIEW_H
