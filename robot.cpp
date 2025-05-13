#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
using namespace std;


class Robot{ //This Robot class is to be inherited by 4 basic abstract subclasses, namely MovingRobot, ShootingRobot, SeeingRobot and ThinkingRobot.
    private:
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

    Robot(string type, string name, int locationX, int locationY){
        type = robot_type;
        name = robot_name;
        locationX = robot_locationX;
        locationY = robot_locationY;
    }

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
        cout << "I am a " << robot_type << " named " << robot_name << endl;
    }
};




vector <Robot> robotsvector;
class MovingRobot : public Robot{
    private:

    public:

};

class ThinkingRobot : public Robot{ // FIXME: Aidil

};


class SeeingRobot : public Robot{ // FIXME: Aidil

};

class ShootingRobot : public Robot{ //Chan
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
            double hit_probability = (rand() % 100) / 100; //random number over 100

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



class generic_robot : public MovingRobot , public ShootingRobot, public ThinkingRobot , public SeeingRobot{ //generic_robot inherits from the 4 subclasses

    private:

        int upgrades_left;

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

string filetoread = "examplefile.txt";
int battlefieldlength = 0;
int battlefieldwidth = 0;
int steps = 0;
int robots = 0;

void DisplayBattlefield(){
    string grid[battlefieldlength];

    for (int i =0;i < battlefieldlength;i++){
        // string line(battlefieldwidth,'*');
        // cout << line << endl;
        grid[i] += string (battlefieldwidth,'*');
    }

    for(int r=0;r<robotsvector.size();r++){
        grid[robotsvector[r].get_locationX()-1][robotsvector[r].get_locationY()-1] = 'O'; //MINUS ONE BECAUSE ARRAYGRID START FROM 0
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
        robots = stoi(stringnum);
    }
    else if(line.find("GenericRobot") != string::npos){
        //create robot object with Robot robot1(Kidd,3,6)                          Does it always have to start with genericrobot?
        //separate by space
        Robot newRobot;
        string word;
        stringstream s(line);
        vector <string> words;
        while(getline(s,word,' ')){
            words.push_back(word); //word array containing keywords, 0 = type , 1 = name, 3= , //What if there's invalid inputs(tell user whats wrong instead of just tspmo?
        }
        newRobot.set_type(words[0]);
        newRobot.set_name(words[1]);
        if (words[2] != "random"){
            newRobot.set_locationX(stoi(words[2]));
        }
        else{
            newRobot.set_locationX(rand() % battlefieldlength); //set it to a random location
        }
        if (words[2] != "random"){
            newRobot.set_locationY(stoi(words[3]));
        }
        else{
            newRobot.set_locationY(rand() % battlefieldwidth);
        }


        // for (int r=0;r < words.size();r++){   //r[0] == robot_type(GenericRobot),r[1] == robot_name(Kidd) //probably dont need this
        //     //create robot type generic
        //     //cout << words[r] << endl;
        //     //newRobot.set_type(words[r]);


        // }
        robotsvector.push_back(newRobot);
    }
    else {
        cout << "invalid command ts pmo " << endl;
    }
}

int main(){
    // vector <Robot> robotsvector;  //if robotsvector is declared here, its value is empty
    ifstream MyReadFile(filetoread);

    // Variable to store each line from the file
    string line;

    // Read each line from the file and print it
    while (getline(MyReadFile, line)) {
        // Process each line as needed
        AnalyseFile(line);
        cout << line << endl;

    }

    for(int v = 0;v < robotsvector.size();v++){

        robotsvector[v].display_stats();
    }
    cout << endl;
    cout << "Battlefield length = " << battlefieldlength << endl;
    cout << "Battlefield width = " << battlefieldwidth << endl;
    cout << "Steps = "<< steps << endl;
    cout << "Amount of robots = "<< robots << endl;
    cout << endl;
    //DisplayBattlefield();

    //robot1think and do
    //robot2think and do
    //displaybattlefield
    //loop until turn = 0

    while(steps > 0){

        for(int i = 0; i < robotsvector.size(); i++){
           // robotsvector[i].uuiiaa();
        }
        DisplayBattlefield();

        steps -=1;
    }
}
