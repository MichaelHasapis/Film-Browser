#pragma once
#include "graphics.h"
#include "config.h"
#include "Filters.h"
#include "Widget.h"
#include <list>

enum state { home, filter_menu, movie_list, found_movie };// definition of the states of the app

class FilmBrowser 
{
	
	static std::list<state> previous_state;// list that contains all the previous states
	static state current_state;// the state that the app is currently at
	static FilmBrowser* my_instance;// the pointer to the instance of the app 
	static std::list <Filter*> filters;// a list with the filters of the app
	static Control_button* navigation[5];// n array with the navigation butttons of the app 
	static std::list<Search_bar*> searches;// a list with the searchbars of the app
	static std::list<Movie*> found_movies;// a list with the movies that passed the filters
	static Movie* showing_movie;// the movie that we want to see the information about
	static std::list<Movie*> all_movies;// all the movies inside the  browser
	graphics::MouseState ms;// the state of the mouse


public:
	
	static FilmBrowser* getInstance();//returns the pointer to instance of the app
	void update();// updates all the components of the app
	void draw();// draws all the components of the app
	void init();// initializes the components of the app

	void fix_buttons();// a fuction that sets the navigation buttons to the right state


	static void releaseInstance() { if (my_instance)delete my_instance; my_instance = nullptr; }// a destructor of the pointer
	FilmBrowser() {
		
	}
	~FilmBrowser() {//destroys everthing that was created in the app
		previous_state.clear();
		filters.clear();
		for (int i = 0; i < 5; i++) {
			delete navigation[i];
		}
		searches.clear();
		found_movies.clear();
		delete showing_movie;
		all_movies.clear();
	}
};