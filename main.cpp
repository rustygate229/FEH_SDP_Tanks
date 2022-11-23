#include "FEHLCD.h"
#include <iostream>

//Button Class Definition
class Button{
    public:
        Button(int, int, int, int, char [10]); // constuctor
        void Draw();
        
    public:
        //Properties
        int x; // x position
        int y; // y position
        int w; // width
        int h; // height
        char text[10];
};

class Menu
{
    public:
        Menu();
        void Draw();

    private:
        Button credits = Button(50, 120, 100, 20, "Credits");
        Button playButton = Button(50, 60, 60, 20, "Play");
        Button howTo = Button(50, 80, 100, 20, "How To");
        Button quit = Button(50, 100, 100, 20, "Quit");

        int ButtonCount;
};

int main()
{
    Menu Mainmenu;
    Mainmenu.Draw();
    
    while (1) {
        LCD.Update();
        // Never end
    }
    return 0;
}

//Button Constructor
Button::Button(int ix, int iy, int iw, int ih, char itext[10])
{
    //Copy variables in
    x = ix;
    y = iy;
    w = iw;
    h = ih;
    strcpy(text,itext);
}

//Draw Button
void Button::Draw()
{
    LCD.DrawRectangle(x, y, w, h);
    LCD.WriteAt(text, x+w/10, y+h/10);
}

//Menu Methods

Menu::Menu() //constructor
{
    ButtonCount = 0;
}

//Menu Draw
void Menu::Draw()
{
    credits.Draw();
    playButton.Draw();
    howTo.Draw();
    quit.Draw();
}