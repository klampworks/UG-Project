#ifndef GLOBAL_H
#define GLOBAL_H

#include "gui.h"

class global {

	public:
		static gui * get_window();
		global();
		
		static int dict_conf;
		static int thes_conf;
		static int web_wiki_conf;
		static int web_goog_conf;
		static int web_clue_conf;
		
	private:

	
};

#endif
