/*
 * gui.cpp
 *
 *  Created on: Jan 4, 2013
 */

#include "gui.h"
#include <QPainter>
#include "global.h"
#include <QEvent>
#include <QHelpEvent>
#include <QFileDialog>
#include "lookup_window.h"
#include <QThread>

gui::gui(QWidget *parent) : QWidget(parent), solver_tt(NULL) {

	 start_x = 10.00;
	 start_y = 10.00;
	 end_x = 450.00;
	 end_y = 450.00;

	 width = end_x - start_x;
	 height = end_y - start_y;

	 columns = 15.00;
	 step = (end_x / columns);
	 
	 clue_label_across = new QLabel("", this);
	 clue_label_down = new QLabel("", this);
	 clue_label_down->setGeometry(10, 480, 350, 300);
	 clue_label_down->setWordWrap(true);
	 
	 clue_label_across->setGeometry(370, 480, 350, 300);
	 clue_label_across->setWordWrap(true);
	 
	info_label = new QLabel( "Info: ", this );
	info_label->setGeometry( 470, 10, 300, 80 );
	 
	file_select_button = new QPushButton("select file", this);
	file_select_button->setGeometry(470,100,200,30);
	connect(file_select_button, SIGNAL(clicked()), this, SLOT(file_select_button_clicked()));
	
	lookup_button = new QPushButton("lookup", this);
	lookup_button->setGeometry(470, 400, 90, 30);
	connect(lookup_button, SIGNAL(clicked()), this, SLOT(lookup_button_pressed() ) );
	
	solve_button = new QPushButton("solve", this);
	solve_button->setGeometry(570, 400, 90, 30);
	solve_button->setEnabled(false);
	connect(solve_button, SIGNAL(clicked()), this, SLOT(solve_button_pressed()));
	
	keep_going_button = new QPushButton("keep going", this);
	keep_going_button->setGeometry(670, 400, 90, 30);
	connect(keep_going_button, SIGNAL(clicked()), this, SLOT(keep_going_button_pressed()));
	
	use_dict = new QCheckBox("Dictionary", this);	
	use_thes = new QCheckBox("Thesaurus", this);	
	use_web_wiki = new QCheckBox("Websearch wiki lists", this);	
	use_web_goog = new QCheckBox("Websearch Google", this);	
	use_web_clue = new QCheckBox("Websearch clue db", this);
	
	use_dict->setGeometry(470,140,90,15);
	use_thes->setGeometry(470,160,90,15);
	use_web_wiki->setGeometry(470,180,200,15);
	use_web_goog->setGeometry(470,200,200,15);
	use_web_clue->setGeometry(470,220,200,15);
	
	use_dict->setChecked(true);
	use_thes->setChecked(true);
	use_web_wiki->setChecked(false);
	use_web_goog->setChecked(false);
	use_web_clue->setChecked(false);
	
	dict_conf = new QLineEdit( QString::number(global::dict_conf), this);
	thes_conf = new QLineEdit( QString::number(global::thes_conf), this);	
	web_wiki_conf = new QLineEdit( QString::number(global::web_wiki_conf), this);
	web_goog_conf = new QLineEdit( QString::number(global::web_goog_conf), this);
	web_clue_conf = new QLineEdit( QString::number(global::web_clue_conf), this);
	
	dict_conf->setGeometry(570,140,30,15);
	thes_conf->setGeometry(570,160,30,15);
	web_wiki_conf->setGeometry(670,180,30,15);
	web_goog_conf->setGeometry(670,200,30,15);
	web_clue_conf->setGeometry(670,220,30,15);	
	
	connect(dict_conf, SIGNAL(textEdited(const QString)), 
		this, SLOT(dict_conf_changed()));
	connect(thes_conf, SIGNAL(textEdited (const QString)), 
		this, SLOT(thes_conf_changed()));
	connect(web_wiki_conf, SIGNAL(textEdited(const QString)), 
		this, SLOT(web_wiki_conf_changed()));
	connect(web_goog_conf, SIGNAL(textEdited(const QString)), 
		this, SLOT(web_goog_conf_changed()));
	connect(web_clue_conf, SIGNAL(textEdited(const QString)), 
		this, SLOT(web_clue_conf_changed()));

	forward_check = new QCheckBox( "Forward checking", this);
	forward_check->setGeometry( 470, 450, 150, 15 );
	forward_check->setChecked( true );
	
	//threads
	int tmp_i = QThread::idealThreadCount();
	
	if (tmp_i == -1)
		max_threads = 1;
	else
		max_threads = tmp_i;
	
	thread_selection = new QComboBox(this);
	thread_selection->setGeometry(470, 260, 90, 20);
	
	for (int i = max_threads; i > 0; i--)
		thread_selection->addItem(QString::number(i));
	
	QLabel *thread_label = new QLabel("No. of threads", this);
	thread_label->setGeometry(470, 240, 90, 20);
	connect(thread_selection, SIGNAL(currentIndexChanged(int)), 
		this, SLOT(thread_selection_changed()) );
	
	QLabel *follow_label = new QLabel("Follow thread", this);
	follow_label->setGeometry(570, 240, 90, 20);
	thread_follow = new QComboBox(this);
	thread_follow->setGeometry(570, 260, 90, 20);
	set_thread_follow();
	connect(thread_follow, SIGNAL(currentIndexChanged(int)), 
		this, SLOT(thread_follow_changed()) );
	
	QLabel *solution_label = new QLabel( "Solutions", this );
	solution_label->setGeometry( 570, 280, 90, 20 );
	solutions = new QComboBox( this) ;
	solutions->setGeometry( 570, 300, 90, 20 );
	connect(solutions, SIGNAL(currentIndexChanged(int)), 
		this, SLOT(solution_selection_changed()) );
	
	//control buttons
	play = new QPushButton("play", this);
	play->setGeometry(470, 300, 50, 20);
	connect(play, SIGNAL(clicked()), this, SLOT(play_button_pressed()));
	
	forward_word = new QPushButton("next word", this);
	forward_word->setGeometry(470, 325, 100, 20);
	connect(forward_word, SIGNAL(clicked()), this, SLOT(forward_clue_pressed()));
	
	forward_stack = new QPushButton("next clue", this);
	forward_stack->setGeometry(470, 355, 100,20);
	
	m_popup = new popup();
}


