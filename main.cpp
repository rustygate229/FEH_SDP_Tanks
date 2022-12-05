// Mayank Karnati and Jake Browning
// SDP Project: Tanks 2.0
// Ben Grier 12:40
// Team D1: Sony
// 12/05/2022
// the Ohio State University 
// Fundamentals of Engineering Honors 
// ENGR 1281.02H, AU 2022

#define _USE_MATH_DEFINES // used for pi
#define DAMAGE 34 
#include "FEHLCD.h" 
#include <iostream> 
#include <cmath> 
#include <FEHUtility.h>  
#include <FEHImages.h> 

//Stats Struct - mayank
// Purpose - store the statistics for the games
struct Statistics
{
    int tank1_shots_fired = 0; // Number of shots fired by player 1
    int tank2_shots_fired = 0; // Number of shots fired by player 2
    int tank1_shots_hit = 0; // Number of shots landed on a tank by player 1
    int tank2_shots_hit = 0; // Number of shots landed on a tank by player 2
    int tank1_wins = 0; // Number of games won by player 1
    int tank2_wins = 0; // Number of games won by player 2
};

//Crown Class - Jake
// Manages the crown that indicates who won the last round of the game
// The crown is drawn only after the first round and is drawn on top of the tank that won the previous round for the entire next round
class Crown{
    public:
        void Draw(int, int); //Draw the crown at x , y
        // where to draw if player 1 is the winner: x = 110
        // where to draw if player 2 is the winner: x = 280
    private:
        bool design[4][8] = {
            1,0,0,0,0,0,0,1,
            1,1,0,1,1,0,1,1,
            1,1,1,1,1,1,1,1,
            1,1,1,1,1,1,1,1
        }; //truth table for the design of the crown - inputted x,y is the top-left pixel, 1 means draw that pixel, 0 means don't  
};

//Projectile Class Definition - Mayank
//Purpose - Manage projectiles across the screen
//The object takes no parameters. The constructor initializes position and velocity values to 0, and acceleration to 0.2.
//Fire method records starting position and velocity of projectile based on the input.
//calcShot method calculates the next position of the projectile in the next frame.
//Draw Method draws the projectile on the screen.
//px - X position of the projectile. py - Y position of the projectile
//vx - X velocity of projectile. vy - Y velocity of projectile.
//ay - Y acceleration of projectile.
class Projectile{
    public:
        Projectile(); // Constructor
        void Fire(float, float, float, float); //Initializes a shot
        void calcShot(); //calculates the next position of the projectile in the next frame.
        void Draw(); //Draws the projectile

    // private:
        float px; // X position
        float py; // Y position
        float vx; // X velocity
        float vy; // Y velocity
        float ay; // Y acceleration
};

// Function Prototypes - Mayank
// Purpose - to detect whether a point is within a rectangle
bool touchInBox(int, int, int, int, int, int);


//Tank Class Definition - Jake
//Purpose - Manage the tank and all related variables and methods
//Each tank is properly intialized with a starting x,y and color variables
//The width and height are set to 20 and 20 by default
//Fire method records starting position and velocity of projectile based on the input.
//calcShot method calculates the next position of the projectile in the next frame.
//Draw Method draws the projectile on the screen.
class Tank{
    public:
        //Methods
        Tank(float, float, unsigned int); //Constructor with x, y, and color
        Tank(); //default constructor
        void calcTurret(); //Calulate turret computation variables: lx, ly, offX, offY
        void Draw(); // Draw the tank
        void Move(float, float); //Moves the tank
        void Aim(float, float); //Aims the tank 
        void drawHealth(int); //Draws the tank's health bar
        //Variables
        float turretX = 10; //turret x position relative to the x position
        float turretY = 8; //turret y position relative to the y position
        float turretR = .5; //turret radius
        float turretL = 15; //turret length
        float xPos; // x position of the tank
        float yPos; // y position of the tank
        int width; // width of tank hitbox
        int height; // height of tank hitbox
        int health; // the tank's current health out of 100
        float offX, offY, lx, ly; //internal computational variables, the x,y of the center of the turret's 
        //and the x and y components of a vector from the center of the turret to the end of the turret
        float xComponent = 1; //turret angle x component
        float yComponent = -1; //turret angle y component
        unsigned int color; //tank color
};

//Terrain Class Definition - Jake
//Purpose - manages information about the terrain
//Draws the background and the landscape
//Is used to handle collisions with projectiles and decide where tanks are placed
class Terrain{
    public:
        void Draw(); //Draw function
        Terrain(int); //Flat terrain Constructor
    //private:
        FEHImage tower; // The image of the tower
        int height; // the y value that the terrain is drawn at
        int terrainType; // terrain type variable - Not used in this version but could be used to add more terrain types 
        int towerX, towerY; //Tower x and y position
};

//Button Class Definition - Jake and Mayanak
//Purpose - manages information about the buttons drawn in the menus
//Can draw itself
//Can check if clicked to allow a return to the previous screen in the menus
class Button{
    public:
        Button(int, int, int, int, char [10]); // constuctor, takes x,y for position of button, w,h for width/height, and a char array for the text on the button
        void Draw(); //draws the button
        int Return(); //used to return to a previous screen, returns 1 if the return button is clicked, else 0
    public:
        //Properties
        int x; // x position
        int y; // y position
        int w; // width
        int h; // height
        char text[10]; // Text on button
};


