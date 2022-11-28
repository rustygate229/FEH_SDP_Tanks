//Mayank Karnati and Jake Browning

#include "FEHLCD.h"
#include <iostream>



// Function Prototypes
bool touchInBox(int, int, int, int, int, int);


//Button Class Definition
class Button{
    public:
        Button(int, int, int, int, char [10]); // constuctor
        void Draw();
        int Return();
        
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
        Button stats = Button(50, 120, 140, 20, "Statistics");

        int ButtonCount;
};

int main()
{
    // 0: Menu, 1: Play, 2: How To, 3: Credits, 4: Stats
    int gameState = 0;

    //variables to display text once
    int once = 1;

    Menu Mainmenu;
    Button Return(200, 200, 85, 20, "Return");
    
    while (1) {
        // clears the previous frame
        LCD.Clear();
        
        switch (gameState)
        {
            case 0:
                Mainmenu.Draw();
                gameState = Mainmenu.Action();
                break;

            case 1:
                LCD.WriteAt("Play Game Here", 10, 10);
                
                if(Return.Return())
                {
                    gameState = 0;
                }

                break;

            case 2:
                LCD.WriteLine("Here are the instructions:");
                LCD.WriteLine("There are 2 players, each with 100 hp");
                LCD.WriteLine("Move the tank using the");
                LCD.WriteLine("arrows");
                LCD.WriteLine("Make a vector by clicking and dragging");
                LCD.WriteLine("to shoot the projectile");
                LCD.WriteLine("and release");
                LCD.WriteLine("First to lose all hp loses");
                
                if(Return.Return())
                {
                    gameState = 0;
                }
                
                break;

            case 3:
                LCD.WriteLine("Game made by Mayank");
                LCD.WriteLine("Karnati and Jake Browning");
                
                if(Return.Return())
                {
                    gameState = 0;
                }
                
                break;

            case 4:
                LCD.WriteLine("Game Statistics: \n");
                LCD.WriteLine("Time: ");
                LCD.WriteLine("Winner: ");
                LCD.WriteLine("Shots Fired: ");
                LCD.WriteLine("Shots Hit: ");
                
                if(Return.Return())
                {
                    gameState = 0;
                }
        }

        LCD.Update(); //Draws the new frame
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

//Return Button
//returns 1 if the return button is clicked, else 0
//only use this method for the return button
int Button::Return()
{
    int touch_x, touch_y;

    LCD.DrawRectangle(x, y, w, h);
    LCD.WriteAt(text, x+w/10, y+h/10);

    if (LCD.Touch(&touch_x, &touch_y))
    {
        if (touchInBox(touch_x, touch_y, x, y, w, h))
        {
            return 1;
        }
    }

    return 0;
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
    stats.Draw();
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
        if (touchInBox(x, y, stats.x, stats.y, stats.w, stats.h))
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
