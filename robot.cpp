#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

// === GLOBAL VARIABLES ===
vector<string> grid;
string filetoread = "examplefile.txt";
int battlefieldlength = 0;
int battlefieldwidth = 0;
int steps = 0;
int robotamount = 0;
int r = 0; //if r > robotamount, dont take anymore generic robots

// === CLASS DECLARATIONS ===
//This Robot class is to be inherited by 4 basic abstract subclasses, namely MovingRobot, ShootingRobot, SeeingRobot and ThinkingRobot.
class Robot{
    protected:
    string robot_type;
    string robot_name;
    int robot_locationX;
    int robot_locationY;
    int robot_lives;
    bool foundEnemy = false; //appropriate name?
    // int targetX;
    // int targetY;

    public:
    Robot(){

            robot_type = "Generic Robot";
            robot_name ="BOB";

            //robot_locationX = rand() * get battlefield width and length
            //robot_locationY = rand() *
            robot_lives = 3; //if lives run out, robot will no longer respawn


    }; //define default constructor parameters

    // Robot(string type, string name, int locationX, int locationY){
    //     type = robot_type;
    //     name = robot_name;
    //     locationX = robot_locationX;
    //     locationY = robot_locationY;
    // }


    public:
    void set_locationX(int x){
        robot_locationX = x-1; //-1 because grid vector starts from 0
    }
    void set_locationY(int y){
        robot_locationY = y-1;
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

    virtual void think() = 0; //abstract functions declaration
    virtual void move() = 0;
    virtual void look() = 0;
    virtual void shoot(int x , int y) = 0;

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
Robot* robots[99]; //Note: delete objects afterwards to reduce memory leaks
class MovingRobot : virtual public Robot{

    private:

    protected:
    void move(){
        int randommove = rand() % 9 + 1;
        //int randommove = 8;
        switch (randommove){
            case 1:
            //if(robotlo)
            if(robot_locationX + 1 != battlefieldwidth){  //only checks if value is on boundary, not beyond
            grid[robot_locationY][robot_locationX] = '-'; //set old location to become empty
            robot_locationX += 1; //right
            cout << robot_name << " moved to the right!" << endl;
            }
            else{cout << robot_name << " moved to the right and hit a wall!" << endl;}
            break;
            case 2:
            if(robot_locationX - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX -= 1; //left
            cout << robot_name << " moved to the left!" << endl;
            }
            else{cout << robot_name << " moved to the left and hit a wall" << endl;}
            break;
            case 3:
            if(robot_locationX + 1 != battlefieldwidth && robot_locationY - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX += 1; //top right
            robot_locationY -= 1;
            cout << robot_name << " moved to the top right!" << endl;
            }
            else{cout << robot_name << "moved to the top right and hit a wall!" << endl;}
            break;
            case 4:
            if(robot_locationX - 1 != -1 && robot_locationY - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX -= 1; //top left
            robot_locationY -= 1;
            cout << robot_name << " moved to the top left!" << endl;
            }
            else{cout << robot_name << " moved to the top left and hit a wall!" << endl;}
            break;
            case 5:
            if(robot_locationY - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationY -= 1;   //top
            cout << robot_name << " moved up!" << endl;
            }
            else {cout << robot_name << " moved to the top and bonked its head on a wall!" << endl;}
            break;
            case 6:
            if(robot_locationY + 1 != battlefieldlength){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationY += 1; //bottom
            cout << robot_name << " moved down!" << endl;
            }
            else{cout << robot_name << " moved to the bottom and hit a wall!" << endl;}
            break;
            case 7:
            if(robot_locationX - 1 != -1 && robot_locationY + 1 != battlefieldlength){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationY += 1; //bottom left
            robot_locationX -= 1;
            cout << robot_name << " moved down left!" << endl;
            }
            else{cout << robot_name << " moved to the bottom left and hit a wall!" << endl;}
            break;
            case 8:
            if(robot_locationX + 1 != battlefieldwidth && robot_locationY + 1 != battlefieldlength){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX += 1;
            robot_locationY += 1; //bottom right
            cout << robot_name << " moved down right!" << endl;
            } else{cout << robot_name << " moved to the bottom right and hit a wall!" << endl;}
            break;
            case 9:
            cout << robot_name << " chose to not move!" << endl;
            break;



    }
}
};

class ShootingRobot : virtual public Robot{

   //1. input parameters (choose where to shoot)
    //2. Check if hit, check if suicide, check if valid
    //3. Check for ammo count, if ammo count == 0, self delete
    //4. Range is 8

    protected:
        int shells= 10;

    public:

    int get_shells(){
        return shells;
    };

    void use_ammo(){
        --shells;
    };

    void shoot(int x,int y){

        if(this->get_shells() <= 0){
            cout << this->get_name() <<  "has ran out of shells and will self destruct" << endl;
            this->decrease_lives();
            return;
        }

        if(this->get_locationX() == x && this->get_locationY() == y){
                    cout << "Don't shoot yourself you dummy" << endl;
                return;
            }

            for(int i =0;i < robotamount;i++){


            if(robots[i]->get_locationX() == x && robots[i]->get_locationY() == y){
                cout << this->robot_name <<" attempts shooting at " << robots[i]->get_name() << "!" << endl;

                //int hit_probability = rand() % 100 + 1; //choose from 0 to 100
                int hit_probability = 69;
                if(hit_probability < 70){
                    cout << this->robot_name << " hit and destroyed " << robots[i]->get_name() << "!" << endl;  //then upgrades
                    robots[i]->decrease_lives(); 
                    //cout << robots[i]->get_locationY() << " " << robots[i]->get_locationX() << endl;
                    grid[robots[i]->get_locationY()][robots[i]->get_locationX()] = '-'; //remove destroyed robot from map, correct the syntax if relevant
                    robots[i]->set_locationX(0);
                    robots[i]->set_locationY(0); // set it to 0 because setlocation -= 1;

                   
                }

                else{
                    cout << this->robot_name << " missed! "  << endl;
                    this->use_ammo();
                }
                return;
            }

        }
    }

// bool fire(Robot* target){ //fire member function
//            int selfX = this->get_locationX();
//            int selfY = this->get_locationY();
//            int targetX = target->get_locationX();
//            int targetY= target->get_locationY();
//            double hit_probability = (rand() % 100) / 100; //random number over 100
//
//            if (selfX == targetX && selfY == targetY){
//                cout << "Don't shoot yourself you dummy" << endl; //these will be logged in the future
//                return false;
//            }
//
//            if (shells <= 0){
//                cout <<  "ran out of shells, self destructing" << endl; //these will be logged in the future
//                while( this->get_lives() > 0){
//                    this->decrease_lives();
//                }
//                return false;
//            }
//
//
//            //if range > 8, out of bounds, return false (figure out how to get target location)
//            if(abs(selfX - targetX) > 8 || abs(selfY - targetY) > 8){
//               cout <<"Target is out of range" << endl;       //these will be logged in the future
//               return false;
//               }
//            //if selfX - targetX > 8 || if selfY - targetY > 8, target out of range
//
//            shells--; //shell fired and down one count
//
//            if(hit_probability < 0.7){
//
//                //get enemy robot location
//                target->decrease_lives();
//                return true;
//
//            }
//            else {
//
//                return false;
//            }
//        // FIXME: [clang] (-Wreturn-type) Non-void function does not return a value in all control paths
//        }
//         int get_shells(){
//
//            return shells;
//
//        }
};

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
        int dx[] = {-1,0,1,1,1,0,-1,-1};  //start with top left, top, top right, right, bottom right, bottom , bottom left,left
        int dy[] = {-1,-1,-1,0,1,1,1,0}; //When dx=-1 and dy = -1, it represents top left, these are parallel arrays
        for(int i = 0;i < 8;i++){
            int a = this->robot_locationX + dx[i];
            int b = this->robot_locationY + dy[i];
            //cout << grid[b][a] << " "  << a << " " << b << endl;

            if(a != battlefieldwidth && b != battlefieldlength && a != -1 && b != -1){    //check if a and b is out of bounds,if true, dont look there
                                                                                            //only checks if value is on boundary, not beyond
            if(grid[b][a] == 'R'){
                switch(i){
                    case 0:
                    cout << robot_name << " found a target on the top left!" << endl;  //target = a and b, seen
                    foundEnemy = true;
                    shoot(a,b);
                    break;
                    case 1:
                    cout << robot_name << " found a target on top!" << endl;
                    foundEnemy = true;
                    shoot(a,b);
                    break;
                    case 2:
                    cout << robot_name << " found a target on top right!" << endl;
                    foundEnemy = true;
                    shoot(a,b);
                    // targetX = a;
                    // targetY = b;
                    break;
                    case 3:
                    foundEnemy = true;
                    cout << robot_name << " found a target on the right!" << endl;
                    shoot(a,b);
                    break;
                    case 4:
                    
                    foundEnemy = true;
                    cout << robot_name << " found a target on below right!" << endl;
                    shoot(a,b);
                    break;
                    case 5:
                    
                    foundEnemy = true;
                    cout << robot_name << " found a target below!" << endl;
                    shoot(a,b);
                    break;
                    case 6:
                    foundEnemy = true;
                    cout << robot_name << " found a target on below left!" << endl;
                    shoot(a,b);
                    break;
                    case 7:
                    foundEnemy = true;
                    cout << robot_name << " found a target on the left!" << endl;
                    shoot(a,b);
                    break;
                }
            }
        }
        }
        //if robot_x + 1 == 'R', what value to save?

    }
};

