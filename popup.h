#ifndef POPUP_H
#define POPUP_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

class popup : public QWidget {

	Q_OBJECT;

	public:
		popup(QWidget *parent = 0);
		void set_text_across(const QString &text);
		void set_text_down(const QString &text);
	protected:
	
		QTextEdit *text_across;
		QTextEdit *text_down;
	
};

#endif
