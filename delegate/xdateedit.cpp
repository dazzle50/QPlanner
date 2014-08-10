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

#include "xdateedit.h"
#include "gui/xtableview.h"

/*************************************************************************************************/
/**************************** Enhanced QDateEdit that can return null ****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

XDateEdit::XDateEdit( QWidget* parent = nullptr ) : QDateEdit( parent )
{
  // initial private variables
  isNull = false;
}

/******************************************* validate ********************************************/

QValidator::State XDateEdit::validate( QString& text, int& pos ) const
{
  // check if user trying to set date to NA
  if ( pos > 0 )
  {
    char  letter = text.at( pos - 1 ).toLatin1();
    if ( letter == 'n' || letter == 'N' ) const_cast<XDateEdit*>( this )->returnNull();
  }

  // otherwise return QDateEdit validator state
  return QDateEdit::validate( text, pos );
}

/******************************************* validate ********************************************/

void XDateEdit::returnNull()
{
  // set flag to say return null and end edit
  isNull            = true;
  XTableView*  view = dynamic_cast<XTableView*>( parentWidget()->parentWidget() );
  if ( view ) view->endEdit();
}
