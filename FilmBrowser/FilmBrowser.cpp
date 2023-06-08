#include "FilmBrowser.h"
#include "config.h"


FilmBrowser* FilmBrowser::my_instance = nullptr;
std::list<Filter*> FilmBrowser::filters;
std::list<state> FilmBrowser::previous_state;
Control_button* FilmBrowser::navigation[5];
state FilmBrowser::current_state;
std::list<Search_bar*> FilmBrowser::searches;
std::list<Movie*> FilmBrowser::all_movies;
std::list<Movie*> FilmBrowser::found_movies;
Movie* FilmBrowser::showing_movie;

FilmBrowser* FilmBrowser::getInstance()// returns a pointer to the instance of our film browser object
{
	if (!my_instance) {
		my_instance = new FilmBrowser();
	}
	return my_instance;
}


void FilmBrowser::update() { //updates the state of the film browser
	graphics::getMouseState(ms); // gets the informations of the mouse
	float mouse_x = graphics::windowToCanvasX(ms.cur_pos_x); // gets the position of the mouse on the x axis
	float mouse_y = graphics::windowToCanvasY(ms.cur_pos_y);// get the position of the mouse on the y axis
	bool done = false; //a bool variable so when we find the navigation button that we pressed to skip checking the rest buttons
	fix_buttons();// decides which navigation buttons will be activated

	for (auto s : searches) {// for all the search bars checks if a button from the keyboard was pressed
		s->finding(mouse_x, mouse_y);
	}

	if (ms.button_left_pressed) {// checks if the left button of the mouse was clicked
		for (auto s : searches) {//check if a searchbar was clicked
			s->pressed(mouse_x, mouse_y);
		}

		for (auto nav : navigation) {//check if a navigation button was clicked and decide which action to do based on the name of the button
			if (nav->pressed(mouse_x,mouse_y)) {
				if (nav->getName() == "home-button") {//if it was the home button go back to the home page
					previous_state.push_back(current_state);
					current_state = home;
				}
				else if (nav->getName() == "back") {// if it was the back button go to the previous state of the filmbrowser
					if (previous_state.size() > 0) {
						current_state = previous_state.back();
						previous_state.pop_back();
					}
					else {
						current_state = home;// if there is no previous state stay at home 
					}

				}
				else if (nav->getName() == "filter_menu") {// if it was the filter menu button proceed to the filter stage
					previous_state.push_back(current_state);
					current_state = filter_menu;
					
				}
				else if (nav->getName() == "Apply") {// if it was the apply button search the movies based on the filters
					previous_state.push_back(current_state);
					current_state = movie_list;
					found_movies.clear();
					bool mismatch = false ;// boolean variable which helps determine if  a move passed a filter
					for (auto m : all_movies) {
						
						for (auto f : filters) {//if a filter is active run the checks to decide if it is added to the list of found movies
							if (f->is_active()) {//with the term filter we refer to genres and year sliders
								found_movies=f->check(m, found_movies,mismatch);
								mismatch = f->check_mismatch(m,found_movies,mismatch); //decide if the movie passed the filter
							}
						}
						for (auto s : searches) {// check the searchbars that have something writen on them if it matches with the movie info
							if (s->is_active()) {
								found_movies = s->check(m, found_movies,mismatch);
								mismatch = s->check_mismatch(m,found_movies,mismatch);
							}
						}
						mismatch = false;// reset the variable for the next movie 
					}
					


				}
				else if (nav->getName() == "Reset") {// if the navigation button that was pressed was the reset button clear all the filters (checkboxes sliders and searchbars)
					for (auto f : filters) {
						f->reset();
					}
					for (auto s : searches) {
						s->reset();
					}
				}
				
				done = true;
			}
		}

		if (!done) {// if it wasnt a navigation button that was pressed 
			if (current_state==filter_menu) {// if we are on the filter menu state check the filters (checkbox if pressed or if the value of the slider changed)
				for (auto f : filters) {
					f->trigger(mouse_x, mouse_y);
				}
			}
			else if (current_state == movie_list) {// if we have found the movies and wwe are at that state check if a movie was clicked to open the viewing state
				for (auto m : found_movies) {
					if(m->pressed(mouse_x, mouse_y)){
						showing_movie=m;
						previous_state.push_back(current_state);
						current_state=found_movie;
					
					};
				}
			}
		}
		
		
		
	}
	
}

