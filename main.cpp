//Mayank Karnati and Jake Browning
#define _USE_MATH_DEFINES
#include "FEHLCD.h"
#include <iostream>
#include <cmath>

/*
TO DO LIST:

JAKE:
    Done: 
        Tank.Aim(xComp, yComp)
    Half Done:
        Tank.Draw()
        myController.Touch()
    To Do:
        myController.takeTurn()
        Add tank turret positions to class
        Make the controls an object that works for both players
        Make the tank only fire after the button is pressed
        Split aim and move actions into hover/click

MAYANK:
    Done:
        Projectile.Fire()
        Projectile.calcShot()
        Projectile.Draw()
        myController.DisplayStats();
        ~struct Stats
    
    Half Done:
        myController.detectHit();

    To Do:
        draw Tower()
        draw Health();

*/

//Stats Struct
struct Statistics
{
    int tank1_shots_fired = 0;
    int tank2_shots_fired = 0;
    int tank1_shots_hit = 0;
    int tank2_shots_hit = 0;
    int tank1_wins = 0;
    int tank2_wins = 0;
};

//Projectile Class Definition - Mayank
class Projectile{
    public:
        Projectile();
        void Fire(float, float, float, float);
        void calcShot();
        void Draw();

    // private:
        float px; // X position
        float py; // Y position
        float vx; // X velocity
        float vy; // Y velocity
        float ay; // Y acceleration
};

// Function Prototypes
bool touchInBox(int, int, int, int, int, int);


//Tank Class Definition - Jake
class Tank{
    public:
        //Constructor
        Tank(float, float, unsigned int);
        Tank();
        void getLocation(float *, float *); //returns tanks location
        void getAngle(float *, float *); //returns components of angle
        void Draw(); // Draw the tank
        void Move(float, float); //Moves the tank
        void Aim(float, float); //Aims the tank 
        void Fire(); //Fires a shot
        //Out of private for testing purposes: - TEST CODE
        float turretX = 10; //turret x position relative to the x position
        float turretY = 8; //turret y position relative to the y position
        float turretR = .5; //turret radius
        float turretL = 15; //turret length
        float xPos; // x position 
        float yPos; // y position
        int width; // width of tank hitbox
        int height; // hieght of tank hitbox
    //private:
        /*Return xPos, yPos, width, hieght to here*/
        float xComponent = 1; //angle x component
        float yComponent = -1; //angle y component
        unsigned int color; //tank color
};

//Terrain Class Definition - Jake
class Terrain{
    public:
        void Draw(); //Draw function
        Terrain(int); //Flat terrain Constructor
    //private:
        int height;
        int terrainType;
};

//Button Class Definition - Jake
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


//GameController Class Definition - Jake
class GameController{
    public:
        void Draw(); //Draw function
        GameController(int, int); //Constructor 
        bool detectHit(); //Checks for projectile hit
        //void checkEnd(); //Checks for game end
        void Touch(int, int); //Input handling
        void Fire(); // Fires a shot
        int ReadyToFire; //Ready to Fire Flag
        int Turn; //Stores which player is to-move: 1 -> player 1, 2 -> player 2
        int Aiming; // Stores a player is aiming
        void DisplayStats(); // Displays Stats
        void calcShot();
        int Fired;
        void Move(int, int); // Moves Tank based on button pressed
    private:
        Terrain myTerrain; //Terrain object
        Tank myTank1; //Tank 1 object
        Tank myTank2; //Tank 2 object
        Projectile bullet1; // Tank 1's projectile
        Projectile bullet2; // Tank 2's projectile
        Button rightArrow = Button(220, 210, 70, 20, "Right");
        Button leftArrow = Button(10, 210, 70, 20, "Left");
        struct Statistics gameStats;
};

