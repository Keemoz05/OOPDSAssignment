#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
using namespace std;

//This Robot class is to be inherited by 4 basic abstract subclasses, namely MovingRobot, ShootingRobot, SeeingRobot and ThinkingRobot.

class Robot{ 
    protected:
    string robot_type;
    string robot_name;
    int robot_locationX;
    int robot_locationY;
    int robot_lives;

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
    }


    void display_stats(){
        cout << "I am a " << robot_type << " named " << robot_name << " at X:" << robot_locationX << " Y:" << robot_locationY << endl;
    }

    virtual void think() = 0;

    
    void TakeTurn(){   //If I haven't looked yet, look. If I see an enemy and have shells, fire. Otherwise, move.
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
        cout << "is moving" << endl;  
    }
};

class ThinkingRobot : virtual public Robot{ // FIXME: Aidil
    // ThinkingRobot is for decision making
    // It depends on functions for look, fire and move
    // There is no specific order to be followed, but it's not random
    void think() {
        cout <<  " is thinking..." << endl;
        //look();  

        // bool enemyNearby = true;  
        // if (enemyNearby && shells > 0) {
        //     fire();
        // } else {
        // move();
        // }
        }
    
};


class SeeingRobot : virtual public Robot { // FIXME: Aidil

    protected:
    void look(){
        cout << " is seeing" << endl;
    }
};

class ShootingRobot : virtual public  Robot{

   //1. input parameters (choose where to shoot)
    //2. Check if hit, check if suicide, check if valid
    //3. Check for ammo count, if ammo count == 0, self delete
    //4. Range is 8

    protected:
    int shells = 10; //default shell count

    public:
        bool fire(int x, int y){ //fire member function
            int selfX = get_locationX();
            int selfY = get_locationY();
            double hit_probability = (rand() % 100) / 100; // random number over 100

            if (x == selfX && y == selfY){
                cout << "Don't shoot yourself you dummy" << endl;
                return false;
            }

            if (shells <= 0){
                cout <<  "ran out of shells, self destructing" << endl;
                return false;
            }

            //if range > 8, out of bounds, return false (figure out how to get target location)

            //if selfX - targetX > 8 || if selfY - targetY > 8, target out of range

            shells--; //shell fired and down one count

            if(hit_probability < 0.7){

                //get enemy robot location
                //enemy.decrease_lives();


            }
            else {
                cout << "Robot fired, and missed the shot." << endl;
            }

        }
         int get_shells(){

            return shells;

        }
};
class GenericRobot : public ShootingRobot,public MovingRobot,public SeeingRobot,public ThinkingRobot{ //derived robot class from robots

    private:
        int upgrades_left;

        //shells = 3;
        //upgrades_left =3; //or the specified amount stated in assignment pdf
                            //figure out how value assignment works in classes


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

// vector <Robot> robotsvector;
Robot* robots[99];

string filetoread = "examplefile.txt";
int battlefieldlength = 0;
int battlefieldwidth = 0;
int steps = 0;
int robotamount = 0;
int r = 0; //if r > robotamount, dont take anymore generic robots

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
        robots[r] = new GenericRobot;
        string word;
        stringstream s(line);
        vector <string> words;
        while(getline(s,word,' ')){
            words.push_back(word); //word array containing keywords, 0 = type , 1 = name, 3= , //What if there's invalid inputs(tell user whats wrong instead of just tspmo?
        }

        //GenericRobot newRobot(words[0],words[1],2,4);
        robots[r]->set_type(words[0]);
        robots[r]->set_name(words[1]);
        if (words[2] != "random"){
            robots[r]->set_locationX(stoi(words[2]));
        }
        else{
            robots[r]->set_locationX(rand() % battlefieldlength); //set it to a random location
        }
        if (words[2] != "random"){
            robots[r]->set_locationY(stoi(words[3]));
        }
        else{
            robots[r]->set_locationY(rand() % battlefieldwidth);
        }
        r++;
        
    }
    else {
        cout << "invalid command ts pmo " << endl;
    }
}

int main(){
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
    
    for(int v = 0;v < robotamount;v++){
        robots[v]->display_stats();
        robots[v]->TakeTurn();
    }
    cout << endl;
    cout << "Battlefield length = " << battlefieldlength << endl;
    cout << "Battlefield width = " << battlefieldwidth << endl;
    cout << "Steps = "<< steps << endl;
    cout << "Amount of robots = "<< robotamount << endl;
    cout << endl;
    DisplayBattlefield();
    robots[0]->TakeTurn();
    // while(steps > 0){
        
    //     for(int i = 0; i < robotsvector.size(); i++){
    //        // robotsvector[i].StartTurn();
    //     }
    //     DisplayBattlefield();

    //     steps -=1;
    // }
}