void FilmBrowser::draw() {// this is the fuction that uses the graphics library to create in our window everything
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "background.png";// we set the background
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	for (auto n : navigation) {// for all the active navigation buttons we draw them on top of the background
		if (n->is_active()) {
			n->draw();
		}

	}

	if(current_state==home){
		br.fill_color[0] = 0.0f;//creating an introduction message for the user
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.0f;
		graphics::setFont(std::string(ASSET_PATH) + "Oswald-Regular.ttf");
		graphics::drawText(CANVAS_WIDTH/2-300, CANVAS_HEIGHT/2, 60, "Welcome to this filmbrowser", br);
	}
	else if (current_state == filter_menu) {// if we are on the filter menu state 
		br.fill_color[0] = 0.0f ;
		br.fill_color[1] = 0.0f ;//we add a semi see through rectange adding as our limiter for the menu on top of the background 
		br.fill_color[2] = 0.0f ;
		br.fill_opacity = 0.6f;
		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2-50, CANVAS_WIDTH-100.0f, CANVAS_HEIGHT-100.0f, br);
		for (auto f : filters) {// we design the checkboxes and the sliders
			f->draw();
		}
		for (auto s : searches) {// we design the searchbars
			s->draw();
		}
		navigation[4]->draw();// we draw the apply button
	}
	
	else if (current_state == movie_list) {// we draw all the movies that passed the filters tests 
		int i = 0;
		if (found_movies.empty()) {
			br.fill_color[0] = 0.0f;//inform that there is no movie matching the required filters
			br.fill_color[1] = 1.0f;
			br.fill_color[2] = 0.0f;
			graphics::setFont(std::string(ASSET_PATH) + "Oswald-Regular.ttf");
			graphics::drawText(CANVAS_WIDTH / 2 - 300, CANVAS_HEIGHT / 2, 40, "No movie exists to match the filters", br);
		}
		for (auto m : found_movies) {
			m->setx((i % 5) * 150 + 200);
			m->sety(int(i / 5) * 200 + 150);
			m->draw();
			i++;
		}
	}
	else if (current_state == found_movie) {// if we have selected a move we draw all the informations about it
		showing_movie->showing_draw();
	}
	
}

