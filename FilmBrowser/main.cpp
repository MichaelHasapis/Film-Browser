#include "graphics.h"
#include "FilmBrowser.h"
#include "config.h"

void update(float ms)// sets the update that will be called by the app on the loop
{
    FilmBrowser* film = FilmBrowser::getInstance();
    film->update();

}


void draw()// sets the draw that will be called by the app on the loop
{
    FilmBrowser* film = FilmBrowser::getInstance();
    film->draw();
}

int main()
{
    FilmBrowser* myNetflix; //create an instance of filmbrowser 

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Film Browser"); // create the window for the app to run 

    graphics::setUserData(&myNetflix);

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    myNetflix->init(); // initialise the instance of the filmbrowser

    graphics::startMessageLoop();


    return 0;
}