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
#include <QSpinBox>
#include <QDateEdit>
#include <QComboBox>

#include "calendarsdelegate.h"
#include "model/calendar.h"
#include "model/calendarsmodel.h"
#include "model/daysmodel.h"
#include "model/plan.h"

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
  if ( index.row() == Calendar::SECTION_CYCLELENGTH )
  {
    QSpinBox* editor = dynamic_cast<QSpinBox*>( QStyledItemDelegate::createEditor( parent, option, index ) );
    editor->setRange( 1, 99 );
    return editor;
  }

  if ( index.row() == Calendar::SECTION_ANCHOR )
  {
    QDateEdit* editor = dynamic_cast<QDateEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
    editor->setDateRange( QDate(1,1,1), QDate(7999,12,31) );
    editor->setCalendarPopup( true );
    return editor;
  }

  if ( index.row() >= Calendar::SECTION_NORMAL1 )
  {
    QComboBox*  combo = new QComboBox( parent );
    combo->addItems( plan->days()->namesList() );
    return combo;
  }

  // use default method for other rows
  return QStyledItemDelegate::createEditor( parent, option, index );
}

/***************************************** setEditorData *****************************************/

void  CalendarsDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
{
  // set the editor initial value, method depends on editor which depends on row
  if ( index.row() >= Calendar::SECTION_NORMAL1 )
  {
    QComboBox* days = dynamic_cast<QComboBox*>( editor );
    QString    name = index.model()->data( index, Qt::EditRole ).toString();
    days->setCurrentText( name );
    return;
  }

  QStyledItemDelegate::setEditorData( editor, index );
  return;
}

/***************************************** setModelData ******************************************/

void  CalendarsDelegate::setModelData( QWidget* editor,
                                       QAbstractItemModel* model,
                                       const QModelIndex& index ) const
{
  // check to ensure no duplicate calendar names
  if ( index.row() == Calendar::SECTION_NAME )
  {
    CalendarsModel*  cals = dynamic_cast<CalendarsModel*>( model );
    QLineEdit*       line = dynamic_cast<QLineEdit*>( editor );
    QString          name = line->text().simplified();

    if ( cals->nameIsDuplicate( name, index.column() ) )
    {
      cals->setOverride( index, name );
      emit editCell( index, "Duplicate names not allowed." );
      return;
    }

    // not duplicate so set model data with simplified name
    model->setData( index, name );
    return;
  }

  // use default method for other rows
  QStyledItemDelegate::setModelData( editor, model, index );
  return;
}