class ThinkingRobot : virtual public Robot{ // Aidil
    // ThinkingRobot is for decision making
    // Should the robot move? shoot? look?

    void think() {
        cout << robot_name << " is thinking..." << endl;
        // Flowchart:
        // Look. If there's enemy, check number of shells.
        // If no shells, move. If there is shells, fire.
        // Ideally, output text of process
        //this->move();
        this->look();
        if(this->foundEnemy == false){
            cout << this->robot_name << " looked around but didnt found anyone!" << endl;
            move();
            }

        // if(foundEnemy == true && shells > 0){
        //     // FIXME: Target Assignment Code
        //
        //     cout << robot_name << " will shoot at " << endl; //FIXME: Code for getting target name
        //     bool hit = this->fire(Robot* target); // yoinked from GenericRobot comment but still unsure how to implement target
        //     if(hit) {
        //         cout << robot_name << " hit the target >:)" << endl;
        //     }
        //     else {
        //         cout << robot_name << " missed the target :(" << endl;
        //     //fire(shootX,shootY);
        //     }
        // else{
        //     this->move();
        // }
        // }

    }
};

class GenericRobot : public ShootingRobot,public MovingRobot,public SeeingRobot,public ThinkingRobot{ //derived robot class from robots

    private:
        int upgrades_left;