//GameController Class Definition - Jake
//Purpose - Manages and calls all game objects and methods, manages statistics and gamestates
//Directly or indirectly controls all game actions, end conditions, initialization, movement of tanks, turns, health
//Draws all game objects
class GameController{
    public:
        //Methods
        GameController(int, int); //Constructor, takes two variables, terrain and player count, neither is full implemented
        bool detectHit(); //Checks for projectile hit, calculates damage done on projectile collision
        bool checkEnd(); //Checks for game end - if one tank has <=0 health, the game ends)
        void Start(); //Creates initial gamestate - sets variables in member objects to default values, sets gamestates)
        void Draw(); //Draw function - Draws all gameobjects
        void takeTurn(); //Updates turn variable
        void Touch(int, int); //Input handling - takes the x and y position of the click and responds depending on where a click is registered
        void Fire(); // Initializes  a shot from the tank that is taking its turn
        void Move(int, int); // Moves Tank based on button pressed - takes and x and y that represent change in x and y respectively
        void DisplayStats(); // Displays Stats
        void displayWinner(int); // Displays victory screen for the winning player
        void calcShot(); // Calculates the projectile path for a fired bullet
        //Variables
        int ReadyToFire; //Ready to Fire Flag - stores if a tank has a shot lined up
        int Turn; //Stores which player is to-move: 1 -> player 1, 2 -> player 2
        int Aiming; // Stores a player is aiming
        int Fired; //Stores whether a shot is in the air
        int Winner; //stores winner 1: Red (player 1), 2: Blue (player 2)
    private:
        Crown myCrown; //Crown Object - placed on top of the winning tank
        Terrain myTerrain; //Terrain object - the terrain for the game
        Tank myTank1; //Tank 1 object - player 1's tank
        Tank myTank2; //Tank 2 object - player 2's tank
        Projectile bullet1; // Tank 1's projectile
        Projectile bullet2; // Tank 2's projectile
        Button rightArrow = Button(220, 210, 70, 20, "Right"); // Right movement button
        Button leftArrow = Button(10, 210, 70, 20, "Left"); // Left movement button
        struct Statistics gameStats; // Games statistics
};

//Menu Class Definition - Mayank
//Purpose - to manage the elements in a menu
//Menu Constructor - default constructor
//Draw Method - draws the menu page along with buttons
//Action - checks for button click and navigates to appropriate page
//Buttons - 4 buttons for each page made from Button class
class Menu
{
    public:
        Menu(); // Constructor
        void Draw(); // Draw
        int Action(); // Performs action based on click

    private:
        //Buttons
        Button playButton = Button(125, 60, 60, 25, "Play");  // Play button
        Button howTo = Button(115, 110, 90, 25, "How To"); // Instructions button
        Button credits = Button(110, 160, 100, 25, "Credits"); // Credits button
        Button stats = Button(90, 210, 140, 25, "Statistics"); // Stats button
};


//Main ------------------------

int main()
{
    //Initialize variables
    int menuState = 0; // 0: Menu, 1: Play, 2: How To, 3: Credits, 4: Stats, 5: Winner Screen
    int gameOngoing = 0; //Game state variable: 0 -> not playing, 1 -> playing
    int winner = 0; //Winner variable: 0 -> no winner, 1 -> player 1, 2 -> player 2
    int mx = 0, my = 0; //Mouse x and y variables
    
    //Initialize objects 
    GameController myController(0,2); //Main game controller object
    //Menu objects
    Menu Mainmenu; //Main menu object
    Button Return(230, 200, 60, 20, "Back"); // Back button to return to screen
    
    LCD.SetBackgroundColor(LIGHTBLUE); //Set background color

    //Main control loop, runs until code is externally terminated
    while (1) {
        LCD.Clear(); // clears the previous frame
        
        //Go to apropriate window - Mayank
        switch (menuState)
        {
            // Main Menu Page
            case 0:
                Mainmenu.Draw(); // draw main menu
                menuState = Mainmenu.Action(); // returns the new page
                break;

            //Game Page - Jake
            case 1:
                //Initialize game if the game is not ongoing
                if (gameOngoing == 0){
                    //Start game
                    myController.Start(); //Call controller start function 
                    gameOngoing = 1; //Update game status
                }
                myController.Draw(); //Draw all game objects
                //Wait for input
                while(!LCD.Touch(&mx,&my));
                //Handle input
                while(LCD.Touch(&mx, &my)){
                    myController.Touch(mx,my); //Handle input
                    myController.Draw(); //Draw all game objects
                }
                //Test if shot was lined up and fire if so
                if(myController.ReadyToFire == 1){
                    myController.Fire(); //Initialize a shot
                    //Run until the shot has landed
                    while (myController.Fired == 1){
                        myController.calcShot(); //Calculate the next position for the shot
                        myController.detectHit(); //check if the shot has landed
                        myController.Draw(); //Draw all game objects
                    }
                    myController.takeTurn(); //End turn
                    //Check if game should end
                    if(myController.checkEnd() == true){
                        winner = myController.Winner; //Update winner varible
                        gameOngoing = 0; //set game state to stopped
                        menuState = 5; //Switch to victory screen
                    }
                }
                break;
            //Instruction Page
            case 2:
                //Write instructions
                LCD.WriteLine("There are 2 players, each with 100 hp");
                LCD.WriteLine("Move the tank using the");
                LCD.WriteLine("movement buttons");
                LCD.WriteLine("To shoot the projectile, ");
                LCD.WriteLine("click in the circle, drag out in a direction");
                LCD.WriteLine("and release");
                LCD.WriteLine("Shots will wraparound the screen");
                LCD.WriteLine("First to lose all hp loses");
                
                // Return Button back to main menu
                if(Return.Return())
                {
                    menuState = 0; // switch to main menu screen
                }
                
                break;
            //Credits Page
            case 3:
            //Write Credits
                LCD.WriteLine("Game made by Mayank");
                LCD.WriteLine("Karnati and Jake Browning");
                LCD.WriteLine("Tower icon is a courtesy");
                LCD.WriteLine("of Flaticon.");
                LCD.WriteLine("Drawline algorithm sourced from:");
                LCD.WriteLine("https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm.");

                // Return Button back to main menu
                if(Return.Return())
                {
                    menuState = 0; // switch to main menu screen
                }
                
                break;

            //Stats Page
            case 4:
                myController.DisplayStats(); // Display game stats stored in gamecontroller object 
                
                // Return Button back to main menu
                if(Return.Return())
                {
                    menuState = 0; // switch to main menu screen
                }
                break;

            //Winner Page
            case 5:
                LCD.Clear(); // Clear previous frame
                myController.displayWinner(winner); //Displays Winner based on who won

                // Return Button back to main menu
                if(Return.Return())
                {
                    menuState = 0; // switch to main menu screen
                }
        }

        LCD.Update(); //Draws the new frame
        // Never end
    }
    return 0;
}

