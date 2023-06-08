#pragma once
#include "graphics.h"
#include "config.h"
#include "Movie.h"
#include <list>



class Widget// widget class in an abstruct class to create all the checkboxes searchbars and sliders
{
protected:
	std::string name;//name of the widget
	float pos_x=0.0f;// position on x axis
	float pos_y=0.0f;// position on y axis
	bool active = false;// boolean for activity
public:
	std::string getName() {
		return name;//fuction to get the name
	}

	float get_pos_x() {// function to get the postion on x axis
		return pos_x;
	}

	float get_pos_y() {// function to get the postion on y axis
		return pos_y;
	}

	void update() {// update fuction that canges the activity boolean
		active = !active;
	}
	virtual void draw() {// draw function to design it on the window
	}
	bool is_active() {// returns the activity of the wigdte
		return active;
	}
	virtual bool pressed(float x, float y) {// fuction that checks if a widget was pressed
		return true;
	}

	void setActive(bool a) {// changes the activity variable to the value given
		active = a;
	}

};

class Search_bar : public Widget //searchbar class a class that you write and saves the written value
{
	std::string searching;// the result of your writitng
	bool idle;// if the searchbar is ready to be written in
public:

	Search_bar(std::string n,float x, float y) {// inititalise the name of the searchbar the postion of it and the word holding and the idle variable
		name = n;
		pos_x = x;
		pos_y = y;
		init();
	}

	void init() {// initialization of the idle state and the word 
		searching = "";//empty stirng
		idle = false;// not ready to be used
	}

	void draw() override {// draw function its for designing the searchbar on the window
		graphics::Brush br;
		br.fill_color[0] = 0.4f ;//give the brush this colour
		br.fill_color[1] = 0.0f ;
		br.fill_color[2] = 1.0f ;
		br.outline_opacity = 0.0f;// turn of the outline opacity
		graphics::drawRect(pos_x, pos_y, 150.0f, 30.0f, br);// create a rectange to the postion of the searchbar with width 150 and height 30
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;// change colour
		br.fill_color[2] = 1.0f;
		graphics::setFont(std::string(ASSET_PATH) + "Oswald-Regular.ttf");// set the font to the downloaded one
		graphics::drawText(pos_x-50, pos_y-20, 20.0f, name, br);// draw the name of the searchbar
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;//change colour
		br.fill_color[2] = 0.0f;
		graphics::drawText(pos_x-20, pos_y+10 , 15.0f, searching, br);// draw the result of the writing on the searchbar
	}

	bool pressed(float x, float y) override { // checks if the searchbar was prssed so that the idle state can be state to true
		if (x<pos_x + 75 && x>pos_x - 75 && y<pos_y + 15 && y > pos_y - 15) {
			idle = true;
			return true;
		}
		idle = false;
		return false;
	}


	void reset()   {// reset the word which the searchbar holds
		active = false;
		searching = "";
		idle = false;
	}


	void finding(float x,float y) {// gets the information from the keyboard and saves the word in to the variable
		if (idle) {
			sleep(50);//to give time to the user to print one letter
			for (int i = 4; i <= 44; i++) { // keyboard input
				if (graphics::getKeyState((graphics::scancode_t)i)) {	
					if (scancodes(i) == "backspace") {//if the backspace button was pressed delete the last character of the word
						searching = searching.substr(0, searching.length() - 1);
					}
					else if (searching.length() < 30) {//if the length is less than 30 (a limit to the length of the word searchingn) 
						searching += scancodes(i);
					}
				}
			}
			if (searching != "") {// if the word isnt empty make filter active
				active = true;
			}
			else {
				active = false;// otherwise turn off filter
			}
		}
		
	}




	std::list<Movie*> check(Movie* m, std::list<Movie*> many,bool mismatch) {//checks if the information of the movie is the same as the variable saved on the searchbar
		if (!mismatch) {// if there wasnt a block from previous filter
			std::string t;
			if (name == "Title") {// find which searchbar was used based on the name
				t = m->get_Name();
				for (int x = 0; x < t.size(); x++) {
					t[x] = tolower(t[x]);// turn the info lowercase so it can be compared with the info on the searchbar
				}
			}
			else if (name == "Protagonist") {
				for (auto p : m->get_Protagonists()) {
					t = p;
					for (int x = 0; x < p.size(); x++) {
						t[x] = tolower(p[x]);
					}
				}
			}
			else if (name == "Director") {
				for (auto d : m->get_Director()) {
					t = d;
					for (int x = 0; x < d.size(); x++) {
						t[x] = tolower(d[x]);
					}
				}
			}


			if (t.find(searching) != std::string::npos) {// if the word saved is a substring of the info
				if (std::find(std::begin(many), std::end(many), m) == std::end(many)) {//if the movie isnt added to the list
					many.push_back(m);// add the movie to the list
				}
			}
			else {
				if (std::find(std::begin(many), std::end(many), m) != std::end(many)) {//if the word wasnt a substring and the movie is in the list
					many.remove(m);// remove the movie from the list
				}
			}
		}
		
		
		return many;
	
	}


