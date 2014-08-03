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

#include "ganttscale.h"
#include "model/datetime.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMenu>

/*************************************************************************************************/
/************************* GanttScale provides a scale for the gantt chart ***********************/
/*************************************************************************************************/

/******************************************* constuctor ******************************************/

GanttScale::GanttScale( QWidget* parent ) : QWidget( parent )
{
  // set area to have static contents
  setAttribute( Qt::WA_StaticContents );
  setAttribute( Qt::WA_NoSystemBackground );

  // set gantt scale height
  setFixedHeight( fontMetrics().lineSpacing() );

  // set private veriables default values
  m_minsPP  = 100.0;
  m_stretch = 100;
  m_menu    = nullptr;
}

/******************************************* paintEvent ******************************************/

void   GanttScale::paintEvent( QPaintEvent* event )
{
  // get the clipping rectangle
  int x = event->rect().x();
  int y = event->rect().y();
  int w = event->rect().width();
  int h = event->rect().height();

  // fill in light gray background & draw line at bottom of scale
  QPainter p( this );
  p.fillRect( x, y, w, h, QColor("#F5F5F5") );
  p.setPen( Qt::black );
  p.drawLine( x, height()-1, x+w, height()-1 );

  // calc first interval edge
  /*
  XDateTime clips = m_start.addSecs( int(m_minsPP * x) );
  XDateTime dt1   = clips.truncInterval( m_interval ), dt2;
  int       x1    = int(m_start.secsTo( dt1 ) / m_minsPP), x2;

  // set font stretch
  if ( m_stretch > 0 )
  {
    QFont font = p.font();
    font.setStretch( m_stretch );
    p.setFont( font );
  }

  // draw interval lines & labels
  p.drawLine( x1, y, x1, y+h );
  while( x1 <= x+w )
  {
    dt2 = dt1.nextInterval( m_interval );
    x2  = int(m_start.secsTo( dt2 ) / m_minsPP);
    p.drawLine( x2, y, x2, y+h );

    if ( m_stretch > 0 )
    {
      int     width   = x2 - x1 - 2;
      int     descent = p.fontMetrics().descent() + p.fontMetrics().leading() + 1;
      QString label   = dt1.toLabel( m_format );
      QRect   rect    = p.boundingRect( x1+1, y, width, h-descent-1, Qt::AlignCenter, label );

      // if bounding rect larger than space available
      if ( width < rect.width() )
      {
        m_stretch = m_stretch * width / rect.width();
        update();
        return;
      }
      else
        p.drawText( rect.bottomLeft(), label );
    }

    dt1 = dt2;
    x1  = x2;
  }
  */
}

/******************************************** setStart *******************************************/

void   GanttScale::setStart( DateTime start )
{
  // set start date-time for GanttScale
  m_start = start;
}

/***************************************** setMinsPerPixel ***************************************/

void   GanttScale::setMinsPerPixel( double mpp )
{
  // set minutes per pixel scale, reset stretch, and update the display
  m_minsPP  = mpp;
  m_stretch = 100;
  update();
}

/****************************************** setInterval ******************************************/

void   GanttScale::setInterval( int i )
{
  // set interval (XDateTime::INTERVAL_xx)
  m_interval = i;
}

/***************************************** setLabelFormat ****************************************/

void   GanttScale::setLabelFormat( QString format )
{
  // set label text format string
  m_format = format;
}

/********************************************** menu *********************************************/

QMenu*   GanttScale::menu()
{
  // if menu not already created, create now
  if ( m_menu == nullptr )
  {
    m_menu = new QMenu( this );
    m_menu->addAction("Year");
    m_menu->addAction("Half Year");
    m_menu->addAction("Quarter Year");
    m_menu->addAction("Month");
    m_menu->addAction("Week");
    m_menu->addAction("Day");
    m_menu->addSeparator();
    m_menu->addAction("Label format");
  }

  return m_menu;
}