//Crown Methods ------------------------

//Crown Draw - Jake
//Draw the crown relative to a given x,y point
void Crown::Draw(int sx, int sy){
    LCD.SetFontColor(GOLD); //Set Color
    //Draw crown relative to given points
    //For each column
    for (int x = 0; x < 8; x++){
        //For each row
        for (int y = 0; y < 4; y++){
            //Draw a pixel if the design array says to at that point
            if (design[y][x] == true){
                LCD.DrawPixel(sx + x, sy + y); //draw the pixel at column value + x offset value, row value + y offset value 
            }
        }
    }
}



//Tank Methods ------------------------
//Default Tank constructor - Jake
//So that the Gamecontroller doesn't error before tanks are created
Tank::Tank(){}

//Tank Constructor - with parameters - Jake
//Takes x,y position and a color, red by default.
Tank::Tank(float x, float y, unsigned int c = RED){
    //Copy variables into object
    color = c; //Tank Color
    xPos = x; // x position
    yPos = y; // y position
    width = 20; // width of tank hitbox
    height = 20; // hieght of tank hitbox
    health = 100; // health of tank
}

//Calc Turret - Jake
//Calculates computational variables used by the gamecontroller and tank to perform various task related to the turret
//such as aiming, drawing it, and firing a projectile
//Desmos used to visualize the math, see website for more detail
void Tank::calcTurret(){
    //Calculate computation variables using input variables
    offX = xPos + turretX;
    offY = yPos + turretY;
    lx =  turretL*xComponent/(sqrt(pow(xComponent,2)+pow(yComponent,2)));
    ly = turretL*yComponent/(sqrt(pow(xComponent,2)+pow(yComponent,2)));
    
    //Avoid divide by zero
    //If the y component vector is near zero relative to the turret length
    if((int)ly == 0){
        //slighly adjust the component and the scaled component vectors
        ly = .01*abs(lx); // scaled
        yComponent += .01; // unscaled
    }
    if((int)lx == 0){
        //slighly adjust the component and the scaled component vectors
        lx = .01*abs(ly); // scaled
        xComponent += .01; // unscaled
    }
}

