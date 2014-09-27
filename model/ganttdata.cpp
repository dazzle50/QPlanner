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

#include "ganttdata.h"
#include "plan.h"
#include "task.h"
#include "calendar.h"

#include <QPainter>

/*************************************************************************************************/
/**************************** Single day type used in plan calendars *****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

GanttData::GanttData()
{
}

/******************************************** setTask ********************************************/

void GanttData::setTask( DateTime start, DateTime end )
{
  if ( start == end )
    setMilestone( start );
  else
  {
    // set task data to simple gantt bar
    m_end.resize(1);
    m_value.resize(1);
    m_start    = start;
    m_end[0]   = end;
    m_value[0] = 1.0;
  }
}

/****************************************** setMilestone *****************************************/

void GanttData::setMilestone( DateTime milestone )
{
  // set task data to milestone
  m_start = milestone;
  m_end.resize(0);
  m_value.resize(0);
}

/******************************************* setSummary ******************************************/

void GanttData::setSummary( DateTime start, DateTime end )
{
  // set task data to summary gantt bar
  m_start    = start;
  m_end.resize(1);
  m_value.resize(1);
  m_end[0]   = end;
  m_value[0] = -1.0;
}

/********************************************* start *********************************************/

DateTime GanttData::start() const
{
  // return task gantt row start date-time
  return m_start;
}

/********************************************** end **********************************************/

DateTime GanttData::end() const
{
  // return task gantt row end date-time
  if ( m_end.size() == 0 ) return m_start;
  return m_end.last();
}

/********************************************* startX ********************************************/

int GanttData::startX( DateTime start, double minsPP ) const
{
  // return task gantt row start x coordinate
  return x( m_start, start, minsPP );
}

/********************************************** endX *********************************************/

int GanttData::endX( DateTime start, double minsPP ) const
{
  // return task gantt row end x coordinate
  return x( end(), start, minsPP );
}

/*********************************************** x ***********************************************/

int GanttData::x( DateTime dt, DateTime start, double minsPP ) const
{
  // return x-coord for stretched date-time given start & minspp
  DateTime stretch = plan->stretch( dt );
  if ( stretch > start ) return ( stretch - start ) / minsPP;
  return ( start - stretch ) / -minsPP;
}

/********************************************* height ********************************************/

int GanttData::height( QPainter* p )
{
  // return max height of task on gantt
  return p->fontMetrics().lineSpacing() * 4 / 5;
}

/***************************************** verticalArrow ******************************************/

void GanttData::verticalArrow( QPainter* p, int x, int y, int size )
{
  // draw vertical arrow for dependency links
  if ( size < 0 ) y++;
  QPoint points[3];

  points[0] = QPoint( x, y );
  points[1] = QPoint( x-size, y-size );
  points[2] = QPoint( x+size, y-size );

  p->setPen( Qt::NoPen );
  p->setBrush( Qt::darkGray );
  p->drawConvexPolygon( points, 3 );
}

/**************************************** horizontalArrow ****************************************/

void GanttData::horizontalArrow( QPainter* p, int x, int y, int size )
{
  // draw horizontal arrow for dependency links
  QPoint points[3];

  points[0] = QPoint( x, y );
  if ( size > 0 )
  {
    points[1] = QPoint( x-size, y-size+1 );
    points[2] = QPoint( x-size, y+size );
  }
  else
  {
    points[1] = QPoint( x-size, y-size );
    points[2] = QPoint( x-size, y+size+1 );
  }

  p->setPen( Qt::NoPen );
  p->setBrush( Qt::darkGray );
  p->drawConvexPolygon( points, 3 );
}

/**************************************** drawDependencyFS ***************************************/