//Use has toggled dictionary on or off.
void gui::dict_conf_changed() {
	global::dict_conf = dict_conf->text().toInt();
}

//User has toggled thesaurus on or off.
void gui::thes_conf_changed() {
	global::thes_conf = thes_conf->text().toInt();
}

//User has toggled wikipedia seach on or off.
void gui::web_wiki_conf_changed() {
	global::web_wiki_conf = web_wiki_conf->text().toInt();
}

//User has toggled google search on or off.
void gui::web_goog_conf_changed() {
	global::web_goog_conf = web_goog_conf->text().toInt();
}

//User has toggled clue database on or off.
void gui::web_clue_conf_changed() {
	global::web_clue_conf = web_clue_conf->text().toInt();
}

//remove?
void gui::set_info_message(std::string message) {

	std::cout << message << std::endl;
	info_label->setText( QString("Info: ") + QString::fromStdString( message ) );
	//global::get_window()->update();

}

//How many threads has the user elected to use?
int gui::get_thread_selection() {
	return (((thread_selection->currentIndex() ) * -1) + max_threads);
}

//Which thread does the user want to follow.
int gui::get_thread_follow() {
	return ((thread_follow->currentIndex()));
}

//User has changed the number of threads to use.
void gui::set_thread_follow() {
	
	int selected_no_threads = get_thread_selection();
	int current = thread_follow->count();

	//If new selection includes more threads than before...
	if (selected_no_threads > current) {
		
		//Add the new threads to the thread follow box.
		for (int i = current + 1; i <= selected_no_threads; i++)
			thread_follow->addItem(QString::number(i));
	} else {

		//Remove the unused threads from the thread follow selection.	
		for (int i = current -1; i >= selected_no_threads; i--)
			thread_follow->removeItem(i);
	}
}

void gui::thread_selected_changed() {
	set_thread_follow();
}

void gui::thread_follow_changed() {
	populate_solutions();
}

void gui::play_button_pressed() {

	static int state = 0;

	switch (state) {
		case 0:
			play->setText("pause");
			state = 1;
			break;
		case 1:
			play->setText("play");
			state = 0;
			break;
	}

	solver_t::set_thread_pause(true);
	solver_t::set_thread_sleep(1000);
	solver_t::set_next_word();
	
	start_solve();
}

