//Mayank Karnati and Jake Browning

#include "FEHLCD.h"
#include <iostream>




// Function Prototypes
bool touchInBox(int, int, int, int, int, int);


//Tank Class Definition
class Tank{
    public:
        //Constructor
        Tank(int, int, unsigned int);
        Tank();
        void getLocation(int *, int *); //returns tanks location
        void getAngle(float *, float *); //returns components of angle
        void Draw(); // Draw the tank
    private:
        int xPos; // x position
        int yPos; // y position
        int width; // width of tank hitbox
        int hieght; // hieght of tank hitbox
        float xAngle; //angle x component
        float yAngle; //angle y component
        unsigned int color; //tank color
};

//Terrain Class Definition
class Terrain{
    public:
        void Draw(); //Draw function
        Terrain(int); //Flat terrain Constructor
    private:
        int height;
        int terrainType;
};

//GameController Class Definition
class GameController{
    public:
        void Draw(); //Draw function
        GameController(int, int); //Constructor 
    private:
        Terrain myTerrain; //Terrain object
        Tank myTank1; //Tank 1 object
        Tank myTank2; //Tank 2 object
};

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

//Menu Class Definition
class Menu
{
    public:
        Menu();
        void Draw();
        int Action();

    private:
        Button playButton = Button(125, 60, 60, 25, "Play");
        Button howTo = Button(115, 110, 90, 25, "How To");
        Button credits = Button(110, 160, 100, 25, "Credits");
        Button stats = Button(90, 210, 140, 25, "Statistics");

        int ButtonCount; // Stores the number of buttons
};


//Main ------------------------

int main()
{
    // 0: Menu, 1: Play, 2: How To, 3: Credits, 4: Stats
    int menuState = 0;
    int gameOngoing = 0;
    GameController myController(0,2);

    Menu Mainmenu;
    Button Return(200, 200, 85, 20, "Return");
    
    while (1) {
        // clears the previous frame
        LCD.Clear();
        
        switch (menuState)
        {
            case 0:
                Mainmenu.Draw();
                menuState = Mainmenu.Action();
                break;

            case 1:
                //Draw Controller
                myController.Draw();


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
                    menuState = 0;
                }
                
                break;

            case 3:
                LCD.WriteLine("Game made by Mayank");
                LCD.WriteLine("Karnati and Jake Browning");
                
                if(Return.Return())
                {
                    menuState = 0;
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
                    menuState = 0;
                }
        }

        LCD.Update(); //Draws the new frame
        // Never end
    }
    return 0;
}

//Tank Methods ------------------------
//Default Tank constructor
Tank::Tank(){}

//Tank Constructor - with parameters
Tank::Tank(int x, int y, unsigned int c = RED){
    //Copy variables into object
    color = c; //Tank Color
    //x and y positiion
    xPos = x;
    yPos = y;
    width = 10; // width of tank hitbox
    hieght = 10; // hieght of tank hitbox
}
//Draw
void Tank::Draw(){
    LCD.SetFontColor(color);
    LCD.DrawRectangle(xPos,yPos, width, hieght);
}


//Terrain Methods ------------------------

//Terrain Constructor
Terrain::Terrain(int h = 200){
    //Copy height value
    height = h;
    //set terrain type to default (0)
    terrainType = 0;
}

//Draw Method
void Terrain::Draw(){
    //Create white line
    LCD.SetFontColor(WHITE);
    LCD.DrawHorizontalLine(height,0,360); //Draw ground line
}

//GameController Methods ------------------------

//GameController Constructor
GameController::GameController(int terrainType = 0, int playerCount = 0){
    //Create Terrain and store it
    myTerrain = Terrain(200);

    //Create Tanks
    myTank1 = Tank(50,200-10, BLUE);
    myTank2 = Tank(200,200-10, RED);

}
//Draw Function
void GameController::Draw(){
    //Draw Terrain
    myTerrain.Draw();
    //Draw Tanks
    myTank1.Draw();
    myTank2.Draw();
}

//Button Methods ------------------------

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
    LCD.SetFontColor(RED);
    LCD.DrawRectangle(x, y, w, h);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt(text, x+w/10, y+h/5);
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

//Menu Methods ------------------------

Menu::Menu() //constructor
{
    ButtonCount = 0;
}

//Menu Draw
void Menu::Draw()
{
    LCD.WriteAt("Tanks 2.0", 90, 0);

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
        //While (!lcd.touch();) - Button release code
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
