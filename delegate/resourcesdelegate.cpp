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

#include "resourcesdelegate.h"
#include "model/resource.h"
#include "model/resourcesmodel.h"

#include <QLineEdit>
#include <QDateEdit>

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
      QDateEdit* editor = dynamic_cast<QDateEdit*>( QStyledItemDelegate::createEditor( parent, option, index ) );
      editor->setDateRange( QDate(1,1,1), QDate(7999,12,31) );
      editor->setCalendarPopup( true );
      return editor;
    }

    default:
      return QStyledItemDelegate::createEditor( parent, option, index );
  }
}

/***************************************** setEditorData *****************************************/

void  ResourcesDelegate::setEditorData( QWidget* editor, const QModelIndex& index) const
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

      if ( res->initialsIsDuplicate( id, index.row() ) )
      {
        res->setOverride( index, id );
        emit editCell( index, "Duplicate initials not allowed." );
        return;
      }

      // not duplicate so set model data with simplified name
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

    default:
      // user default method for other columns
      QStyledItemDelegate::setModelData( editor, model, index );
      return;
  }
}