//Menu Class Definition - Mayank
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
    //Game state variable
    int gameOngoing = 0;

    //Mouse x and y variables
    int mx = 0, my = 0;
    
    //Main game controller object
    GameController myController(0,2);

    //Menu objects
    Menu Mainmenu; //Main menu object
    Button Return(230, 200, 60, 20, "Back"); // Back button to return to screen
    


    while (1) {
        // clears the previous frame
        LCD.Clear();
        
        //Go to apropriate window - Mayank
        switch (menuState)
        {
            case 0:
                Mainmenu.Draw();
                menuState = Mainmenu.Action();
                break;

            case 1:
                //Draw controller - J
                myController.Draw();
                //Wait for input - J
                while(!LCD.Touch(&mx,&my));
                //Handle input - J
                while(LCD.Touch(&mx, &my)){
                    myController.Move(mx, my);
                    myController.Touch(mx,my);
                    myController.Draw();
                }
                //Test if shot was lined up and fire if so
                if(myController.ReadyToFire == 1){
                    myController.Fire();
                    while (myController.Fired == 1){
                        myController.calcShot();
                        myController.Draw();
                    }
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
                myController.DisplayStats();
                
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
//Default Tank constructor - Jake
Tank::Tank(){}

//Tank Constructor - with parameters - Jake
Tank::Tank(float x, float y, unsigned int c = RED){
    //Copy variables into object
    color = c; //Tank Color
    //x and y positiion
    xPos = x;
    yPos = y;
    width = 20; // width of tank hitbox
    height = 20; // hieght of tank hitbox
}

//Draw - Jake
void Tank::Draw(){
    //Draw rectangle using color, position, and dimensions stored in tank object
    LCD.SetFontColor(color);
    LCD.FillRectangle(xPos,yPos + height/2, width, height/2);
    LCD.FillRectangle(xPos + width/4,yPos + width*1/4, width/2, height/3);
    
    
    //Draw turret - Working  but gross. Just do circle line circle for the love of god
    
    //Computation variables for sanity - Handle these as class members - DO THIS
    float a = xComponent;
    float b = yComponent;
    float r = turretR;
    float l = turretL;
    float offX = xPos + turretX;
    float offY = yPos + turretY;
    float lx =  l*a/(sqrt(pow(a,2)+pow(b,2)));
    float ly = l*b/(sqrt(pow(a,2)+pow(b,2)));
    
    LCD.DrawLine(offX, offY, offX + lx, offY + ly);
    
    //Handle near zero cases
    //Draw turret
    //Edges
    LCD.DrawLine(offX + r*a/abs(a)*cos(atan2(b,a)+ M_PI_2), offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a)+M_PI_2)), offX + r*a/abs(a)*cos(atan2(b,a)+ M_PI_2) + lx, offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a) + M_PI_2)) + ly);
    LCD.DrawLine(offX + r*a/abs(a)*cos(atan2(b,a)- M_PI_2), offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a)-M_PI_2)), offX + r*a/abs(a)*cos(atan2(b,a)- M_PI_2) + lx, offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a) - M_PI_2)) + ly);
    //Muzzle
    LCD.DrawLine(offX + r*a/abs(a)*cos(atan2(b,a)+ M_PI_2) + lx, offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a) + M_PI_2)) + ly, offX + r*a/abs(a)*cos(atan2(b,a)- M_PI_2) + lx, offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a) - M_PI_2)) + ly);
    LCD.DrawLine(offX + r*a/abs(a)*cos(atan2(b,a)+ M_PI_2), offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a) + M_PI_2)), offX + r*a/abs(a)*cos(atan2(b,a)- M_PI_2), offY + a*b/(abs(a*b))*r*sin((b/abs(b))*(atan2(b,a) - M_PI_2)));
}

//Aim - Jake
void Tank::Aim(float xComp, float yComp){
    //Copy in values
    xComponent = xComp;
    yComponent = yComp;
}
//Fire - Jake
void Tank::Fire(){
    
}

//Move - Jake
void Tank::Move(float dx, float dy){
    //Add change in position to current position
    xPos += dx; //x position
    yPos += dy; //y position
}

//Terrain Methods ------------------------

//Terrain Constructor - Jake
Terrain::Terrain(int h = 200){
    //Copy height value
    height = h;
    //set terrain type to default (0)
    terrainType = 0;
}

//Draw Method - Jake
void Terrain::Draw(){
    //Create white line
    LCD.SetFontColor(WHITE);
    LCD.DrawHorizontalLine(height,0,360); //Draw ground line
}

//GameController Methods ------------------------

