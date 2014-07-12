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

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include "daysdelegate.h"
#include "model/day.h"

/*************************************************************************************************/
/******************** Delegate for displaying & editing day types data items *********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

DaysDelegate::DaysDelegate() : QStyledItemDelegate()
{
}

/***************************************** createEditor ******************************************/

QWidget*  DaysDelegate::createEditor( QWidget *parent,
                                      const QStyleOptionViewItem& option,
                                      const QModelIndex& index ) const
{
  // create different editors based on the column
  switch ( index.column() )
  {
    case Day::SECTION_NAME:
    {
      QLineEdit* editor = dynamic_cast<QLineEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setMaxLength( 100 );
      return editor;
    }

    case Day::SECTION_WORK:
    {
      QDoubleSpinBox*  editor = new QDoubleSpinBox( parent );
      editor->setRange( 0.0, 9.99 );
      editor->setDecimals( 2 );
      editor->setSingleStep( 0.1 );
      return editor;
    }

    case Day::SECTION_PARTS:
    {
      QSpinBox* editor = dynamic_cast<QSpinBox*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setRange( 0, 10 );
      return editor;
    }

    default:
      // TODO custom editor for period starts/ends
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}

/***************************************** setEditorData *****************************************/

void  DaysDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
{
  // set the editor initial value, method depends on editor which depends on column
  switch ( index.column() )
  {

    default:
      QStyledItemDelegate::setEditorData( editor, index );
      return;
  }
}

/***************************************** setModelData ******************************************/

void  DaysDelegate::setModelData( QWidget* editor,
                                  QAbstractItemModel* model,
                                  const QModelIndex& index ) const
{
  // update the model value, method depends on editor which depends on column
  switch ( index.column() )
  {

    default:
      QStyledItemDelegate::setModelData( editor, model, index );
      return;
  }
}
