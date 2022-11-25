#include "FEHLCD.h"
#include <iostream>

// Function Prototypes
bool touchInBox(int, int, int, int, int, int);


//Button Class Definition
class Button{
    public:
        Button(int, int, int, int, char [10]); // constuctor
        void Draw();
        int Action();
        
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
        int Action();

    private:
        Button credits = Button(50, 100, 100, 20, "Credits");
        Button playButton = Button(50, 60, 60, 20, "Play");
        Button howTo = Button(50, 80, 100, 20, "How To");
        Button quit = Button(50, 120, 100, 20, "Quit");

        int ButtonCount;
};

int main()
{
    // 0: Menu, 1: Play, 2: How To, 3: Credits, 4: Quit
    int gameState = 0;

    int gameOn = 1;

    Menu Mainmenu;
    
    while (gameOn) {
        
        switch (gameState)
        {
            case 0:
                Mainmenu.Draw();
                gameState = Mainmenu.Action();

                if (gameState != 0)
                {
                    LCD.Clear();
                }

                break;
            case 1:
                LCD.WriteAt("Here should be the game", 10, 10);
                break;
            case 2:
                LCD.WriteAt("Here are the instructions", 10, 10);
                break;
            case 3:
                LCD.WriteAt("Game Made by Mayank and Jake", 10, 10);
            case 4:
                gameOn = 0;
        }

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
    LCD.WriteAt("Tanks 2.0", 50, 20);

    credits.Draw();
    playButton.Draw();
    howTo.Draw();
    quit.Draw();
}

//Menu Action
int Menu::Action()
{
    int x, y;
    if (LCD.Touch(&x, &y))
    {
        if (touchInBox(x, y, playButton.x, playButton.y, playButton.w, playButton.h))
        {
            return 1;
        }
        if (touchInBox(x, y, howTo.x, howTo.y, howTo.w, howTo.h))
        {
            return 2;
        }
        if (touchInBox(x, y, credits.x, credits.y, credits.w, credits.h))
        {
            return 3;
        }
        if (touchInBox(x, y, quit.x, quit.y, quit.w, quit.h))
        {
            return 4;
        }
    }
    
}


// Generic Functions
bool touchInBox(int x, int y, int box_x, int box_y, int box_w, int box_h)
{
    if (x > box_x && x < (box_x + box_w))
    {
        if (y > box_y && y < (box_y + box_h))
        {
            return true;
        }
    }

    return false;
}
