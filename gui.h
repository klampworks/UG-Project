/*
 * gui.h
 *
 *  Created on: Jan 4, 2013
 */

#ifndef GUI_H
#define GUI_H

#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include "popup.h"
#include "solver.h"
#include "solver_t.h"

class gui : public QWidget {

	Q_OBJECT

	public:
		gui(QWidget *parent = 0);
		~gui();
		void set_grid(const std::vector<std::vector<int> > &g);
		void set_grid_2(const std::vector<std::vector<int> > &g);
		
		solver solver_s;
		solver_t *solver_tt;
		std::vector<thread_param *> thread_data;

	private slots:
		void file_select_button_clicked();
		void lookup_button_pressed();
		void solve_button_pressed();
		void thread_follow_changed();
		void thread_selected_changed();
		void solution_selection_changed();
		
		void play_button_pressed();
		void forward_word_pressed();
		void forward_clue_pressed();
		
		void dict_conf_changed();
		void thes_conf_changed();
		void web_wiki_conf_changed();
		void web_goog_conf_changed();
		void web_clue_conf_changed();
		
		void keep_going_button_pressed();
		
	protected:
	
		popup *m_popup;
	
		void draw_letters();
		void start_solve();
		void prepare_grid();
		
		void paintEvent(QPaintEvent *event);
		QPainter *qp;
		void draw_grid();
		void black_square(int x, int y);
		void draw_number(int x, int y, const std::string &number);
		void draw_letter(int x, int y, const std::string &letter);
		void draw_number(int x, int y, const QString &number);
		void draw_number(int x, int y, char *number);
		
		void draw_number(int x, int y, int number);
		void clear_square(int x, int y);
		void draw_word(int x, int y, const std::string &word, bool across);
		void draw_word(const clue &m_clue, const std::string &word);
		void convert(int *x, int *y);

		void set_info_message( std::string message );
		void create_grid();
		
		QPushButton *file_select_button;
		QPushButton *lookup_button;
		QPushButton *solve_button;
		
		QPushButton *keep_going_button;
		
		QLabel *clue_label_across;
		QLabel *clue_label_down;
		QLabel *info_label;
		
		QCheckBox *use_dict;
		QCheckBox *use_thes;
		QCheckBox *use_web_wiki;
		QCheckBox *use_web_goog;
		QCheckBox *use_web_clue;
		
		QLineEdit *dict_conf;
		QLineEdit *thes_conf;
		QLineEdit *web_wiki_conf;
		QLineEdit *web_goog_conf;
		QLineEdit *web_clue_conf;
		
		QCheckBox *forward_check;

		QPushButton *play;
		QPushButton *forward_word;
		QPushButton *forward_stack;

		bool event(QEvent *e);

		std::vector<std::vector<int> > grid;
		std::vector<std::vector<int> > grid_2;
		std::vector<QRect> rects;
		
		int max_threads;
		QComboBox *thread_selection;
		QComboBox *thread_follow;
		QComboBox *solutions;
		void populate_solutions();
		
		int get_thread_selection();
		int get_thread_follow();
		void set_thread_follow();
		
		float start_x;
		float start_y;
		float end_x;
		float end_y;

		float width;
		float height;

		float columns;
		float step;
};

#endif