//Draw - Jake
//Draws the tank treads, body, and turret
//Desmos used to visualize the math, see website for more detail
//Modified drawline is an implementation of the pseudocode from wikipedia: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Tank::Draw(){
    //Draw back of the treads
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(xPos + width/8, yPos + height/2, width - 2*width/8, height/2);
    
    //Draw Wheels
    LCD.SetFontColor(GRAY);
    LCD.FillCircle(xPos + width/8, yPos + 7*height/8, height/8);
    LCD.FillCircle(xPos + 3*width/8, yPos + 7*height/8, height/8);
    LCD.FillCircle(xPos + 5*width/8, yPos + 7*height/8, height/8);
    LCD.FillCircle(xPos + 7*width/8, yPos + 7*height/8, height/8);
    
    //Draw Treads
    LCD.SetFontColor(BLACK);
    LCD.DrawCircle(xPos + width/8, yPos + 7*height/8, height/8);
    LCD.DrawCircle(xPos + 3*width/8, yPos + 7*height/8, height/8);
    LCD.DrawCircle(xPos + 5*width/8, yPos + 7*height/8, height/8);
    LCD.DrawCircle(xPos + 7*width/8, yPos + 7*height/8, height/8);
    
    //Draw body
    LCD.SetFontColor(color); //Set font color to tank's color
    LCD.FillRectangle(xPos,yPos + height/2, width, height/2.5); //Draw rectangle using color, position, and dimensions stored in tank object
    
    //Draw turret
    calcTurret(); //Calculate turret computation variables   
    
    //Turret Base
    LCD.FillRectangle(xPos + width/4, yPos + height/4, width/2, height/4);
    
    //Turret Barrel - aims at xComponent, yComponent
    //Centerline
    LCD.DrawLine(offX, offY, offX + lx, offY + ly);
    //Edges - See desmos link if you want to see how this math works. In the desmos rendering, a = xComponent, b = y Component, offX = 0, offY = 0, l = turretL, r = turretR
    LCD.DrawLine(offX + turretR*xComponent/abs(xComponent)*cos(atan2(yComponent,xComponent)+ M_PI_2), offY + xComponent*yComponent/(abs(xComponent*yComponent))*turretR*sin((yComponent/abs(yComponent))*(atan2(yComponent,xComponent)+M_PI_2)), offX + turretR*xComponent/abs(xComponent)*cos(atan2(yComponent,xComponent)+ M_PI_2) + lx, offY + xComponent*yComponent/(abs(xComponent*yComponent))*turretR*sin((yComponent/abs(yComponent))*(atan2(yComponent,xComponent) + M_PI_2)) + ly);
    LCD.DrawLine(offX + turretR*xComponent/abs(xComponent)*cos(atan2(yComponent,xComponent)- M_PI_2), offY + xComponent*yComponent/(abs(xComponent*yComponent))*turretR*sin((yComponent/abs(yComponent))*(atan2(yComponent,xComponent)-M_PI_2)), offX + turretR*xComponent/abs(xComponent)*cos(atan2(yComponent,xComponent)- M_PI_2) + lx, offY + xComponent*yComponent/(abs(xComponent*yComponent))*turretR*sin((yComponent/abs(yComponent))*(atan2(yComponent,xComponent) - M_PI_2)) + ly);

    //Round courners to make tank less blocky
    //Turret
    LCD.SetFontColor(LIGHTBLUE); //Backgound color - this is bad practice butt it is somewhat unavoidable without restructuring how objects are drawn
    LCD.DrawPixel(xPos + width/4, yPos + height/4); // Top right
    LCD.DrawPixel(xPos + 3*width/4 - 1, yPos + height/4); // Top left
    
    //Tank body
    LCD.DrawPixel(xPos, yPos + height/2); // Top right
    LCD.DrawPixel(xPos + width - 1, yPos + height/2); // Top left
    
}

//Aim - Jake
//Aims the tank's turret at the x and y values inputted
void Tank::Aim(float xComp, float yComp){
    //Copy in values
    xComponent = xComp; // x component vector
    yComponent = yComp; // y component vector
}


//Move - Jake
//Moves the tank dx and dy pixels in the horizontal and vertical directions, respectively
void Tank::Move(float dx, float dy){
    //Add change in position to current position
    xPos += dx; //x position
    yPos += dy; //y position
}

// Draw Health - Mayank
// Draws the healthbars for both tanks
// a - which tank's health must be drawn
void Tank::drawHealth(int a)
{
    if (a == 1) // Red tank
    {   
        // Draws red tank's health
        LCD.SetFontColor(RED);
        LCD.WriteAt("Red:", 0, 0);
        LCD.FillRectangle(50, 0, health / 2, 15);
        LCD.SetFontColor(WHITE);
    }
    else if (a == 2) // Blue tank
    {   
        // Draws Blue Tank's health
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Blue:", 160, 0);
        LCD.FillRectangle(220, 0, health / 2, 15);
        LCD.SetFontColor(WHITE);
    }
}

//Terrain Methods ------------------------

//Terrain Constructor - Jake
//initializes a flat terrain with a specified height (default h = 200)
Terrain::Terrain(int h = 200){
    height = h; //Copy height value
    terrainType = 0; //set terrain type to default (0), only 1 type implemented
    //set tower's coordinates
    towerX = 105; //x coord
    towerY = 100; //y coord
}

//Draw Method - Jake
//Draws the terrain, includes the sky, ground, and tower
//Tower image sourced from https://www.flaticon.com/free-icon/tower_909428?term=tower&page=1&position=14&page=1&position=14&related_id=90942
void Terrain::Draw(){
    //Create sky
    LCD.SetFontColor(LIGHTBLUE); //sky color
    LCD.FillRectangle(0,0,320, height); //Draw  sky
    //Create ground
    LCD.SetFontColor(DARKGREEN); // ground color
    LCD.FillRectangle(0,height,320, 240-height); //Draw ground 
    //Create Tower
    tower.Open("towerFEH.pic"); // Open image of the tower
    tower.Draw(towerX, towerY); // draw tower at its position
    tower.Close(); // Close the image of the tower
}

//GameController Methods ------------------------

