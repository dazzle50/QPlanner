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

#include <QDateTimeEdit>
#include <QComboBox>

#include "tasksdelegate.h"
#include "timespanspinbox.h"
#include "xdatetimeedit.h"
#include "model/task.h"

/*************************************************************************************************/
/********************** Delegate for displaying & editing task data items ************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

TasksDelegate::TasksDelegate() : QStyledItemDelegate()
{
}

/***************************************** createEditor ******************************************/

QWidget*  TasksDelegate::createEditor( QWidget *parent,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& index ) const
{
  // create different editors based on the column
  switch ( index.column() )
  {
    case Task::SECTION_DURATION:
    case Task::SECTION_WORK:
      return new TimeSpanSpinBox( parent );

    case Task::SECTION_START:
    case Task::SECTION_END:
    {
      QDateTimeEdit*  editor = dynamic_cast<QDateTimeEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      QDateTime       dt     = index.data( Qt::EditRole ).toDateTime();
      if ( dt.isNull() ) dt = QDateTime::currentDateTime();
      editor->setDateTimeRange( XDateTime::MIN_QDATETIME, XDateTime::MAX_QDATETIME );
      editor->setCalendarPopup( true );
      editor->setDateTime( dt );
      return editor;
    }

    case Task::SECTION_DEADLINE:
    {
      XDateTimeEdit*  editor = new XDateTimeEdit( parent );
      QDateTime       dt     = index.data( Qt::EditRole ).toDateTime();
      if ( dt.isNull() ) dt = QDateTime::currentDateTime();
      editor->setDateTimeRange( XDateTime::MIN_QDATETIME, XDateTime::MAX_QDATETIME );
      editor->setCalendarPopup( true );
      editor->setDateTime( dt );
      return editor;
    }

    case Task::SECTION_TYPE:
    {
      QComboBox* editor = new QComboBox( parent );
      editor->addItem( Task::typeToString( Task::TYPE_ASAP_FDUR ) );
      editor->addItem( Task::typeToString( Task::TYPE_ASAP_FWORK ) );
      editor->addItem( Task::typeToString( Task::TYPE_SON_FDUR ) );
      editor->addItem( Task::typeToString( Task::TYPE_SON_FWORK ) );
      editor->addItem( Task::typeToString( Task::TYPE_FIXED_PERIOD ) );
      return editor;
    }

    default:
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}

/***************************************** setEditorData *****************************************/

void  TasksDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
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

void  TasksDelegate::setModelData( QWidget* editor,
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