        //shells = 3;
        //upgrades_left =3; //or the specified amount stated in assignment pdf
                            //figure out how value assignment works in classes


    public:



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
class RobotUpgrade : public GenericRobot{
//Robot only upgrades when it destroys another
//Robot can only choose one upgrade each from (moving,shooting,looking)
//Robot has an upgrade limit of 3


    protected:

        GenericRobot* pRobot;
    public:

        RobotUpgrade(GenericRobot* Robot){}
        void RandUpgrade();
//        Keep track of which categories have been picked
//        Randomly pick a category not yet picked
//        Randomly pick an upgrade from that category






};

class HideBot : public RobotUpgrade{



};
class JumpBot : public RobotUpgrade{};
class LongShotBot : public RobotUpgrade{};
class SemiAutoBot : public RobotUpgrade{};
class ThirtyShotBot : public RobotUpgrade{};
class ScoutBot : public RobotUpgrade{};
class TrackBot : public RobotUpgrade{};




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
    for (int i =0;i < battlefieldlength;i++){
        // string line(battlefieldwidth,'-');
        //grid[] += string (battlefieldwidth,'-');
        grid.push_back(string (battlefieldwidth,'-'));
    }
    DisplayBattlefield();
    robots[0]->TakeTurn();
    DisplayBattlefield();
    // while(steps > 0){

    //     for(int i = 0; i < r; i++){
    //         robots[i]->taketurn();                      //comment these out to make it run only for one robot
    //     }
    //     displaybattlefield();

    //     steps -=1;
    // }
}

// === FUNCTION DECLARATIONS ===
void DisplayBattlefield(){



    for(int b=0;b < robotamount;b++){
        //grid[robotsvector[b].get_locationX()-1][robotsvector[b].get_locationY()-1] = 'O'; //MINUS ONE BECAUSE ARRAYGRID START FROM 0
        //cout << robots[b]->get_locationX() << "," <<  robots[b]->get_locationY() << endl;
        int y = robots[b]->get_locationY();
        int x = robots[b]->get_locationX();
        //cout << x << " " << y << endl;
        if(y != -1 || x != -1){
            grid[y][x] = 'R'; //grid[Y][X]
        }
        
    }


    for (string row : grid) {
        cout << row << endl;
    }


}

void CreateRobotObjects(string type,string name,string X,string Y){ //auto is not used for function arguments
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