	bool check_mismatch(Movie* m, std::list<Movie*> many, bool mis)  {// once again a function that blocks the next filters if the movie didnt pass this one
		if (mis) {

			return mis;
		}
		else {
			return std::find(std::begin(many), std::end(many), m) == std::end(many);
		}
	}

};


class Button : public Widget {// an abstruct button class helps us create later classes for checkboxes and navigation/ control buttons
protected: 
	float size;// size fo the buttom
public:
	Button(float x, float y, std::string n,float s) {
		pos_x = x;//position on x axis
		pos_y = y;// position on y axis
		name = n;// name of the button
		size = s;// size of the button
	}
	void draw() override{// creates the button in the window

		graphics::Brush br;
		br.fill_color[0] = 0.0f + active;
		br.fill_color[1] = 1.0f + active;//give a colour that cahnges based on whether is active the button or not
		br.fill_color[2] = 1.0f + active;
		br.outline_opacity = 0.0f;
		br.texture = std::string(ASSET_PATH) + "button.png";
		graphics::drawRect(pos_x, pos_y, size, size, br);
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.3f;
		graphics::setFont(std::string(ASSET_PATH) + "Oswald-Regular.ttf");// draw the name of the button on it
		graphics::drawText(pos_x-25, pos_y , size/4, name, br);
	}

	bool pressed(float x, float y) override {// overrides the widget method and checks if the button was pressed
		if (x<pos_x + (size / 2) && x>pos_x - (size / 2) && y<pos_y + (size / 2) && y > pos_y - (size / 2)) {
			return true;
		}
		return false;
	}

	

	void setActive(bool st) {// set the activity variable of the button to the value given
		active = st;
	}

};


class Checkbox : public Button {// a check box class 

public:// uses the constructor of the button
	Checkbox(float x,float y, std::string n ,float s) : Button(x,y,n,s) {
		
	}
	
	
};


class Control_button : public Button {
public:
	Control_button(float x, float y, std::string n, float s) : Button(x, y, n, s) {// uses the constructor of the button
		active = true;// initialises the activity to true
	}

	

	void draw() override {// draws the control button to the wondow
		graphics::Brush br;
		br.outline_opacity = 0.0f;
		br.texture = std::string(ASSET_PATH) + name +".png";
		graphics::drawRect(pos_x, pos_y, size, size, br);
	}
};

class Slider : public Widget {// slider
private:
	int value=2000;// initialised value of the year will be 2000
	float val_pos_x = pos_x;// starting postion on x axis for the value will be the same as the slider's
public:
	Slider(float x, float y, std::string n) {// initialize the position and the name of the slider
		pos_x = x;
		pos_y = y;
		name = n;
	}

	void update(float x,float y) {// it updates the slider changing the value variable and the position of the value to a fixed spot
		if (x<pos_x + 80 && x>pos_x - 80 && y<pos_y + 15 && y > pos_y - 15) {
			active = true;
			for (int i = 1; i < 20; i++) {
				if (abs((pos_x + i * 8 - 80)-x)<4) {
					val_pos_x = pos_x + i * 8 - 80;
					value = 2000 + i - 10;
				}

			}

		}
	
	}

	void draw() override {// draw function that creates the slider on the window
		graphics::Brush br;
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.4f;// set colour to the brush
		br.fill_color[2] = 0.0f;
		br.outline_opacity = 0.0f;
		graphics::drawRect(pos_x , pos_y, 160.0f, 30.0f, br);// draw the body of the slider
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;
		graphics::setFont(std::string(ASSET_PATH) + "Oswald-Regular.ttf");//draw the name of the slider
		graphics::drawText(pos_x-80, pos_y-30, 30.0f, name, br);
		graphics::drawText(pos_x , pos_y + 30, 15.0f, std::to_string(value), br);//draw the number that value holds
		br.fill_color[0] = 0.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.0f;
		for (int i = 1; i < 20; i++) {
			graphics::drawDisk(pos_x-80+i*8,pos_y,2.0f,br);// draw all the possible places that the value can be
		}
		br.fill_color[0] = 0.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 1.0f;
		br.fill_opacity = active;
		graphics::drawDisk(val_pos_x , pos_y, 3.0f, br);// draw brighter the position of the value

	}

	void set_val(int i) {// a setter for value
		value = i;
	}

	int get_val() {// a getter for value
		return value;
	}
	void set_val_pos(float x) {// a setter for the position of value
		val_pos_x = x;
	}

};

