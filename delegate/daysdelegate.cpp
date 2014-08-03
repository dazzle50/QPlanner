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
#include <QTimeEdit>

#include "daysdelegate.h"
#include "xtimeedit.h"
#include "model/day.h"
#include "model/daysmodel.h"
#include "model/plan.h"

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

    case Day::SECTION_PERIODS:
    {
      QSpinBox* editor = dynamic_cast<QSpinBox*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setRange( 0, 10 );
      return editor;
    }

    default:
      // custom editor for period starts/ends
      Day* day = plan->day( index.row() );
      Time min = 0;
      if ( index.column() > Day::SECTION_START )
      {
        // if column is not first start, set min to one minute past time in cell to left
        QVariant value = day->data( index.column()-1, Qt::EditRole );
        min = XTime::time( value.toString() ) + 1;
      }

      Time max = 24*60;
      if ( index.column() < day->periods()*2+Day::SECTION_START-1 )
      {
        // if column is not last end, set max to one minute before time in cell to right
        QVariant value = day->data( index.column()+1, Qt::EditRole );
        max = XTime::time( value.toString() ) - 1;
      }

      return new XTimeEdit( parent, min, max );
  }
}

/***************************************** setEditorData *****************************************/

void  DaysDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
{
  // set the editor initial value, method depends on editor which depends on column
  if ( index.column() >= Day::SECTION_START )
  {
    // column is a start or end which uses special time editor
    XTimeEdit* timeEditor = dynamic_cast<XTimeEdit*>( editor );
    QString newTime = index.model()->data( index, Qt::EditRole ).toString();
    timeEditor->setTime( newTime );
    return;
  }

  // use default method for name/work/periods
  QStyledItemDelegate::setEditorData( editor, index );
  return;
}

/***************************************** setModelData ******************************************/

void  DaysDelegate::setModelData( QWidget* editor,
                                  QAbstractItemModel* model,
                                  const QModelIndex& index ) const
{
  // check to ensure sufficient time after last end for increased num of work periods
  if ( index.column() == Day::SECTION_PERIODS )
  {
    DaysModel*  days     = dynamic_cast<DaysModel*>( model );
    QSpinBox*   spin     = dynamic_cast<QSpinBox*>( editor );
    int         newValue = spin->value();
    int         oldValue = model->data( index ).toInt();

    if ( oldValue > 0 && newValue > oldValue )
    {
      // check at least two minutes for each increase in num of work periods
      Time end = days->day( index.row() )->end( oldValue - 1 );
      if ( ( 24*60 - end ) / ( newValue - oldValue ) < 2 )
      {
        days->setOverride( index, newValue );
        emit editCell( index, "Insufficient time for this increase in number of work periods." );
        return;
      }
    }

    // sufficient time so set model data with new value
    model->setData( index, newValue );
    return;
  }

  // check to ensure no duplicate day type names
  if ( index.column() == Day::SECTION_NAME )
  {
    DaysModel*  days = dynamic_cast<DaysModel*>( model );
    QLineEdit*  line = dynamic_cast<QLineEdit*>( editor );
    QString     name = line->text().simplified();

    if ( name.isEmpty() )
    {
      days->setOverride( index, name );
      emit editCell( index, "Blank names not allowed." );
      return;
    }

    if ( days->nameIsDuplicate( name, index.row() ) )
    {
      days->setOverride( index, name );
      emit editCell( index, "Duplicate names not allowed." );
      return;
    }

    // not blank nor duplicate so set model data with simplified name
    model->setData( index, name );
    return;
  }

  // update the model value, method depends on editor which depends on column
  if ( index.column() >= Day::SECTION_START )
  {
    // column is a start or end which uses special time editor
    XTimeEdit* timeEditor = dynamic_cast<XTimeEdit*>( editor );
    model->setData( index, timeEditor->toString() );
    return;
  }

  // use default method for name/work/periods
  QStyledItemDelegate::setModelData( editor, model, index );
  return;
}
