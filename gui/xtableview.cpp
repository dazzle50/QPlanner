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

#include "xtableview.h"

#include <QHeaderView>

/*************************************************************************************************/
/*************************** XTableView provides an enhanced QTableView **************************/
/*************************************************************************************************/

class XTableHeader : public QHeaderView
{
  public:
    XTableHeader( QWidget* parent ) : QHeaderView( Qt::Horizontal, parent )
    { customHeight = 0; }

    QSize sizeHint() const
    {
      QSize rs = QHeaderView::sizeHint();
      if ( customHeight > 0 ) rs.setHeight( customHeight );
      return rs;
    }

    int customHeight;
};

/****************************************** constuctor *******************************************/

XTableView::XTableView( QWidget* parent ) : QTableView( parent )
{
  // set horizontal header to enable having controllable height
  setHorizontalHeader( new XTableHeader( this ) );
  horizontalHeader()->setSectionsClickable( true );
  horizontalHeader()->setHighlightSections( true );

  // set smaller row height for table views
  int height = fontMetrics().lineSpacing() + 4;
  verticalHeader()->setDefaultSectionSize( height );
  verticalHeader()->setMinimumSectionSize( height );

  // enable smooth scrolling
  setVerticalScrollMode( ScrollMode::ScrollPerPixel );
  setHorizontalScrollMode( ScrollMode::ScrollPerPixel );
}

/**************************************** setHeaderHeight ****************************************/

void   XTableView::setHeaderHeight( int height )
{
  // set horizontal header height
  (dynamic_cast<XTableHeader*>(horizontalHeader()))->customHeight = height;
}

/******************************************** endEdit ********************************************/

void  XTableView::endEdit()
{
  // if any editting in progress, commits the update to the model
  QModelIndex index = currentIndex();
  currentChanged( index, index );
}