//GameController Constructor - Jake
GameController::GameController(int terrainType = 0, int playerCount = 0){
    //Create Terrain and store it
    myTerrain = Terrain(200);

    //Create Tanks
    myTank1 = Tank(50,200-20, RED);
    myTank2 = Tank(200,200-20, BLUE);

    //Default gamestate values
    ReadyToFire = 0;
    Turn = 1;
    Aiming = 0;
}
//Draw Function - Jake
void GameController::Draw(){
    // clears the previous frame
    LCD.Clear();
    //Draw Terrain
    myTerrain.Draw();
    //Draw Tanks
    myTank1.Draw();
    myTank2.Draw();
    bullet1.Draw();
    //Draw Buttons
    rightArrow.Draw();
    leftArrow.Draw();
    if (Turn == 1){
        //Show controls for player 1
        LCD.SetFontColor(WHITE);
        //Sides
        LCD.DrawRectangle (myTank1.xPos + myTank1.width + 10, myTank1.yPos + myTank1.height/2 - 1, 2, 2 );
        LCD.DrawRectangle (myTank1.xPos - 10, myTank1.yPos + myTank1.height/2 - 1, 2, 2 );
        //Top
        LCD.DrawRectangle (myTank1.xPos + myTank1.width/2 -2 , myTank1.yPos - 8, 4, 4);
    }
    if (Turn == 2){
        //Show controls for player 2
        LCD.SetFontColor(WHITE);
        //Sides
        LCD.DrawRectangle (myTank2.xPos + myTank2.width + 10, myTank2.yPos + myTank2.height/2 - 1, 2, 2 );
        LCD.DrawRectangle (myTank2.xPos - 10, myTank2.yPos + myTank2.height/2 - 1, 2, 2 );
        //Top
        LCD.DrawRectangle (myTank2.xPos + myTank2.width/2 -2 , myTank2.yPos - 8, 4, 4 );
    }

    //Update Screen
    LCD.Update();
}

//Touch Function - Jake
void GameController::Touch(int mx, int my){
    //Start Aiming if box clicked
    if(Aiming == 0 && Turn == 1 &&  touchInBox(mx, my, myTank1.xPos + myTank1.width/2 -1 , myTank1.yPos - 10, 8, 8 )){
        //Update Statuses
        Aiming = 1;
        ReadyToFire = 1;
    }
    //Aim the Tank
    if(Aiming == 1){
        //Aim correct tank towards mouse
        if (Turn == 1){
            myTank1.Aim(mx - (myTank1.xPos + myTank1.turretX), my - (myTank1.yPos + myTank1.turretY));
        }
        if (Turn == 2){
            myTank2.Aim(mx - (myTank2.xPos + myTank2.turretX), my - (myTank2.yPos + myTank2.turretY)); 
        }
    }
}

//Fire Function - Jake
void GameController::Fire(){
    //update Status
    Aiming = 0;
    Fired = 1;
    ReadyToFire = 0;
    
    //Fire shot from correct tank
    if (Turn == 1){
        bullet1.Fire(myTank1.xPos+12, myTank1.yPos, 0.01*(myTank1.xComponent), 0.01*(myTank1.yComponent));
    }
    if (Turn == 2){
        bullet2.Fire(myTank2.xPos, myTank2.yPos, 0.01*(myTank2.xComponent), 0.01*(myTank2.yComponent));
    }
}

//CalcShot Function - Jake

void GameController::calcShot(){
    //Calculate shot trajectory
    bullet1.calcShot();    
}

//Detect Hit Function - Mayank
bool GameController::detectHit()
{
    // If the first player shoots
    if (Turn == 1)
    {   
        //Distance formula between first bullet and second tank
        if (sqrt(pow(bullet1.px - myTank2.xPos, 2) + pow(bullet1.py - myTank2.yPos, 2)) < 50)
        {
            return true; // returns true if hit
         
        // if bullet hits terrain instead
        } else if (bullet1.py >= myTerrain.height) {
            return false; // returns false for miss
        } 

    // If second player shoots
    } else if (Turn == 2) {

        //Distance formula between second bullet and first tank
        if (sqrt(pow(bullet2.px - myTank1.xPos, 2) + pow(bullet2.py - myTank1.yPos, 2)) < 50)
        {
            return true; // returns true if hit

        // if bullet hits terrain
        } else if (bullet2.py >= myTerrain.height) {
            return false; // returns false for miss
        }
    }
}