//GameController Constructor - Jake
GameController::GameController(int terrainType = 0, int playerCount = 0){
    //Create game objects
    //Create graphics objects
    myCrown = Crown(); //Create a crown and store it
    myTerrain = Terrain(200); //Create Terrain and store it
    //Create Tanks
    myTank1 = Tank(50, myTerrain.height-myTank1.height, RED); //Player 1's Tank
    myTank2 = Tank(250, myTerrain.height-myTank2.height, BLUE); //Player 2's Tank

    //Default gamestate values
    ReadyToFire = 0; // No tank has a shot lined up by default
    Turn = 1; // player 1 always goes first
    Aiming = 0; // no tank starts aiming
    Fired = 0; // no shot is in the air
}

//Start Function - Jake
//Sets initial values of member objects of gamecontroller
//Sets gamestate variables to their default value
//Called each time a new round of the game starts
void GameController::Start(){
    //place tanks
    myTank1.xPos = 50; // Place tank in the middle of the right side
    myTank1.yPos = myTerrain.height-myTank1.height; // Place tank on terrain

    //place tanks
    myTank2.xPos = 250; // Place tank in the middle of the left side
    myTank2.yPos = myTerrain.height-myTank2.height; // Place tank on terrain

    //Aim tanks vagule towards each other and up a bit
    myTank1.Aim(5,-1); // aim to the right
    myTank2.Aim(-5,-1); // aim to the left

    //reset hp
    myTank1.health = 100; // tank 1 starts at full hp
    myTank2.health = 100; // tank 2 starts at full hp

    //Default gamestate values
    ReadyToFire = 0; // No tank has a shot lined up by default
    Turn = 1; // player 1 always goes first
    Aiming = 0; // no tank starts aiming
    Fired = 0; // no shot is in the air
}

//Draw Function - Jake
void GameController::Draw(){
    LCD.Clear(); //Clears the previous frame
    myTerrain.Draw(); //Draw Terrain
    
    //Draw Tanks
    myTank1.Draw(); // Draw player 1's tank
    myTank2.Draw(); // Draw player 2's tank
    
    //Draw correct bullet if fired
    if(Fired){
        //If it's player 1's turn, draw their bullet
        if (Turn == 1)
        {
            //Don't draw if it's offscreen
            if(bullet1.py >= 0){
                bullet1.Draw(); //Draw player 1's bullet
            }
        }
        //If it's player 2's turn, draw their bullet
        if (Turn == 2)
        {
            //Don't draw if it's offscreen
            if(bullet2.py >= 0){
                bullet2.Draw(); //Draw player 1's bullet
            }
        }
    }
    //Draw Buttons
    rightArrow.Draw(); //right
    leftArrow.Draw(); //left
    // Draw Health
    myTank1.drawHealth(1); //player 1's hp
    myTank2.drawHealth(2); //player 2's hp
    //Draw Crown on winners head
    //If player 1 won the last round
    if (Winner == 1){
        myCrown.Draw(myTank1.xPos + myTank1.width/4.0 +1, myTank1.yPos); //Draw crown on player 1's head
    }
    //If player 2 won the last round
    if (Winner == 2){
        myCrown.Draw(myTank2.xPos + myTank2.width/4.0 +1, myTank2.yPos); //Draw crown on player 2's head
    }

    //Draw Firing controller
    //If it's player 1's turn
    if (Turn == 1){
        //Show controls for player 1
        LCD.SetFontColor(myTank1.color); //set color to player 1's color
        LCD.FillCircle(myTank1.xPos + myTank1.width/2 - 1, myTank1.yPos - 5, 3); // Aiming button
        //If aiming but not fired
        if (Aiming == 1 && Fired == 0){
            //Draw aiming line
            LCD.SetFontColor(WHITE);
            //Draw line from tank turret towards the mouse
            LCD.DrawLine(myTank1.offX + myTank1.lx, myTank1.offY + myTank1.ly, myTank1.offX + 0.75*myTank1.xComponent, myTank1.offY + 0.75*myTank1.yComponent); 
        }
    }
    //If it's player 2's turn
    if (Turn == 2){
        //Show controls for player 2
        LCD.SetFontColor(myTank2.color); //set color to player 1's color
        LCD.FillCircle(myTank2.xPos + myTank2.width/2 - 1, myTank2.yPos - 5, 3); // Aiming button
        //If aiming but not fired
        if (Aiming == 1 && Fired == 0){
            //Draw aiming line
            LCD.SetFontColor(WHITE);
            //Draw line from tank turret towards the mouse
            LCD.DrawLine(myTank2.offX + myTank2.lx, myTank2.offY + myTank2.ly, myTank2.offX + 0.75*myTank2.xComponent, myTank2.offY + 0.75*myTank2.yComponent);
        }
    }
    //Update Screen
    LCD.Update();
}

//Take Turn function - Jake
//Ends a players turn
void GameController::takeTurn(){
    //Update turn variable
    Turn = Turn%2 + 1; //Switches between 1 and 2: If turn = 1, 1 % 2 + 1 = 1 + 1 = 2, if turn = 2, 2 % 2 + 1 = 0 + 1 = 1. 
}

