#ifndef LOOKUP_WINDOW_H
#define LOOKUP_WINDOW_H

#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QWidget>
#include "solver.h"

class lookup_window : public QWidget {

	Q_OBJECT

	public:
	
		lookup_window( QWidget *parent = 0 );
		
	protected slots:
		void enter_pressed();
		
	protected:
	
		QTextEdit *text_input;
		
		QCheckBox *use_dict;
		QCheckBox *use_thes;
		QCheckBox *use_web_wiki;
		QCheckBox *use_web_goog;
		QCheckBox *use_web_clue;
		
		QTextEdit *output;
		QLineEdit *input;
		
	private:

		solver solver_s;
};

#endif
