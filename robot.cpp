#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

// === CLASS DECLARATIONS ===
//This Robot class is to be inherited by 4 basic abstract subclasses, namely MovingRobot, ShootingRobot, SeeingRobot and ThinkingRobot.
class Robot{
    protected: 
    int shells = 10;
    string robot_type;
    string robot_name;
    int robot_locationX;
    int robot_locationY;
    int robot_lives;
    bool foundEnemy; //appropriate name?

    public:
    Robot(){

            robot_type = "Generic Robot";
            robot_name ="BOB";

            //robot_locationX = rand() * get battlefield width and length
            //robot_locationY = rand() *
            robot_lives = 3;


    }; //define default constructor parameters

    // Robot(string type, string name, int locationX, int locationY){
    //     type = robot_type;
    //     name = robot_name;
    //     locationX = robot_locationX;
    //     locationY = robot_locationY;
    // }


    public:
    void set_locationX(int x){
        robot_locationX = x;
    }
    void set_locationY(int y){
        robot_locationY = y;
    }
    void set_name(string name){
        robot_name = name;
    }
    void set_type(string type){
        robot_type = type;
    }

    string get_type() //accessors
    {
        return robot_type;

    }


    string get_name() //accessors
    {
        return robot_name;

    }

    int get_locationX()
    {

        return robot_locationX;
    };

    int get_locationY()
    {

        return robot_locationY;
    }

    int get_lives()
    {
        return robot_lives;
    }
     void decrease_lives(){

        robot_lives--;
    }

    bool isAlive(){
        if(robot_lives <= 0){
            cout << "Robot is eliminated" << endl; //run this before every turn
            return true;
        }
    // FIXME:[clang] (-Wreturn-type) Non-void function does not return a value in all control paths
    }


    void display_stats(){
        cout << "I am a " << robot_type << " named " << robot_name << " at X:" << robot_locationX << " Y:" << robot_locationY << endl;
    }

    virtual void think() = 0;
    virtual void move() = 0;
    virtual void look() = 0;
   // virtual void fire() = 0;

    void TakeTurn(){   
        think();       
    }
    //     // Example: move randomly
    //     int dx = (rand() % 3) - 1; // -1, 0, 1
    //     int dy = (rand() % 3) - 1;
    //     int newX = max(1, min(get_locationX() + dx, battlefieldlength));
    //     int newY = max(1, min(get_locationY() + dy, battlefieldwidth));
    //     move(newX, newY);
    // }
};

class MovingRobot : virtual public Robot{

    private:

    protected:
    void move(){

        int randommove = rand() % 8 + 1;
        switch (randommove){
            case 1:
            robot_locationX += 1; //right
            cout << robot_name << " moved to the right!" << endl;
            break;
            case 2:
            robot_locationX -= 1; //left
            cout << robot_name << " moved to the left!" << endl;
            break;
            case 3:
            robot_locationX += 1; //top right
            robot_locationY -= 1;
            cout << robot_name << " moved to the top right!" << endl;
            break;
            case 4:
            robot_locationX -= 1; //top left
            robot_locationY -= 1;
            cout << robot_name << " moved to the top left!" << endl;
            break;
            case 5:
            robot_locationY -= 1;   //top
            cout << robot_name << " moved up!" << endl;
            break;
            case 6:
            robot_locationY += 1; //bottom
            cout << robot_name << " moved down!" << endl;
            break;
            case 7:    
            robot_locationY += 1; //bottom left
            robot_locationX -= 1; 
            cout << robot_name << " moved down left!" << endl;
            break;
            case 8:
            robot_locationX += 1;
            robot_locationY += 1; //bottom right
            cout << robot_name << " moved down right!" << endl;




    }
}
};

class ShootingRobot : virtual public Robot{

   //1. input parameters (choose where to shoot)
    //2. Check if hit, check if suicide, check if valid
    //3. Check for ammo count, if ammo count == 0, self delete
    //4. Range is 8


    
    public:

        bool fire(Robot* target){ //fire member function
            int selfX = this->get_locationX();
            int selfY = this->get_locationY();
            int targetX = target->get_locationX();
            int targetY= target->get_locationY();
            double hit_probability = (rand() % 100) / 100; //random number over 100

            if (selfX == targetX && selfY == targetY){
                cout << "Don't shoot yourself you dummy" << endl; //these will be logged in the future
                return false;
            }

            if (shells <= 0){
                cout <<  "ran out of shells, self destructing" << endl; //these will be logged in the future
                while( this->get_lives() > 0){
                    this->decrease_lives();
                }
                return false;
            }


            //if range > 8, out of bounds, return false (figure out how to get target location)
            if(abs(selfX - targetX) > 8 || abs(selfY - targetY) > 8){
               cout <<"Target is out of range" << endl;       //these will be logged in the future
               return false;
               }
            //if selfX - targetX > 8 || if selfY - targetY > 8, target out of range

            shells--; //shell fired and down one count

            if(hit_probability < 0.7){

                //get enemy robot location
                target->decrease_lives();
                return true;

            }
            else {

                return false;
            }
        // FIXME: [clang] (-Wreturn-type) Non-void function does not return a value in all control paths 
        }
         int get_shells(){

            return shells;

        }
};
Robot* robots[99];
class SeeingRobot : virtual public Robot { // Aidil
    // The look(x,y) action will check a 3x3 grid to a robot,
    // centred on (robotsPositionX + x, robotsPositionY + y).
    // Example: look(0,0) will:
    // 1. Provide a robot with its immediate neighbourhood (all visitable places in the next turn)
    // 2. Reveal whether the location is within the battlefield
    // 3. Reveal whether a location is has an enemy robot
    // Note: A location can only have one robot at a time
    protected:
    void look(){
        cout << robot_name <<" is looking around..." << endl;
    }
};