void GanttData::drawDependencyFS( QPainter* p, int thisY, int otherY, int num,
                                  DateTime start, double minsPP )
{
  // draw dependency FINISH_START line on gantt
  int  otherX = plan->task(num)->ganttData()->endX( start, minsPP );
  int  thisX  = startX( start, minsPP );
  int  sign   = thisY > otherY ? 1 : -1;
  int  h      = height( p );

  // if other is milestone adjust for size, using same size formula as used in drawMilestone
  if ( plan->task(num)->isMilestone() )
  {
    int size = 1 + h / 3;
    otherX += size - 1 ;
  }

  // if this is milestone adjust for size, using same size formula as used in drawMilestone
  if ( m_value.isEmpty() )
  {
    int size = 1 + h / 3;
    thisX -= size - 1 ;
  }

  p->setPen( Qt::darkGray );

  if ( thisX == otherX || thisX == otherX+1 || thisX == otherX+2 )
  {
    p->drawLine( otherX+1, otherY,      otherX+2, otherY );
    p->drawLine( otherX+3, otherY+sign, otherX+3, thisY-sign*(h/2+2) );
    verticalArrow( p, otherX+3, thisY-sign*h/2, (sign*h*2)/5 );
  }
  else if ( thisX > otherX )
  {
    p->drawLine( otherX+1, otherY,      thisX-1, otherY );
    p->drawLine( thisX,    otherY+sign, thisX,   thisY-sign*(h/2+2) );
    verticalArrow( p, thisX, thisY-sign*h/2, (sign*h*2)/5 );
  }
  else // if ( thisX < otherX )
  {
    p->drawLine( otherX+1, otherY,      otherX+2, otherY );
    p->drawLine( otherX+3, otherY+sign, otherX+3, otherY+sign*(h/2+2) );
    p->drawLine( otherX+2, otherY+sign*(h/2+3), thisX-6, otherY+sign*(h/2+3));
    p->drawLine( thisX-7, otherY+sign*(h/2+4), thisX-7, thisY-sign );
    p->drawLine( thisX-6, thisY, thisX-2, thisY );
    horizontalArrow( p, thisX, thisY, (h*2)/5 );
  }
}

/**************************************** drawDependencySF ***************************************/

void GanttData::drawDependencySF( QPainter* p, int thisY, int otherY, int num,
                                  DateTime start, double minsPP )
{
  // draw dependency START_FINISH line on gantt
  int  otherX = plan->task(num)->ganttData()->startX( start, minsPP );
  int  thisX  = endX( start, minsPP );
  int  sign   = thisY > otherY ? 1 : -1;
  int  h      = height( p );

  p->setPen( Qt::darkGray );

  if ( thisX == otherX  || thisX == otherX-1 )
  {
    p->drawLine( otherX-1, otherY,      otherX-2, otherY );
    p->drawLine( otherX-3, otherY+sign, otherX-3, thisY-sign*(h/2+2) );
    verticalArrow( p, otherX-3, thisY-sign*h/2, (sign*h*2)/5 );
  }
  else if ( thisX > otherX )
  {
    p->drawLine( otherX-1, otherY,      thisX+1, otherY );
    p->drawLine( thisX,    otherY+sign, thisX,   thisY-sign*(h/2+2) );
    verticalArrow( p, thisX, thisY-sign*h/2, (sign*h*2)/5 );
  }
  else // if ( thisX < otherX )
  {
    p->drawLine( otherX-1, otherY,      otherX-2, otherY );
    p->drawLine( otherX-3, otherY+sign, otherX-3, otherY+sign*(h/2+2) );
    p->drawLine( otherX-2, otherY+sign*(h/2+3), thisX+6, otherY+sign*(h/2+3));
    p->drawLine( thisX+7, otherY+sign*(h/2+4), thisX+7, thisY-sign );
    p->drawLine( thisX+6, thisY, thisX+2, thisY );
    horizontalArrow( p, thisX, thisY, (h*2)/5 );
  }
}

/**************************************** drawDependencySS ***************************************/

void GanttData::drawDependencySS( QPainter* p, int thisY, int otherY, int num,
                                  DateTime start, double minsPP )
{
  // draw dependency START_START line on gantt
  qDebug("GanttData::drawDependencySS  thisY=%i  otherY=%i  num=%i",thisY,otherY,num);
  Q_UNUSED(p)
  Q_UNUSED(start)
  Q_UNUSED(minsPP)
}

/**************************************** drawDependencyFF ***************************************/

void GanttData::drawDependencyFF( QPainter* p, int thisY, int otherY, int num,
                                  DateTime start, double minsPP )
{
  // draw dependency FINISH_FINISH line on gantt
  qDebug("GanttData::drawDependencyFF  thisY=%i  otherY=%i  num=%i",thisY,otherY,num);
  Q_UNUSED(p)
  Q_UNUSED(start)
  Q_UNUSED(minsPP)
}

/******************************************** drawTask *******************************************/

