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
#include <QDateEdit>
#include <QComboBox>

#include "resourcesdelegate.h"
#include "xdateedit.h"
#include "model/resource.h"
#include "model/resourcesmodel.h"
#include "model/calendarsmodel.h"
#include "model/plan.h"

/*************************************************************************************************/
/********************* Delegate for displaying & editing resource data items *********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

ResourcesDelegate::ResourcesDelegate() : QStyledItemDelegate()
{
  // set up validators to later use
  QRegExp rx1( "[^ ,\\[\\]]{0,20}" );   // up to 20 characters except space, comma, open & close sq brackets
  m_validator_initials = new QRegExpValidator( rx1, this );

  QRegExp rx2( "[^,\\[\\]]{0,99}" );    // up to 99 characters except comma, open & close sq brackets
  m_validator_name = new QRegExpValidator( rx2, this );
}

/***************************************** createEditor ******************************************/

QWidget*  ResourcesDelegate::createEditor( QWidget *parent,
                                           const QStyleOptionViewItem& option,
                                           const QModelIndex& index ) const
{
  // create different editors based on the column
  switch ( index.column() )
  {
    case Resource::SECTION_INITIALS:
    {
      QLineEdit* editor = dynamic_cast<QLineEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setValidator( m_validator_initials );
      return editor;
    }

    case Resource::SECTION_NAME:
    case Resource::SECTION_ORG:
    case Resource::SECTION_GROUP:
    case Resource::SECTION_ROLE:
    case Resource::SECTION_ALIAS:
    {
      QLineEdit* editor = dynamic_cast<QLineEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setValidator( m_validator_name );
      return editor;
    }

    case Resource::SECTION_START:
    case Resource::SECTION_END:
    {
      XDateEdit*  edit  = new XDateEdit( parent );
      Resource*   res   = plan->resource( index.row() );
      QDate       date  = index.data( Qt::EditRole ).toDate();
      if ( date.isNull() ) date = QDate::currentDate();
      edit->setCalendarPopup( true );
      edit->setDate( date );

      if ( index.column() == Resource::SECTION_START )
        // if start, can be any date equal or less then end
        edit->setDateRange( XDate::MIN_QDATE, XDate::qdate( res->end() ) );
      else
        // if end, can be any date equal or greater than start
        edit->setDateRange( XDate::qdate( res->start() ), XDate::MAX_QDATE );

      return edit;
    }

    case Resource::SECTION_CALENDAR:
    {
      QComboBox*  combo = new QComboBox( parent );
      combo->addItems( plan->calendars()->namesList() );
      return combo;
    }

    default:
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}

/***************************************** setEditorData *****************************************/

void  ResourcesDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
{
  // set the editor initial value, use default method
  QStyledItemDelegate::setEditorData( editor, index );
}

/***************************************** setModelData ******************************************/

void  ResourcesDelegate::setModelData( QWidget* editor,
                                       QAbstractItemModel* model,
                                       const QModelIndex& index ) const
{
  // update the model value, method depends on editor which depends on column
  switch ( index.column() )
  {
    case Resource::SECTION_INITIALS:
    {
      // check to ensure no duplicate resource ids (initials) entered
      ResourcesModel*  res  = dynamic_cast<ResourcesModel*>( model );
      QLineEdit*       line = dynamic_cast<QLineEdit*>( editor );
      QString          id   = line->text().simplified();

      if ( id.isEmpty() )
      {
        res->setOverride( index, id );
        emit editCell( index, "Blank initials not allowed." );
        return;
      }

      if ( res->initialsIsDuplicate( id, index.row() ) )
      {
        res->setOverride( index, id );
        emit editCell( index, "Duplicate initials not allowed." );
        return;
      }

      // not blank nor duplicate so set model data with simplified id
      model->setData( index, id   );
      return;
    }

    case Resource::SECTION_NAME:
    case Resource::SECTION_ORG:
    case Resource::SECTION_GROUP:
    case Resource::SECTION_ROLE:
    case Resource::SECTION_ALIAS:
    {
      // set model data with simplified editor text
      QLineEdit*       line = dynamic_cast<QLineEdit*>( editor );
      QString          name = line->text().simplified();
      model->setData( index, name );
      return;
    }

    case Resource::SECTION_START:
    case Resource::SECTION_END:
    {
      XDateEdit* edit = dynamic_cast<XDateEdit*>( editor );
      if ( edit->isNull )
      {
        model->setData( index, QDate() );
        return;
      }
    }

    default:
      // user default method for other columns
      QStyledItemDelegate::setModelData( editor, model, index );
      return;
  }
}