void FilmBrowser::init() {// the fuction that sets up all the needed things for the app to run 
	current_state = home;// we set the starting state as the home screen 
	filters.push_back(new Genre_Filter());//we insert the checkboxes and the sliders
	filters.push_back(new Year_Slides());
	navigation[0]=(new Control_button(20.0f, 20.0f, "home-button", 30.0f));//we initialize the navigation buttons
	navigation[1]=(new Control_button(20.0f, 55.0f, "back", 30.0f));
	navigation[2]=(new Control_button(CANVAS_WIDTH / 2,25.0f, "filter_menu", 50.0f));
	navigation[3]=new Control_button(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 +175, "Apply", 50.0f);
	navigation[4] = new Control_button(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 + 50, "Reset", 70.0f);


	searches.push_back(new Search_bar("Title",CANVAS_WIDTH/2+270,CANVAS_HEIGHT/2-150));// we create the searchbars
	searches.push_back(new Search_bar("Director", CANVAS_WIDTH / 2 + 270, CANVAS_HEIGHT / 2 - 80));
	searches.push_back(new Search_bar("Protagonist", CANVAS_WIDTH / 2 + 270, CANVAS_HEIGHT / 2 - 10));

	//inserting the movies in the film browser
	Movie* to_create =new Movie("The Lord of the Rings The Return of the King", 2003, 0.0f, 0.0f, "Gandalf and Aragorn lead the World of Men against Sauron's army to draw his gaze from Frodo and Sam as they approach Mount Doom with the One Ring.");
	to_create->addDir("Peter Jackson");
	to_create->addGenre("Action");
	to_create->addGenre("Drama");
	to_create->addGenre("Fantasy");
	to_create->addPr("Elijah Wood");
	to_create->addPr("Viggo Mortensen");
	to_create->addPr("Ian McKellen");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create=new Movie("The Dark Knight",2008,0.0f,0.0f,"When the menace known as the Joker wreaks havoc and chaos on the people of Gotham, Batman must accept one of the greatest psychological and physical tests of his ability to fight injustice.");
	to_create->addDir("Christopher Nolan");
	to_create->addGenre("Action");
	to_create->addGenre("Drama");
	to_create->addPr("Christian Bale");
	to_create->addPr("Heath Ledger");
	to_create->addPr("Aaron Eckhart");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("The Shawshank Redemption", 1994, 0.0f, 0.0f, "Two imprisoned men bond over a number of years, finding solace and eventual redemption through acts of common decency.");
	to_create->addDir("Frank Darabont");
	to_create->addGenre("Drama");
	to_create->addPr("Tim Robbins");
	to_create->addPr("Morgan Freeman");
	to_create->addPr("Bob Gunton");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("Sideshow", 2001, 0.0f, 0.0f, "Three spies are brought together, to love, to kill and to be killed.");
	to_create->addDir("Thomas Napper");
	to_create->addGenre("Thriller");
	to_create->addPr("John Shrapnel");
	to_create->addPr("Ciarán McMenamin");
	to_create->addPr("Eoin McCarthy");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("Scary Movie", 2000, 0.0f, 0.0f, "A year after disposing of the body of a man they accidentally killed, a group of dumb teenagers are stalked by a bumbling serial killer.");
	to_create->addDir("Keenen Ivory Wayans");
	to_create->addGenre("Comedy");
	to_create->addPr("Anna Faris");
	to_create->addPr("Jon Abrahams");
	to_create->addPr("Marlon Wayans");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("The Notebook", 2004, 0.0f, 0.0f, "A poor yet passionate young man falls in love with a rich young woman, giving her a sense of freedom, but they are soon separated because of their social differences.");
	to_create->addDir("Nick Cassavetes");
	to_create->addGenre("Romance");
	to_create->addGenre("Drama");
	to_create->addPr("Gena Rowlands");
	to_create->addPr("James Garner");
	to_create->addPr("Rachel McAdams");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("The SpongeBob SquarePants Movie", 2004, 0.0f, 0.0f, "SpongeBob SquarePants takes leave from the town of Bikini Bottom in order to track down King Neptune's stolen crown.");
	to_create->addDir("Stephen Hillenburg");
	to_create->addDir("Mark Osborne");
	to_create->addGenre("Action");
	to_create->addGenre("Fantasy");
	to_create->addGenre("Comedy");
	to_create->addPr("Tom Kenny");
	to_create->addPr("Jeffrey Tambor");
	to_create->addPr("Rodger Bumpass");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("Saw", 2004, 0.0f, 0.0f, "Two strangers awaken in a room with no recollection of how they got there, and soon discover they're pawns in a deadly game perpetrated by a notorious serial killer.");
	to_create->addDir("James Wan");
	to_create->addGenre("Horror");
	to_create->addGenre("Mystery");
	to_create->addGenre("Thriller");
	to_create->addPr("Cary Elwes");
	to_create->addPr("Leigh Whannell");
	to_create->addPr("Danny Glover");
	all_movies.push_back(to_create);
	to_create->break_summary();
	to_create = new Movie("Pokemon", 1998, 0.0f, 0.0f, "Scientists genetically create a new Pokémon, Mewtwo, but the results are horrific and disastrous.");
	to_create->addDir("Kunihiko Yuyama");
	to_create->addDir("Michael Haigney");
	to_create->addGenre("Action");
	to_create->addGenre("Fantasy");
	to_create->addPr("Veronica Taylor");
	to_create->addPr("Rachael Lillis");
	to_create->addPr("Eric Stuart");
	all_movies.push_back(to_create);
	to_create->break_summary();

	
}


void FilmBrowser::fix_buttons() {// its a fuction that sets the right activity values on the navigation buttons so that we can draw them the right time
	if (current_state == home) {
		navigation[2]->setActive(true);
		navigation[3]->setActive(false);
		navigation[4]->setActive(false);
	}
	else if (current_state == filter_menu) {
		navigation[2]->setActive(false);
		navigation[3]->setActive(true);
		navigation[4]->setActive(true);
	}
	else {
		navigation[2]->setActive(false);
		navigation[3]->setActive(false);
		navigation[4]->setActive(false);
	}


}