void gui::forward_clue_pressed() {
	solver_t::set_next_word();
}

void gui::forward_word_pressed() {
std::cout << "DOES NOTHING" << std::endl;
}

void gui::solve_button_pressed() {
	std::cout << "Starting the solve process" << std::endl;
	start_solve();
}

void gui::start_solve() {

	if (solver_tt) {
	
		set_info_message( "Cleaning up environment..." );
	
		delete solver_tt;
		solver_tt = NULL;
		thread_data.clear();
		solutions->clear();
	}
	
	solver_s.create_grid();
	
	solver_s.branch_order();
	
	for (unsigned i = 0; i < solver_s.master_stack.size(); i++)
		solver_s.master_stack.get(i).print();
	
	solver_s.prepare_stack(use_dict->isChecked(), use_thes->isChecked(), 
				use_web_wiki->isChecked(), use_web_goog->isChecked(), 
				use_web_clue->isChecked());
	
	solver_s.create_grid();
	
	solver_tt = new solver_t((&solver_s.cl), (&solver_s.master_stack), (&solver_s.cm));
	
	thread_data = solver_tt->solve(2, forward_check->isChecked());
}

void gui::lookup_button_pressed() {
	
	lookup_window *wm = new lookup_window();
	wm->show();
}

void gui::file_select_button_clicked() {
	
	static QString last_directory = "~/";
	
	QString s = QFileDialog::getOpenFileName(this, tr("Open File"), 
						last_directory, NULL);

	if (s.isNull())
		return;

	if (solver_tt) {
	
		delete solver_tt;
		solver_tt = NULL;
		thread_data.clear();
		solutions->clear();
	}
	
	clue_label_across->setText( QString("") );
	clue_label_down->setText( QString("") );
	
	global::get_window()->update();

	last_directory = s.mid(0, s.lastIndexOf("/"));

	file_select_button->setText(s.mid( s.lastIndexOf("/") + 1 ) );
	file_select_button->setToolTip(s);
	
	solver_s.prepare_clue_list(s.toUtf8().constData());
	
	columns = solver_s.get_grid_size();
	step = (end_x / columns);
		
	solver_s.create_grid();
	
	for (int i = 0; i < solver_s.cl_size(); i++) {
		
		std::string tmp = solver_s.cl_get_clue_text(i);
		
		if (solver_s.cl_is_across(i)) {
			
			clue_label_across->setText(
				clue_label_across->text().append(
					QString::fromStdString(tmp + "\n")));

		} else {
			clue_label_down->setText(
				clue_label_down->text().append(
					QString::fromStdString(tmp + "\n")));
		}
	}
	
	lookup_button->setEnabled(true);
	solve_button->setEnabled(true);
}

bool gui::event(QEvent *e) {

	if (e->type() == QEvent::ToolTip) {

    	QHelpEvent *helpEvent = static_cast<QHelpEvent*>(e);
    	
    	std::vector<int> clue_numbers;
    	clue_numbers.clear();
    	
    	for (int i = 0; i < rects.size(); i++) {
    		
    		if (rects[i].contains(helpEvent->pos())) {
    			
    			clue_numbers.push_back(i);
    			
    			if (clue_numbers.size() == 2) {
    				//there will never be more than 2 clues intersecting each other
    				break;
    			}
    		}
    	}
    	
    	if (clue_numbers.size() == 0) {
    		
    		m_popup->hide();
    		
    		setToolTip("");
    		e->ignore();
    		return QWidget::event( e );
    	}
    	
    	
    	bool ms_ok = (solver_s.master_stack.size() > 0);
    	QString text("\n");
    	QString text_across( "\n" );
    	QString text_down( "\n" );
    	bool is_across = false;
    	
    	for (int i = 0; i < clue_numbers.size(); i++) {
    	
    		text += QString("\nClue ") + QString::number(grid_2[clue_numbers[i]][0]);

    			
    		if (grid_2[clue_numbers[i]][4] == 1) {
    			//across
    			text += QString(" Across\n");
    			
    			text_across += QString::number (grid_2[clue_numbers[i]][0]);
    			text_across += QString(" Across\n");
    			 is_across = true;
    			 
    		} else  {
    			//down
    			text += QString(" Down\n");
    			
    			text_down += QString::number (grid_2[clue_numbers[i]][0]);
    			text_down += QString(" Down\n");
    			 is_across = false;
    		}
    		
    		if (ms_ok) {
    			   			
		m_list local_ms = solver_s.get_master_stack_m(clue_numbers[i]);
			
		for (int j = 0; j < local_ms.size(); j++) {
				
			if (is_across) {
			
				text_across += QString::fromStdString(local_ms.get_rank(j)) + "     (" + QString::number(local_ms.get_conf(j)) + ")\n";
			
			} else {
			
				text_down += QString::fromStdString(local_ms.get_rank(j)) + "     (" + QString::number(local_ms.get_conf(j)) + ")\n";
			
			}
    		}
		}

    	}
    	
	//Populate the popup window with the strings we generated.
    	m_popup->set_text_across(text_across);
    	m_popup->set_text_down(text_down);
    	
	//Move the popup to where the cursor is.
    	QPoint pos = QCursor::pos();
	m_popup->move(pos);
    	m_popup->show();
    }
    	
	return QWidget::event(e);
}

