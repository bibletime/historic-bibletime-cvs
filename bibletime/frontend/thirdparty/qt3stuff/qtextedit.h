/****************************************************************************
** $Id$
**
** Definition of the QTextEdit class
**
** Created : 990101
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QT3_QTEXTEDIT_H
#define QT3_QTEXTEDIT_H

#ifndef QT_H
#include "qlist.h"
#include "qpixmap.h"
#include "qcolor.h"
#endif // QT_H

#include "qtextview.h"

#ifndef QT_NO_TEXTEDIT

namespace Qt3 {

class Q_EXPORT QTextEdit : public Qt3::QTextView
{
    Q_OBJECT
    Q_PROPERTY( int undoDepth READ undoDepth WRITE setUndoDepth )
    Q_PROPERTY( bool overWriteMode READ isOverwriteMode WRITE setOverwriteMode )

public:
    QTextEdit(QWidget *parent = 0, const char *name = 0 );
    ~QTextEdit();

    void getCursorPosition( int &parag, int &index ) const;
    bool isModified() const;
    bool italic() const;
    bool bold() const;
    bool underline() const;
    QString family() const;
    int pointSize() const;
    QColor color() const;
    QFont font() const;
    int alignment() const;
    bool isOverwriteMode() const;
    int undoDepth() const;
    virtual void insert( const QString &text, bool indent = FALSE, bool checkNewLine = TRUE, bool removeSelected = TRUE );
    virtual bool getFormat( int parag, int index, QFont &font, QColor &color );

public slots:
    virtual void setOverwriteMode( bool b );
    virtual void undo();
    virtual void redo();
    virtual void cut();
    virtual void paste();
    virtual void pasteSubType( const QCString &subtype );
    virtual void clear();
    virtual void del();
    virtual void indent();
    virtual void setItalic( bool b );
    virtual void setBold( bool b );
    virtual void setUnderline( bool b );
    virtual void setFamily( const QString &f );
    virtual void setPointSize( int s );
    virtual void setColor( const QColor &c );
    virtual void setFont( const QFont &f );
    virtual void setAlignment( int a );
    virtual void setParagType( Qt3::QStyleSheetItem::DisplayMode dm, Qt3::QStyleSheetItem::ListStyle listStyle );
    virtual void setCursorPosition( int parag, int index );
    virtual void setSelection( int parag_from, int index_from, int parag_to, int index_to, int selNum = 0 );
    virtual void setSelectionAttributes( int selNum, const QColor &back, bool invertText );
    virtual void setModified( bool m );
    virtual void resetFormat();
    virtual void setUndoDepth( int d );
    virtual void removeSelectedText();

signals:
    void undoAvailable( bool yes );
    void redoAvailable( bool yes );
    void currentFontChanged( const QFont &f );
    void currentColorChanged( const QColor &c );
    void currentAlignmentChanged( int a );
    void cursorPositionChanged( Qt3::QTextCursor *c );
    void returnPressed();
    void modificationChanged( bool m );

protected:
    enum KeyboardAction { // keep in sync with QTextView
	ActionBackspace,
	ActionDelete,
	ActionReturn,
	ActionKill
    };

    enum MoveDirection { // keep in sync with QTextView
	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	MoveHome,
	MoveEnd,
	MovePgUp,
	MovePgDown
    };

    void setFormat( Qt3::QTextFormat *f, int flags );
    void ensureCursorVisible();
    void placeCursor( const QPoint &pos, Qt3::QTextCursor *c = 0 );
    void moveCursor( MoveDirection direction, bool shift, bool control );
    void moveCursor( MoveDirection direction, bool control );
    void doKeyboardAction( KeyboardAction action );
    Qt3::QTextCursor *textCursor() const;

private:
    bool isReadOnly() const { return FALSE; }
    void emitUndoAvailable( bool b ) { emit undoAvailable( b ); }
    void emitRedoAvailable( bool b ) { emit redoAvailable( b ); }
    void emitCurrentFontChanged( const QFont &f ) { emit currentFontChanged( f ); }
    void emitCurrentColorChanged( const QColor &c ) { emit currentColorChanged( c ); }
    void emitCurrentAlignmentChanged( int a ) { emit currentAlignmentChanged( a ); }
    void emitCursorPositionChanged( Qt3::QTextCursor *c ) { emit cursorPositionChanged( c ); }
    void emitReturnPressed() { emit returnPressed(); }
    void emitModificationChanged( bool m ) { emit modificationChanged( m ); }

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QTextEdit( const QTextEdit & );
    QTextEdit& operator=( const QTextEdit & );
#endif
};

};

inline void Qt3::QTextEdit::getCursorPosition( int &parag, int &index ) const
{
    Qt3::QTextView::getCursorPosition( parag, index );
}

inline bool Qt3::QTextEdit::isModified() const
{
    return Qt3::QTextView::isModified();
}

inline bool Qt3::QTextEdit::italic() const
{
    return Qt3::QTextView::italic();
}

inline bool Qt3::QTextEdit::bold() const
{
    return Qt3::QTextView::bold();
}

inline bool Qt3::QTextEdit::underline() const
{
    return Qt3::QTextView::underline();
}

inline QString Qt3::QTextEdit::family() const
{
    return Qt3::QTextView::family();
}

inline int Qt3::QTextEdit::pointSize() const
{
    return Qt3::QTextView::pointSize();
}

inline QColor Qt3::QTextEdit::color() const
{
    return Qt3::QTextView::color();
}

inline QFont Qt3::QTextEdit::font() const
{
    return Qt3::QTextView::font();
}

inline int Qt3::QTextEdit::alignment() const
{
    return Qt3::QTextView::alignment();
}

inline bool Qt3::QTextEdit::isOverwriteMode() const
{
    return Qt3::QTextView::isOverwriteMode();
}

inline int Qt3::QTextEdit::undoDepth() const
{
    return Qt3::QTextView::undoDepth();
}

inline void Qt3::QTextEdit::insert( const QString &text, bool indent, bool checkNewLine, bool removeSelected )
{
    Qt3::QTextView::insert( text, indent, checkNewLine, removeSelected );
}

inline void Qt3::QTextEdit::setOverwriteMode( bool b )
{
    Qt3::QTextView::setOverwriteMode( b );
}

inline void Qt3::QTextEdit::undo()
{
    Qt3::QTextView::undo();
}

inline void Qt3::QTextEdit::redo()
{
    Qt3::QTextView::redo();
}

inline void Qt3::QTextEdit::cut()
{
    Qt3::QTextView::cut();
}

inline void Qt3::QTextEdit::paste()
{
    Qt3::QTextView::paste();
}

inline void Qt3::QTextEdit::pasteSubType( const QCString &subtype )
{
    Qt3::QTextView::pasteSubType( subtype );
}

inline void Qt3::QTextEdit::clear()
{
    Qt3::QTextView::clear();
}

inline void Qt3::QTextEdit::del()
{
    Qt3::QTextView::del();
}

inline void Qt3::QTextEdit::indent()
{
    Qt3::QTextView::indent();
}

inline void Qt3::QTextEdit::setItalic( bool b )
{
    Qt3::QTextView::setItalic( b );
}

inline void Qt3::QTextEdit::setBold( bool b )
{
    Qt3::QTextView::setBold( b );
}

inline void Qt3::QTextEdit::setUnderline( bool b )
{
    Qt3::QTextView::setUnderline( b );
}

inline void Qt3::QTextEdit::setFamily( const QString &f )
{
    Qt3::QTextView::setFamily( f );
}

inline void Qt3::QTextEdit::setPointSize( int s )
{
    Qt3::QTextView::setPointSize( s );
}

inline void Qt3::QTextEdit::setColor( const QColor &c )
{
    Qt3::QTextView::setColor( c );
}

inline void Qt3::QTextEdit::setFont( const QFont &f )
{
    Qt3::QTextView::setFontInternal( f );
}

inline void Qt3::QTextEdit::setAlignment( int a )
{
    Qt3::QTextView::setAlignment( a );
}

inline void Qt3::QTextEdit::setParagType( Qt3::QStyleSheetItem::DisplayMode dm, Qt3::QStyleSheetItem::ListStyle listStyle )
{
    Qt3::QTextView::setParagType( dm, listStyle );
}

inline void Qt3::QTextEdit::setCursorPosition( int parag, int index )
{
    Qt3::QTextView::setCursorPosition( parag, index );
}

inline void Qt3::QTextEdit::setSelection( int parag_from, int index_from, int parag_to, int index_to, int selNum )
{
    Qt3::QTextView::setSelection( parag_from, index_from, parag_to, index_to, selNum );
}

inline void Qt3::QTextEdit::setSelectionAttributes( int selNum, const QColor &back, bool invertText )
{
    Qt3::QTextView::setSelectionAttributes( selNum, back, invertText );
}

inline void Qt3::QTextEdit::setModified( bool m )
{
    Qt3::QTextView::setModified( m );
}

inline void Qt3::QTextEdit::resetFormat()
{
    Qt3::QTextView::resetFormat();
}

inline void Qt3::QTextEdit::setUndoDepth( int d )
{
    Qt3::QTextView::setUndoDepth( d );
}

inline void Qt3::QTextEdit::setFormat( Qt3::QTextFormat *f, int flags )
{
    Qt3::QTextView::setFormat( f, flags );
}

inline void Qt3::QTextEdit::ensureCursorVisible()
{
    Qt3::QTextView::ensureCursorVisible();
}

inline void Qt3::QTextEdit::placeCursor( const QPoint &pos, Qt3::QTextCursor *c )
{
    Qt3::QTextView::placeCursor( pos, c );
}

inline void Qt3::QTextEdit::moveCursor( MoveDirection direction, bool shift, bool control )
{
    Qt3::QTextView::moveCursor( (MoveDirectionPrivate)direction, shift, control );
}

inline void Qt3::QTextEdit::moveCursor( MoveDirection direction, bool control )
{
    Qt3::QTextView::moveCursor( (MoveDirectionPrivate)direction, control );
}

inline void Qt3::QTextEdit::removeSelectedText()
{
    Qt3::QTextView::removeSelectedText();
}

inline void Qt3::QTextEdit::doKeyboardAction( KeyboardAction action )
{
    Qt3::QTextView::doKeyboardAction( (KeyboardActionPrivate)action );
}

inline Qt3::QTextCursor *Qt3::QTextEdit::textCursor() const
{
    return cursor;
}

inline bool Qt3::QTextEdit::getFormat( int parag, int index, QFont &font, QColor &color )
{
    return Qt3::QTextView::getFormat( parag, index, font, color );
}

#endif // QT_NO_TEXTEDIT

#endif
