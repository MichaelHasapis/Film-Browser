#pragma once
#include "graphics.h"
#include "Widget.h"




class Filter {// an abstruct class for the filters of the app
public:
	virtual void draw() {}// the fuction that will create them on the window
	virtual void init() {}// initiliazation of the variables
	virtual void trigger(float x,float y){}//check if it was pressed
	virtual void reset() {}//reset to the default values
	virtual std::list<Movie*> check(Movie* m, std::list<Movie*> many, bool mismatch) { return many; }//checks if a movie passes the filter criteria
	virtual bool check_mismatch(Movie* m, std::list<Movie*> many, bool mis) { return true; }//check if a movie was blocked by a previous filter
	virtual bool is_active() { return true; }// check if a filter is active
};

class Genre_Filter : public Filter {// a class that contains all the check boxes of the genres in the browser
	std::vector<std::string> genre_names;// list with the names of the genres
public:
	std::vector<Checkbox*> genres;// list that contains all the check boxes (checkboxes are used solely for genre finding purpose)
	Genre_Filter() {// when a genre filter object is created it calls its initiliazation fuction
		init();
	}

	void init() {// the initiliazation fuction of the genre filter it creates all the checkboxes
		genre_names.push_back("Action");
		genre_names.push_back("Comedy");
		genre_names.push_back("Drama");
		genre_names.push_back("Fantasy");
		genre_names.push_back("Horror");
		genre_names.push_back("Mystery");
		genre_names.push_back("Romance");
		genre_names.push_back("Thriller");
		genre_names.push_back("Western");


		for (int i = 0; i < genre_names.size(); i++) {
			Checkbox* check = new Checkbox(((i % 3) * 120) + 95.0f, ((int)((i / 3)) * 120) + 100.0f, genre_names[i], 75.0f);
			genres.push_back(check);
		}
	}

	bool is_active() {// check if any checkbox is active which makes the whole genre filter being active
		for (auto g : genres) {
			if (g->is_active()) {
				return true;
			}
		}
		return false;
	}

	void trigger(float x, float y) override {// checks if a checkbox was pressed and updates its state
		for (auto g : genres) {
			if (g->pressed(x, y)) {
				g->update();
			}
		}
	}

	void draw() {// draws to the window the checkboxes
		for (auto box : genres) {
			box->draw();
		}

	}

	std::list<Movie*> check(Movie* m, std::list<Movie*> many, bool mismatch) {// checks if the movie has any of the active genres this filter
		int matching = 0;//it takes as variables a movie the list to add it and the boolean value of the previous filter 
		int active = 0;
		for (auto g: genres) {
			
			if (g->is_active()) {
				active++;
				if (m->checkGen(g->getName())) {
					matching++;
				}
			}
		}
		if (active == matching) {// if all the active genres are the genres of the movie then the movie is added
			many.push_back(m);
		}
		return many;
		
	}

	bool check_mismatch(Movie* m, std::list<Movie*> many, bool mis) override{//decides if the movie has passed this filter or blocks it for the next filters 
		return std::find(std::begin(many), std::end(many), m) == std::end(many);//it takes as variables a movie the list to add it and the boolean value of the previous filter 
	}

	void reset() override {// resets the values of the checkboxes
		for (auto g : genres) {
			g->setActive(false);
		}
	
	}	
};


class Year_Slides : public Filter {// the slider bars filter for the date of the movie 
	
	Slider* year_from;//two sliders one symbolising the lower limit
	Slider* year_to;// one the upper

public:
	Year_Slides() {
		init();// initialises the sliders when an object is created
	}

	void init() {//the initi;iazation function it sets up the sliders

		year_from = new Slider(500, 100, "From");
		year_to = new Slider(500, 200, "To");
	}

	bool is_active() {// checks if the sliders are active
		if (year_from->is_active() && year_to->is_active()) {
			return true;
		}
		return false;
	}

	void draw() {// draws to the window the sliders
		year_from->draw();
		year_to->draw();

	}
	

	std::list<Movie*> check(Movie* m, std::list<Movie*> many, bool mismatch) override{//it takes as variables a movie the list to add it and the boolean value of the previous filter 
		if (!mismatch) {// if it wasnt blocked by the previous filter
			if (m->get_date() >= year_from->get_val() && m->get_date() <= year_to->get_val()) {//if the value is between the sliders values 
				if (std::find(std::begin(many), std::end(many), m) == std::end(many)) {//if it not inside the list
					many.push_back(m);//add the movie to the list
				}
			}
			else {
				if (std::find(std::begin(many), std::end(many), m) != std::end(many)) {//if the movie is in the list and the date is not in the range of the sliders
					many.remove(m);// remove the movie
				}
			}
		}
		return many;
		
	}

	bool check_mismatch(Movie* m, std::list<Movie*> many, bool mis) override{//it takes as variables a movie the list to add it and the boolean value of the previous filter 
		if (mis) {//if it was blocked it stays blocked
			
			return mis;
		}
		else {
			return std::find(std::begin(many), std::end(many), m) == std::end(many);
		} // if it was not blocked checks if it passed the filter and blockes it for the next
	}


	void trigger(float x, float y) {//if the slider was pressed it changes the value of the slider based on the coordinations passed
		year_from->update(x, y);
		year_to->update(x, y);
	}


	void reset() {// setd the filter back to its original state when it was initialized
		year_from->set_val(2000);
		year_from->set_val_pos(year_from->get_pos_x());
		year_from->setActive(false);
		year_to->set_val(2000);
		year_to->set_val_pos(year_to->get_pos_x());
		year_to->setActive(false);
	}

};