class ThinkingRobot : virtual public Robot{ // Aidil
    // ThinkingRobot is for decision making
    // Should the robot move? shoot? look?

    void think() { //start with look. If I see an enemy,fire. Otherwise, move.
        cout << robot_name << " is thinking..." << endl;

        this->look();
        if(foundEnemy == true){
            //fire();
        }
        else{
            this->move();
        }
        
        
            
        // Still not sure where I should put the logic for the thinking (within think or another nested function)
        // For now, I'll leave these declarations
        // bool enemyNearby = true;
            // if (enemyNearby && shells > 0) {
            //     fire();
            // } else {
            // move();
      

    }
};

class GenericRobot : public ShootingRobot,public MovingRobot,public SeeingRobot,public ThinkingRobot{ //derived robot class from robots

    private:
        int upgrades_left;

        //shells = 3;
        //upgrades_left =3; //or the specified amount stated in assignment pdf
                            //figure out how value assignment works in classes

    public:
        //bool hit = robot->fire(5, 7);


        int get_upgrades_left(){
            return upgrades_left;

        }
        void take_action();             //needs definition
        void upgrade();                 //needs definition
        void upgrade_cap(){


            if(upgrades_left > 0){

                upgrades_left--;
            }
            else{

                cout << "Robot have exceeded the upgrade cap" << endl; //temporary cout, will need to link which robot has cap
            }
        }


};

// === GLOBAL VARIABLES ===
 // vector <robot> robotsvector;
string filetoread = "examplefile.txt";
int battlefieldlength = 0;
int battlefieldwidth = 0;
int steps = 0;
int robotamount = 0;
int r = 0; //if r > robotamount, dont take anymore generic robots

// === FUNCTION PROTOTYPES ===
void DisplayBattlefield();
void AnalyseFile(string line);

// === MAIN PROGRAM ===
int main(){
    srand (time(0)); //to make random number work, else same number will keluar everytime
    // vector <Robot> robotsvector;  //if robotsvector is declared here, its value is empty
    ifstream MyReadFile(filetoread);
    // int max_robots = robotamount;
    // Robot* robots[max_robots];
    // Variable to store each line from the file
    string line;

    // Read each line from the file and print it
    while (getline(MyReadFile, line)) {
        // Process each line as needed
        AnalyseFile(line);
        cout << line << endl;

    }

    // for(int v = 0;v < robotamount;v++){
    //     robots[v]->display_stats();
    //     robots[v]->TakeTurn();
    // }
    cout << endl;
    cout << "Battlefield length = " << battlefieldlength << endl;
    cout << "Battlefield width = " << battlefieldwidth << endl;
    cout << "Steps = "<< steps << endl;
    cout << "Amount of robots = "<< robotamount << endl;
    cout << endl;
    DisplayBattlefield();
    robots[0]->TakeTurn();
    DisplayBattlefield();
    // while(steps > 0){

    //     for(int i = 0; i < robotsvector.size(); i++){
    //        // robotsvector[i].StartTurn();
    //     }
    //     DisplayBattlefield();

    //     steps -=1;
    // }
}

// === FUNCTION DECLARATIONS ===
void DisplayBattlefield(){
    string grid[battlefieldlength];

    for (int i =0;i < battlefieldlength;i++){
        // string line(battlefieldwidth,'*');
        grid[i] += string (battlefieldwidth,'*');
    }

    for(int b=0;b < robotamount;b++){
        //grid[robotsvector[b].get_locationX()-1][robotsvector[b].get_locationY()-1] = 'O'; //MINUS ONE BECAUSE ARRAYGRID START FROM 0
        grid[robots[b]->get_locationY()-1][robots[b]->get_locationX()-1] = 'R';

    }


    for (string row : grid) {
        cout << row << endl;
    }


}

void CreateRobotObjects(string type,string name,auto X,auto Y){
        robots[r] = new GenericRobot;
        robots[r]->set_type(type);
        robots[r]->set_name(name);
        if (X != "random"){
            robots[r]->set_locationX(stoi(X));
        }
        else{
            robots[r]->set_locationX(rand() % battlefieldlength); //set it to a random location
        }
        if (Y != "random"){
            robots[r]->set_locationY(stoi(Y));                          
        }
        else{
            robots[r]->set_locationY(rand() % battlefieldwidth);
        }
        r++;

    }
  


void AnalyseFile(string line){

    if(line.find("M by N") != string::npos){
        int pos = line.find(":");
        int pos2 = line.rfind(" ");
        string strlength = line.substr(pos+1,line.length());
        string strwidth = line.substr(pos2,line.length());
        battlefieldlength = stoi(strlength);
        battlefieldwidth = stoi(strwidth);
    }
    else if(line.find("steps")!= string::npos){
        int pos = line.find(":");
        string stringnum = line.substr(pos+1,line.length());
        steps = stoi(stringnum);
    }
    else if(line.find("robots")!= string::npos){
        int pos = line.find(":");
        string stringnum = line.substr(pos+1,line.length());
        robotamount = stoi(stringnum);
    }
    else if(line.find("GenericRobot") != string::npos){ //and robots < 5
        //create robot object with Robot robot1(Kidd,3,6)                          Does it always have to start with genericrobot?
        //separate by space
        string word;     
        stringstream s(line);
        vector <string> words;
        while(getline(s,word,' ')){
            words.push_back(word); //word array containing keywords, 0 = type , 1 = name, 3= , //What if there's invalid inputs(tell user whats wrong instead of just tspmo?
        }
        //GenericRobot newRobot(words[0],words[1],2,4);
        CreateRobotObjects(words[0],words[1],words[2],words[3]);
   
}
     else{
        cout << "invalid command ts pmo " << endl;
    }
}
