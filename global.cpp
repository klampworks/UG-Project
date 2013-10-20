#include "global.h"

/*
 * Quick and dirty way of passing information between modules and threads.
 *
 * Only gui will ever modify dict_conf, thes_conf etc.
 * Pointer to the gui window will not change while the program is being 
 * executed (main will exit if it is destroyed). 
 */
		
 int global::dict_conf = 10;
 int global::thes_conf = 10;
 int global::web_wiki_conf = 10;
 int global::web_goog_conf = 10;
 int global::web_clue_conf = 10;

global::global() {}

gui* global::get_window() {

	static gui *window = new gui;
	return window;
}
