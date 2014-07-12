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

#include "calendarsdelegate.h"

/*************************************************************************************************/
/********************* Delegate for displaying & editing calendar data items *********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

CalendarsDelegate::CalendarsDelegate() : QStyledItemDelegate()
{
}

/***************************************** createEditor ******************************************/

QWidget*  CalendarsDelegate::createEditor( QWidget *parent,
                                           const QStyleOptionViewItem& option,
                                           const QModelIndex& index ) const
{
  // create different editors based on the row
  switch ( index.row() )
  {

    default:
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}

/***************************************** setEditorData *****************************************/

void  CalendarsDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
{
  // set the editor initial value, method depends on editor which depends on row
  switch ( index.row() )
  {

    default:
      QStyledItemDelegate::setEditorData( editor, index );
      return;
  }
}

/***************************************** setModelData ******************************************/

void  CalendarsDelegate::setModelData( QWidget* editor,
                                       QAbstractItemModel* model,
                                       const QModelIndex& index ) const
{
  // update the model value, method depends on editor which depends on row
  switch ( index.row() )
  {

    default:
      QStyledItemDelegate::setModelData( editor, model, index );
      return;
  }
}