//Check end function - Jake
//Checks if the game should end. The game should end if one tank has 0 or less hp
//Returns true if game should end, false otherwise
//Stores who won the games
bool GameController::checkEnd(){
    //If one of the tanks is dead return true, else return false
    //If tank 2 is dead
    if (myTank2.health <= 0){
        gameStats.tank1_wins++; //give tank 2 one win on stats page
        Winner = 1; //set winner to player 1
        return true; // game over
    }
    //If tank 1 is dead
    else if(myTank1.health <= 0){
        gameStats.tank2_wins++; //give tank 2 one win on stats page
        Winner = 2; //set winner to player 1
        return true; // game over
    }
    //If both are still alive
    else{
        return false; // game continues
    }
}


//Touch Function - Jake
//Handles input: movement and aiming
//Takes the mouse x and y position as inputs
void GameController::Touch(int mx, int my){
    Move(mx, my); // Handle movement
    //Handle aiming
    
    //If player 1's turn:
    //Start Aiming if box clicked
    if(Aiming == 0 && Turn == 1 &&  touchInBox(mx, my, myTank1.xPos + myTank1.width/2 - 5 , myTank1.yPos - 11, 10, 10 )){
        //Update Statuses
        Aiming = 1; //start aiming
        ReadyToFire = 1; //a shot is lined up
    }
    //If player 2's turn:
    //Start Aiming if box clicked
    if(Aiming == 0 && Turn == 2 &&  touchInBox(mx, my, myTank2.xPos + myTank2.width/2 -5 , myTank2.yPos - 11, 10, 10)){
        //Update Statuses
        Aiming = 1; //start aiming
        ReadyToFire = 1; //a shot is lined up
    }
    
    //Aim the Tank if aiming initialized
    if(Aiming == 1){
        //Aim correct tank towards mouse
        //Player 1
        if (Turn == 1){
            myTank1.Aim(mx - (myTank1.xPos + myTank1.turretX), my - (myTank1.yPos + myTank1.turretY)); //Aim at the mouse
        }
        //Player 2
        if (Turn == 2){
            myTank2.Aim(mx - (myTank2.xPos + myTank2.turretX), my - (myTank2.yPos + myTank2.turretY)); //Aim at the mouse
        }
    }
}

//Fire Function - Jake
//Initializes a shot from the correct tank
//Called when the mouse is released after a shot is lined up
void GameController::Fire(){
    //update Status
    Aiming = 0; //No longer aiming
    Fired = 1; // A shot is in the air
    ReadyToFire = 0; // a shot is not lined up
    
    //Fire shot from correct tank
    //Player 1's turn
    if (Turn == 1){
        //Update stats
        gameStats.tank1_shots_fired++;
        //Find starting pos using computation variables
        myTank1.calcTurret(); //calculate turret position
        bullet1.Fire(myTank1.offX + myTank1.lx, myTank1.offY + myTank1.ly, 0.025*(myTank1.xComponent), 0.025*(myTank1.yComponent)); //fire the shot based on turret position
    }
    //Player 2's turn
    if (Turn == 2){
        //Update stats
        gameStats.tank2_shots_fired++;
        //Find starting pos using computation variables
        myTank2.calcTurret(); //calculate turret position
        bullet2.Fire(myTank2.offX + myTank2.lx, myTank2.offY + myTank2.ly, 0.025*(myTank2.xComponent), 0.025*(myTank2.yComponent)); //fire the shot based on turret position
    }
}

//CalcShot Function - Jake
//Calls the calcShot function for the fired shot for the correct tank
//Called after a shot is fired and before it lands
void GameController::calcShot(){
    //Calculate shot trajectory
    //If player 1's turn
    if (Turn == 1){
        bullet1.calcShot(); //call player 1's bullet's calculation function
    }
    //If player 2's turn
    if (Turn == 2){
        bullet2.calcShot(); //call player 2's bullet's calculation function
    }
}