//Called when QT needs to repaint part of the window.
void gui::paintEvent(QPaintEvent *e) {

	Q_UNUSED(e);

	QPainter qp(this);
	this->qp = &qp;
	
	draw_grid();
	draw_letters();
}


void gui::solution_selection_changed() {
	global::get_window()->update();
}

//Populate the solutions combobox.
void gui::populate_solutions() {

	int thread = get_thread_follow();
	
	if (thread >= thread_data.size())
		return;

	thread_param *thread_data_tmp = thread_data[get_thread_follow()];

	if (!thread_data_tmp)
		return;
	
	if (!thread_data_tmp->finished)
		return;

	solutions->clear();

	std::vector<std::vector<std::string> > tmp_solutions = thread_data_tmp->t_sol.get_all();
			
	for (unsigned i = 0; i < tmp_solutions.size(); i++)
		solutions->addItem(QString::number(tmp_solutions[i].size()) + " letters");
}

void gui::keep_going_button_pressed() {

	int thread = get_thread_follow();
	
	if (thread >= thread_data.size())
		return;

	thread_param *thread_data_tmp = thread_data[get_thread_follow()];

	if (!thread_data_tmp)
		return;
	
	if (!thread_data_tmp->finished)
		return;
	
	solver_tt->solve(2, forward_check->isChecked(), thread_data_tmp->t_sol);
}

void gui::draw_letters() {

	int selected_thread = get_thread_follow();
	
	if (thread_data.size() <= selected_thread)
		return;
	
	thread_param *thread_data_tmp = thread_data[selected_thread];
	
	if (!thread_data_tmp)
		return;	
	
	if (thread_data_tmp->finished) {
	
		//no more live attempts, paint static solutions instead
		if (solutions->count() == 0) {
		
			//populate combobox
			populate_solutions();
		
		} else {
		
			std::vector<std::vector<std::string> > tmp_solutions = thread_data_tmp->t_sol.get_all();
			int index = solutions->currentIndex();
			
			//safty
			if (index >= tmp_solutions.size())
				//do nothing
				return;
		
			for (int i = 0; i < tmp_solutions[index].size(); i++) {
	
				draw_word(thread_data_tmp->cl->get_clue(thread_data_tmp->solver_stack.logical_index(i)), tmp_solutions[index][i]);
			
			}
		}

	} else {
	
		//Thread is still running, paint the current status of the solve process
		for (int i = 0; i < thread_data_tmp->solver_stack.size(); i++) {
	
			clue one = thread_data_tmp->cl->get_clue(thread_data_tmp->solver_stack.logical_index(i));
			clue two = thread_data_tmp->cl->get_clue(i);
			
			std::string word_one = thread_data_tmp->solver_stack.get(thread_data_tmp->solver_stack.logical_index(i)).peek_s();
			std::string word_two = thread_data_tmp->solver_stack.get(i).peek_s();
				
			clue tmp_clue = thread_data_tmp->cl->get_clue(i);
			std::string tmp_word = thread_data_tmp->solver_stack.get(i).peek_s();
			
			draw_word(tmp_clue, tmp_word);
		}
	}
}

void gui::draw_word(const clue &m_clue, const std::string &word) {

	if (word.empty())
		return;

	int x = m_clue.get_start_x();
	int y = m_clue.get_start_y();
	bool is_across = m_clue.get_across();
		
	draw_word(x, y, word, is_across);
}

