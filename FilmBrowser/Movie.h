#pragma once
#include <list>




class Movie   {// class for the movies
private:	
	std::string name; // name of the movie 
	float pos_x = 0.0f;// position on x axis
	float pos_y = 0.0f;// position on y axis
	float height = 150.0f;// height of the rectangle that will be drawn
	float width = 100.0f;// width of the rectangle that will be drawn
	int date;// year of the movie that was produced
	std::list<std::string> director;// list with the directors
	std::list<std::string> protagonist;// list with the protagonists
	std::list<std::string> genres;// list with the genres of the movie
	std::string summary;// summary of the movie
	std::list<std::string> tokens;// list to save the broken in pieces summary


public:

	Movie(std::string n, int d, float x, float y,std::string sum) {// constructor of the movie
		pos_x = x;// position on x axis
		pos_y = y;// position on y axis
		name = n;// name of the movie
		date = d;// year that was produced
		summary = sum;// summary of the movie
	}

	std::string get_Name() {// return the name of the movie
		return name;
	}

	int get_date() {// returns the year that the movie was released
		return date;
	
	}

	std::list<std::string> get_Director() {// returns a list with the direcotrs of the movie
		return director;
	}

	std::list<std::string> get_Protagonists() {// returns a list with the protagonists of the movie
		return protagonist;
	}

	void addDir(std::string dir) {// adds a director to the list of directors 
		director.push_back(dir);
	}

	void addPr(std::string pr) {// adds a protagonits to the list of protagonists
		protagonist.push_back(pr);
	}

	void addGenre(std::string g) {// adds a genre to the list of genres
		genres.push_back(g);
	}

	bool checkGen(std::string gen) {// check if a genre is in list
		return std::find(std::begin(genres), std::end(genres), gen) != std::end(genres);
	}
	bool checkProt(std::string pr) {// check if a protagonist is in list
		return std::find(std::begin(protagonist), std::end(protagonist), pr) != std::end(protagonist);
	}
	bool checkDir(std::string dir) {// check if a director is in list
		return std::find(std::begin(director), std::end(director), dir) != std::end(director);
	}

	void setx(float x) {// sets the position of the movie on the x axis
		pos_x = x;
	}

	void sety(float y) {// sets the postiion of the movie on the y axis
		pos_y = y;
	}

	bool pressed(float x, float y) {// cehcks if a movie was pressed by the mouse
		if (x<pos_x + width/2 && x>pos_x - width/2 && y<pos_y + height/2 && y > pos_y - height/2) {
			return true;
		}
		return false;
	}

	void break_summary() {// breaks the summary to smaller strings for better display
		std::string s = summary;
		while ((int)(s.size() / 70) > 0) {
			tokens.push_back(s.substr(0, 70));
			s = s.substr(70, s.size());
		}
		tokens.push_back(s);
		
	}

	void draw() {// draw the movie as a small rectangle 
		graphics::Brush br;
		br.outline_opacity = 0.0f;
		br.texture = std::string(ASSET_PATH) + name + ".png";
		graphics::drawRect(pos_x, pos_y, width, height, br);
		
	}

	void showing_draw() {// draws the movie and all the important details of it , rectangle is bigger
		pos_x = 200;
		pos_y = CANVAS_WIDTH / 2-300;
		graphics::Brush br;
		br.outline_opacity = 0.0f;
		br.texture = std::string(ASSET_PATH) + name + ".png";
		graphics::drawRect(pos_x, pos_y, width*2, height*2, br);
		graphics::setFont(std::string(ASSET_PATH) + "Oswald-Regular.ttf");
		graphics::drawText(pos_x+150,pos_y-100,30,name,br);
		
		br.fill_color[0] = 0.0f ;
		br.fill_color[1] = 1.0f ;
		br.fill_color[2] = 0.0f ;
		graphics::drawText(pos_x + 150, pos_y - 50, 20, "Directors:", br);
		graphics::drawText(pos_x + 500, pos_y - 50, 20, "Genres:", br);
		graphics::drawText(pos_x + 150, pos_y + 50, 20, "Protagonists:", br);
		graphics::drawText(pos_x + 150, pos_y + 150, 20, "Summary:", br);
		graphics::drawText(pos_x + 500, pos_y +30 , 20, "Year:", br);
		
				
		graphics::drawText(pos_x+ 600, pos_y+30, 15.0f, std::to_string(date), br);

		int i = 0;
		for (auto d : director) {
			graphics::drawText(pos_x + 300, pos_y - 50+(i*20), 15,d , br);
			i++;
		}
		i = 0;
		for (auto g : genres) {
			graphics::drawText(pos_x + 600, pos_y - 50 + (i * 20), 15, g, br);
			i++;
		}
		i = 0;

		for (auto p : protagonist) {
			graphics::drawText(pos_x + 300, pos_y + 50 + (i * 20), 15, p, br);
			i++;
		}
		i = 0;
		for (auto s: tokens) {
			graphics::drawText(pos_x + 300, pos_y + 150 + (i * 20), 15, s, br);
			i++;
		}

	}

};