void GanttData::drawTask( QPainter* p, int y, DateTime start, double minsPP, QString label )
{
  // if gantt data start date-time is not valid do not draw anything
  if ( m_start == XDateTime::NULL_DATETIME ) return;

  // if no data then draw milestone, otherwise summary or task bar
  if ( m_value.isEmpty() )     drawMilestone( p, y, start, minsPP );
  else if ( m_value[0] < 0.0 ) drawSummary( p, y, start, minsPP );
  else                         drawTaskBar( p, y, start, minsPP );

  // add label to end
  if ( !label.isEmpty() )
  {
    int x     = endX( start, minsPP );
    int flags = Qt::AlignLeft + Qt::AlignVCenter + Qt::TextSingleLine + Qt::TextDontClip;
    p->setPen( Qt::black );
    p->drawText( x, y-1, 0, 0, flags, "   "+label );
  }
}

/***************************************** drawMilestone *****************************************/

void GanttData::drawMilestone( QPainter* p, int y, DateTime start, double minsPP )
{
  // calc x position of milestone & height
  int x = startX( start, minsPP );
  int h = 1 + height( p ) / 3;

  // populate points array to draw the milestone
  QPoint points[4];
  points[0] = QPoint( x  , y-h );
  points[1] = QPoint( x+h, y   );
  points[2] = QPoint( x  , y+h );
  points[3] = QPoint( x-h, y   );

  // draw the milestone
  p->setPen( Qt::NoPen );
  p->setBrush( Qt::black );
  p->drawConvexPolygon( points, 4 );
}

/******************************************* drawSummary *****************************************/

void GanttData::drawSummary( QPainter* p, int y, DateTime start, double minsPP )
{
  // calc x positions of summary & height
  int xs = startX( start, minsPP );
  int xe = endX( start, minsPP );
  int h  = 1 + height( p ) / 3;
  int w  = h;
  if ( w > xe - xs ) w = xe - xs;

  // populate points array to draw the summary
  QPoint points[6];
  points[0] = QPoint( xs+w, y   );
  points[1] = QPoint( xs  , y+h );
  points[2] = QPoint( xs  , y-h );
  points[3] = QPoint( xe+1, y-h );
  points[4] = QPoint( xe+1, y+h );
  points[5] = QPoint( xe-w, y   );

  // draw the milestone
  p->setPen( Qt::NoPen );
  p->setBrush( Qt::black );
  p->drawPolygon( points, 6 );
  p->drawPolygon( points, 3 );
}

/******************************************* drawTaskBar *****************************************/

void GanttData::drawTaskBar( QPainter* p, int ty, DateTime start, double minsPP )
{
  // determine scale to draw offset
  float scale = 0.0;
  for( int period=0 ; period<m_value.size() ; period++ )
    if ( m_value[period] > scale ) scale = m_value[period];
  scale *= height( p ) / 2;

  // set pen and fill colours
  p->setPen( QColor( Qt::blue ) );
  QBrush fill = QColor( Qt::yellow );

  // calc start position of task bar
  int tx     = startX( start, minsPP );
  int offset = int( m_value[0] * scale );

  // draw front edge
  p->drawLine( tx, ty+offset, tx, ty-offset );

  // for each period within task bar draw next section
  int newX, newOffset;
  for( int period=1 ; period<m_value.size() ; period++ )
  {
    newX      = x( m_end[period-1], start, minsPP );
    newOffset = int( m_value[period] * scale );
    if ( offset > 0 && newX > tx )
    {
      p->fillRect( tx+1, ty-offset+1, newX-tx, offset+offset-1, fill );
      p->drawLine( tx, ty+offset, newX, ty+offset );
    }
    p->drawLine( tx, ty-offset, newX, ty-offset );
    p->drawLine( newX, ty+offset, newX, ty+newOffset );
    p->drawLine( newX, ty-offset, newX, ty-newOffset );

    tx     = newX;
    offset = newOffset;
  }

  // calc end position and draw edges and fill
  newX = endX( start, minsPP );
  if ( offset > 0 && newX > tx )
  {
    p->fillRect( tx+1, ty-offset+1, newX-tx-1, offset+offset-1, fill );
    p->drawLine( tx, ty+offset, newX, ty+offset );
    p->drawLine( newX, ty+offset, newX, ty-offset );
  }
  p->drawLine( tx, ty-offset, newX, ty-offset );
}