//Draw a grid of white squares.
void gui::draw_grid() {

	qp->setBrush(QBrush("#ffffff"));
	qp->drawRect(start_x-2, start_y-2, end_x+4, end_y+4);

	qp->setPen("#000000");
	qp->setBrush(QBrush("#000000"));
	qp->drawLine(start_x + step, start_y, start_x + step, end_y+start_y);

	for (qreal i = start_x; i <= end_x + start_x; i += step) {

		qp->drawLine(i, start_y, i, end_y+start_y);
		qp->drawLine(start_x, i, end_x+start_x, i);
	}

	prepare_grid();
}

void gui::black_square(int x, int y) {

	convert(&x, &y);

	x += 1;
	y += 1;

	int size = step - 1;

	qp->setPen("#000000");
	qp->setBrush(QBrush("#000000"));
	qp->drawRect(x, y, size, step);
}

void gui::clear_square(int x, int y) {

	convert(&x, &y);

	x += 1;
	y += 10;

	qp->setPen("#ffffff");
	qp->setBrush(QBrush("#ffffff"));
	qp->drawRect(x, y, (step - 2 ), ((step - 2) - 10));
}

void gui::draw_number(int x, int y, char *number) {

	draw_number(x, y, QString(number));
}

void gui::draw_number(int x, int y, int number) {

	draw_number(x, y, QString::number(number));
}

void gui::draw_number(int x, int y, const std::string &number) {
	
	draw_number(x, y, QString::fromStdString(number));
}
void gui::draw_number(int x, int y, const QString &number) {

	convert(&x, &y);

	x += 1;
	y += 8;

	qp->setPen("#000000");
	qp->setBrush(QBrush("#000000"));
	QFont font("Georgia", 7);
	QPainterPath my_path;
	my_path.addText(x, y, font, number);
	qp->drawPath(my_path);
}

void gui::draw_letter(int x, int y, const std::string &letter) {

	y += 1;

	convert(&x, &y);

	x += 7;
	y -= 6;

	qp->setPen("#000000");
	qp->setBrush(QBrush("#000000"));
	QFont font("Georgia", 16);
	QPainterPath my_path;
	my_path.addText(x, y, font, letter.c_str());
	qp->drawPath(my_path);
}

void gui::draw_word(int x, int y, const std::string &word, bool across) {

	for (unsigned i = 0; i < word.size(); i++) {
		if (across) {
		
			clear_square((i+x), y);
			draw_letter((i+x), (y), word.substr(i, 1));
		} else {
		
			clear_square((x), (i+y));
			draw_letter((x), (i+y), word.substr(i, 1));
		}
	}
}

//Convert logical indexies to real pixel coordinates.
void gui::convert(int *x, int *y) {

	*x = (*x * step) + start_x;
	*y = (*y * step) + start_y;
}

void gui::prepare_grid() {

	for (unsigned i = 0; i < grid.size(); i ++) {

		for (unsigned j = 0; j < grid[i].size(); j++) {

			if (!grid[i][j])
				black_square(i, j);
		}
	}
	
	rects.clear();
	
	for (unsigned i = 0; i < grid_2.size(); i++) {
	
		draw_number(grid_2[i][1], grid_2[i][2], grid_2[i][0]);
		
		int x1 = grid_2[i][1];
		int y1 = grid_2[i][2];
		int x2 = grid_2[i][3];
		int y2 = grid_2[i][4];
		
		convert(&x1, &y1);
		convert(&x2, &y2);
		
		rects.push_back(QRect(x1, y1, x2, y2));
	}
}

void gui::set_grid(const std::vector<std::vector<int> > &g) {
	this->grid = g;
}

void gui::set_grid_2(const std::vector<std::vector<int> > &g) {
	this->grid_2 = g;
}

void gui::create_grid() {

	int size = columns;
	std::vector<std::vector<int> > my_grid;
	std::vector<int> tmp;

	for (int i = 0; i < size; i ++) {

		for (int j = 0; j < size; j++) {
			tmp.push_back((j % 2) + (i % 2));
		}

		my_grid.push_back(tmp);
		tmp.clear();
	}

	prepare_grid();
}

gui::~gui() {
	delete m_popup;
}
