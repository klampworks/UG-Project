#include "lookup_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "clue.h"

/*
 * This is the window which pops up when the lookup button is pressed.
 *
 * It provides an interactive environment for entering arbitrary clues and 
 * returning words the system would use as candidates had this been an actual
 * crossword clue, along with confidence scores.
 *
 * This is entirly for demonstrational purposes. (and debugging)
 */

lookup_window::lookup_window(QWidget *parent) : QWidget(parent) {

	use_dict = new QCheckBox("Dictionary", this);	
	use_thes = new QCheckBox("Thesaurus", this);	
	use_web_wiki = new QCheckBox("Websearch wiki lists", this);	
	use_web_goog = new QCheckBox("Websearch Google", this);	
	use_web_clue = new QCheckBox("Websearch clue db", this);	

	QHBoxLayout *h = new QHBoxLayout(this);
	QVBoxLayout *v = new QVBoxLayout();
	
	QVBoxLayout *v2 = new QVBoxLayout();
	
	input = new QLineEdit(this);
	output = new QTextEdit(this);
	output->setReadOnly(true);
	
	v2->addWidget(input);
	v2->addWidget(output);
	
	v->addWidget(use_dict);
	v->addWidget(use_thes);
	v->addWidget(use_web_wiki);
	v->addWidget(use_web_goog);
	v->addWidget(use_web_clue);
	
	h->addLayout(v);
	h->addLayout(v2);
	
	connect(input, SIGNAL(returnPressed()), this, SLOT(enter_pressed()));
}

void lookup_window::enter_pressed() {

	output->setText("");

	std::string input_s = input->text().toUtf8().constData();
	
	unsigned int index = input_s.find_last_of(' ');
	
	std::string clue_s = input_s.substr(0, index);
	std::string length_s = input_s.substr(index+1);
	unsigned int length_i = atoi(length_s.c_str());
	
	if (length_i == 0) {
	
		output->setText(QString("Please specifiy length i.e. fleshy red fruit 6"));
		return;
	}

	m_list results = solver_s.lookup_clue(clue(clue_s, length_s), 
		use_dict->isChecked(), use_thes->isChecked(), 
		use_web_wiki->isChecked(), use_web_goog->isChecked(), 
		use_web_clue->isChecked());

	QString output_text;
	output_text += "Total " + QString::number(results.size()) + " words found";

	for (unsigned i = 0; i < results.size(); i++) {
		
		output_text += "\n" + QString::fromStdString(results.get_rank(i)) + 
			"     (" + QString::number(results.get_conf(i)) + ")";
	}
	
	output->setText(output_text);
}