//Detect Hit Function - Mayank
// Purpose - detect all possible collisions in the game
// This page was referenced when trying to resolve negative mod error: https://www.geeksforgeeks.org/modulus-on-negative-numbers/
bool GameController::detectHit()
{
    // If the first player shoots
    if (Turn == 1)
    {   
        //Fix negative mod error
        if(bullet1.px<0){
            bullet1.px += 320; // adjust x value so that it's within [0,320]
        }
        //If bullet 1 within tank 2 rectangle
        if (touchInBox((((int)bullet1.px))%320, bullet1.py, myTank2.xPos, myTank2.yPos, myTank2.width, myTank2.height))
        {   
            gameStats.tank1_shots_hit++; //Update stats
            myTank2.health -= DAMAGE; //update health
            Fired = 0; //Update fired variable
            return true; // returns true if hit
        }
        // If bullet 1 within tank 1 rectangle
        else if (touchInBox((((int)bullet1.px))%320, bullet1.py, myTank1.xPos, myTank1.yPos, myTank1.width, myTank1.height))
        {   
            gameStats.tank1_shots_hit++; //Update stats
            myTank1.health -= DAMAGE; //update health
            Fired = 0; //Update fired variable
            return true; // returns true if hit
        }
        // If bullet 1 within tower's rectangle
        else if (touchInBox((((int)bullet1.px))%320, bullet1.py, myTerrain.towerX+10, myTerrain.towerY, 80, 90))
        {   
            Fired = 0; //Update fired variable
            return false; // returns false for miss
        } 
        // if bullet hits terrain or upper screen instead
        else if (bullet1.py >= myTerrain.height) {
            Fired = 0; //Update fired variable
            return false; // returns false for miss
        }
        // if it hits the screen below a certain height
        else if ((bullet1.px < 1 || bullet1.px > 303) && bullet1.py > 160)
        {
            Fired = 0; //Update fired variable
            return false; // returns false for miss
        }
    // If second player shoots
    } else if (Turn == 2) {
        //Fix negative mod error
        if(bullet2.px<0){
            bullet2.px += 320; // adjust x value so that it's within [0,320]
        }
        //If bullet 2 within tank 1 rectangle
        if (touchInBox((((int)bullet2.px))%320, bullet2.py, myTank1.xPos, myTank1.yPos, myTank1.width, myTank1.height))
        {
            gameStats.tank2_shots_hit++; //Update stats
            myTank1.health -= DAMAGE; //update health
            Fired = 0; //Update fired variable
            return true; // returns true if hit
        }
        //If bullet 2 within tank 2 rectangle
        else if (touchInBox((((int)bullet2.px))%320, bullet2.py, myTank2.xPos, myTank2.yPos, myTank2.width, myTank2.height))
        {   
            gameStats.tank2_shots_hit++; //Update stats
            myTank2.health -= DAMAGE; //update health
            Fired = 0; //Update fired variable
            return true; // returns true if hit
        }
        // If bullet 2 within tower's rectangle
        else if (touchInBox((((int)bullet2.px))%320, bullet2.py, myTerrain.towerX, myTerrain.towerY, 80, 90))
        {   
            Fired = 0; //Update fired variable
            return false; // returns false for miss
        }
        // if bullet 2 hits terrain or upper screen
        else if (bullet2.py >= myTerrain.height) {
            Fired = 0; //Update fired variable
            return false; // returns false for miss
        }
        // if it hits the screen below a certain height
        else if (bullet2.px < 335 && bullet2.px > 320 && bullet2.py > 160)
        {
            Fired = 0; //Update fired variable
            return false; // returns false for miss
        }
    }
}

//Move Function - Mayank
// Purpose - Moves tank if the right or left button is pressed
// mx - x position of mouse click
// my - y position of mouse click
void GameController::Move(int mx, int my)
{
    if (Turn == 1) // if it's red tank's turn
    {
        if (touchInBox(mx, my, rightArrow.x, rightArrow.y, rightArrow.w, rightArrow.h)) // if right button is pressed
        {
            if (myTank1.xPos + 1 < 95) // if the movement does not go past the tower
            {
                myTank1.Move(1, 0); // Move the tank 1 pixel to the right
            }
        }

        if (touchInBox(mx, my, leftArrow.x, leftArrow.y, leftArrow.w, leftArrow.h)) // if left button is pressed
        {
            if (myTank1.xPos - 1 > 4) // if the movement does not go past the screen
            {
                myTank1.Move(-1, 0); // Move the tank 1 pixel to the left
            }
        }
    }

    if (Turn == 2) // if it's blue tank's turn
    {
        if (touchInBox(mx, my, rightArrow.x, rightArrow.y, rightArrow.w, rightArrow.h)) // if right button is pressed
        {
            if (myTank2.xPos + 1 < 295) // if the movement does not go past the screen
            {
                myTank2.Move(1, 0); // Move the tank 1 pixel to the right
            }
            
        }

        if (touchInBox(mx, my, leftArrow.x, leftArrow.y, leftArrow.w, leftArrow.h)) // if left button is pressed
        {
            if (myTank2.xPos - 1 > 200) // if the movement does not go past the tower
            {
                myTank2.Move(-1, 0); // Move the tank 1 pixel to the left
            }
        }
    }
}

// Display Stats Method - Mayank
// Purpose - draws statistics page and stats
void GameController::DisplayStats()
{
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Game Statistics:", 0, 0);

    LCD.SetFontColor(RED);
    LCD.WriteAt("Player 1", 0, 30);
    LCD.SetFontColor(BLUE);
    LCD.WriteAt("Player 2", 150, 30);

    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Wins:", 0, 60);
    LCD.SetFontColor(RED);
    LCD.WriteAt(gameStats.tank1_wins, 70, 60);
    LCD.SetFontColor(BLUE);
    LCD.Write("");
    LCD.WriteAt(gameStats.tank2_wins, 100, 60);

    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Shots Fired:", 0, 75);
    LCD.SetFontColor(RED);
    LCD.WriteAt(gameStats.tank1_shots_fired, 150, 75);
    LCD.SetFontColor(BLUE);
    LCD.Write("");
    LCD.WriteAt(gameStats.tank2_shots_fired, 180, 75);

    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Shots Hit:", 0, 90);
    LCD.SetFontColor(RED);
    LCD.WriteAt(gameStats.tank1_shots_hit, 130, 90);
    LCD.SetFontColor(BLUE);
    LCD.Write("");
    LCD.WriteAt(gameStats.tank2_shots_hit, 160, 90);
}

