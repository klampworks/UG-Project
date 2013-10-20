#include "popup.h"
#include "QHBoxLayout"
#include <QScrollBar>

/*
 * This is the popup window which appears when the cursor hovers over a white box 
 * in the grid. To popup window will show the underlying stack of words and their 
 * confidence scores for whichever row or column the curser is hovering over (both
 * at an intersection point).
 *
 * Logic is in the gui.cpp file, this is just an outline of the wifgit.
 */

popup::popup(QWidget *parent) : 
	QWidget(parent, Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint) {

	this->setAttribute(Qt::WA_ShowWithoutActivating);
	this->resize(300, 400);

	text_across = new QTextEdit("kkkkkkkkkkkkkk", this);
	text_across->setReadOnly( true );
	
	text_down = new QTextEdit("lllllllllllllll", this);
	text_down->setReadOnly(true);
	
	text_down->setAlignment(Qt::AlignTop);
	text_across->setAlignment(Qt::AlignTop);
	
	QHBoxLayout *h = new QHBoxLayout(this);
	
	h->addWidget(text_across);
	h->addWidget(text_down);
	

	h->addStretch(1);
}

void popup::set_text_across(const QString &text) {
	text_across->setText(text);
}

void popup::set_text_down(const QString &text) {
	text_down->setText(text);
}