//Move Function - Mayank
void GameController::Move(int mx, int my)
{
    if (Turn == 1)
    {
        if (touchInBox(mx, my, rightArrow.x, rightArrow.y, rightArrow.w, rightArrow.h))
        {
            myTank1.Move(1, 0);
        }

        if (touchInBox(mx, my, leftArrow.x, leftArrow.y, leftArrow.w, leftArrow.h))
        {
            myTank1.Move(-1, 0);
        }
    }

    if (Turn == 2)
    {
        if (touchInBox(mx, my, rightArrow.x, rightArrow.y, rightArrow.w, rightArrow.h))
        {
            myTank2.Move(1, 0);
        }

        if (touchInBox(mx, my, leftArrow.x, leftArrow.y, leftArrow.w, leftArrow.h))
        {
            myTank2.Move(-1, 0);
        }
    }
}

// Display Stats Method - Mayank
void GameController::DisplayStats()
{
    LCD.WriteAt("Game Statistics:", 0, 0);

    LCD.SetFontColor(RED);
    LCD.WriteAt("Player 1", 0, 30);
    LCD.SetFontColor(BLUE);
    LCD.WriteAt("Player 2", 150, 30);

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Wins:", 0, 60);
    LCD.SetFontColor(RED);
    LCD.WriteAt(gameStats.tank1_wins, 70, 60);
    LCD.SetFontColor(BLUE);
    LCD.Write("");
    LCD.WriteAt(gameStats.tank2_wins, 100, 60);

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Shots Fired:", 0, 75);
    LCD.SetFontColor(RED);
    LCD.WriteAt(gameStats.tank1_shots_fired, 150, 75);
    LCD.SetFontColor(BLUE);
    LCD.Write("");
    LCD.WriteAt(gameStats.tank2_shots_fired, 180, 75);

    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Shots Hit:", 0, 90);
    LCD.SetFontColor(RED);
    LCD.WriteAt(gameStats.tank1_shots_hit, 130, 90);
    LCD.SetFontColor(BLUE);
    LCD.Write("");
    LCD.WriteAt(gameStats.tank1_shots_hit, 160, 90);

    LCD.SetFontColor(WHITE);
}
//Button Methods ------------------------

//Button Constructor - Mayank
Button::Button(int ix, int iy, int iw, int ih, char itext[10])
{
    //Copy variables in
    x = ix;
    y = iy;
    w = iw;
    h = ih;
    strcpy(text,itext);
}

//Draw Button - Mayank
void Button::Draw()
{
    LCD.SetFontColor(RED);
    LCD.DrawRectangle(x, y, w, h);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt(text, x+w/10, y+h/5);
}

//Return Button - Mayank
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

Menu::Menu() //constructor - Both?
{
    ButtonCount = 0;
}

//Menu Draw - Mayank
void Menu::Draw()
{
    LCD.WriteAt("Tanks 2.0", 90, 0);

    credits.Draw();
    playButton.Draw();
    howTo.Draw();
    stats.Draw();
}

//Menu Action - Mayank
int Menu::Action()
{
    int x, y;

    //Wait for touchscreen press
    while(!LCD.Touch(&x, &y));

    //Wait for touchscreen release
    while(LCD.Touch(&x, &y));

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


// Generic Functions - Mayank
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

// Projectile Methods ------------------------------------

// Projectile Constructor - Mayank
Projectile::Projectile()
{
    px = 0;
    py = 0;
    vx = 0;
    vy = 0;
    ay = +.01;
}

// Projectile Fire - Mayank
void Projectile::Fire(float px0, float py0, float vx0, float vy0)
{   
    px = px0;
    py = py0;
    vx = vx0;
    vy = vy0;
}

// Projectile Calc Shot - Mayank
void Projectile::calcShot()
{
    px += vx;
    py += vy;
    vy += ay;
}

// Projectile Draw - Mayank
void Projectile::Draw()
{
    LCD.SetFontColor(LCD.White);
    LCD.FillCircle(px, py, 1);
}