// Winner Screen Method - Mayank
// Purpose - draws winner page depending on winner
// a (parameter) - which tank won
void GameController::displayWinner(int a)
{
    if (a == 1) //If tank 1 wins
    {
        LCD.SetBackgroundColor(LIGHTBLUE);
        LCD.SetFontColor(RED);
        LCD.WriteAt("Red Tank Wins!!!!!", 50, 50);
        LCD.WriteAt("Congratulations!", 50, 70);
    }
    else if (a == 2) //If tank 2 wins
    {
        LCD.SetBackgroundColor(LIGHTBLUE);
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Blue Tank Wins!!!!!", 50, 50);
        LCD.WriteAt("Congratulations!", 50, 70);
    }
    // Resets colors to default
    LCD.SetBackgroundColor(LIGHTBLUE);
    LCD.SetFontColor(BLACK);
}

//Button Methods ------------------------

//Button Constructor - Mayank
//initializes a button
// ix - x position of button. iy - y position of button
// iw - width of button. ih - height of button
// itext - the text of the button
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
// Purpose - draws button on screen
void Button::Draw()
{
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(x, y, w, h);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt(text, x+w/10, y+h/5);
}

//Return Button - Mayank
//returns 1 if the return button is clicked, else 0
//only use this method for the return button
int Button::Return()
{
    int touch_x, touch_y;

    //Set Color
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(x, y, w, h);
    LCD.WriteAt(text, x+w/10, y+h/10);

    // track location of mouse
    if (LCD.Touch(&touch_x, &touch_y))
    {
        // checks if mouse is within return button
        if (touchInBox(touch_x, touch_y, x, y, w, h))
        {
            return 1;
        }
    }

    return 0;
}

//Menu Methods ------------------------

Menu::Menu() //Menu Constuctor
{
    //This has no reason to be here, but I think it's funny that this is line 1000 so I'm leaving it in. - Jake
}

//Menu Draw - Mayank
//Purpose - draws the entire menu
void Menu::Draw()
{
    //Set font color
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Tanks 2.0", 100, 20);

    credits.Draw(); // draw credits button
    playButton.Draw(); // draw play button
    howTo.Draw(); // draw how to button
    stats.Draw(); // draw statistics page
}

//Menu Action - Mayank
//Purpose - checks if any menu button was clicked
//returns int 1-4 depending on which button was cliecked
int Menu::Action()
{
    int x, y;

    //Wait for touchscreen press
    while(!LCD.Touch(&x, &y));

    //Wait for touchscreen release
    while(LCD.Touch(&x, &y));

    // if play button was clicked
    if (touchInBox(x, y, playButton.x, playButton.y, playButton.w, playButton.h))
    {
        return 1;
    }
    // if how to button was clicked
    if (touchInBox(x, y, howTo.x, howTo.y, howTo.w, howTo.h))
    {
        return 2;
    }
    // if credits button was clicked
    if (touchInBox(x, y, credits.x, credits.y, credits.w, credits.h))
    {
        return 3;
    }
    // if stats button was clicked
    if (touchInBox(x, y, stats.x, stats.y, stats.w, stats.h))
    {
        return 4;
    }
    
}


// Generic Functions

// touch in box  - Mayank
// Purpose - check whether a point is inside of a rectangle
// x - x position of point. y - y position of point
// box_x - x position of rectangle. box_y - y position of rectangle.
// box_w - width of rectangle. box_h - height of box
// returns true if point within rectangle, false otherwise
bool touchInBox(int x, int y, int box_x, int box_y, int box_w, int box_h)
{
    //is x within the box's x value endpoints
    if (x > box_x && x < (box_x + box_w))
    {
        //is y within the box's y value endpoints
        if (y > box_y && y < (box_y + box_h))
        {
            return true;
        }
    }

    return false;
}

// Projectile Methods ------------------------------------

// Projectile Constructor - Mayank
// Purpose - initialize projectile values to 0 except for acceleration(constant)
Projectile::Projectile()
{
    px = 0; // x position
    py = 0; // y position
    vx = 0; // y component of velocity vector
    vy = 0; // y component of velocity vector
    ay = +.02; // y component of acceleration vector
}

// Projectile Fire - Mayank
// Purpose - start the projectile firing process
// sets the staring x, y position and x, y velocities
// px0 - starting x position, py0 - starting y position
// vx0 - x velocity, vy0 - y velocity
void Projectile::Fire(float px0, float py0, float vx0, float vy0)
{   
    // Copies values in
    px = px0; // initial x position
    py = py0; // initial y position
    vx = vx0; // x component of initial velocity vector
    vy = vy0; // y component of initial velocity vector
}

// Projectile Calc Shot - Mayank
// Purpose - calculates the position and velocities for next frame
void Projectile::calcShot()
{
    px += vx;// moves position to the right
    py += vy;// moved position based on y velocity
    vy += ay;// increases velocity (falling down because y is positive downwards)
}

// Projectile Draw - Mayank
// Purpose - Draws projectile
void Projectile::Draw()
{
    LCD.SetFontColor(BLACK);
    LCD.FillCircle(px, py, 1); // draw bullet at it's current x,y position
}
