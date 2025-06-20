//**********|**********|**********|
//Program: robot.cpp
//Course: CCP6124-OOPDS
//Trimester: 2510
//Name: Syed Haqeem Bin Syed Isa (242UC244DP) - Chan Chuan Meng (242UC244LR) -  Aidil Amdan Bin Zulaime - (243UC245MZ)
//Lecture Section: TC3L
//Tutorial Section: TT10L
//Email: syed.haqeem.syed@student.mmu.edu.my - chan.chuan.meng@student.mmu.edu.my - aidil.amdan.zulaime@student.mmu.edu.my
//Phone: 0174848002- 0125293192 - 0162419456
//**********|**********|**********/


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <ctime>
using namespace std;

// === GLOBAL VARIABLES ===
vector<string> grid;          
class Robot; //forward declaration
vector<Robot*> respawnlist;
string filetoread = "examplefile.txt";
int battlefieldlength = 0;
int battlefieldwidth = 0;
int steps = 0;
int turncounter = 1;
int robotamount = 0;
int r = 0; //if r > robotamount, dont take anymore generic robots
Robot* robots[99];
ofstream outFile("GameLog3.txt", ios::out);

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
    vector<string> upgrades_done;



    bool CanJump = false;
    int JumpCharges;

    bool CanLongShot = false;

    bool SemiAutoShot = false;
    int SemiAutoShots = 3;

    bool canScout = false;
    int ScoutCharges = 3;

    bool canTrack = false;
    int TrackCharges = 3;

    int HideCharges;
    public:
    bool IsHiding = false; //public because being used by other robot to see if the enmy is hidden
        // Check if a robot has already used an area upgrade
    bool hasUpgradeArea(string area) {
        for (string a : upgrades_done) {
            if (a == area) return true;
        }
        return false;
    }
    int getUpgradeCount() {
        return upgrades_done.size();
    }
        // Add a newly selected area to the list
    void addUpgradeArea(string area) {
        upgrades_done.push_back(area);
    }
    Robot(){

            robot_type = "Generic Robot";
            robot_name ="BOB";

            //robot_locationX = rand() * get battlefield width and length
            robot_lives = 3; //if lives run out, robot will no longer respawn


    }; //define default constructor parameters

    // Robot(string type, string name, int locationX, int locationY){
    //     type = robot_type;
    //     name = robot_name;
    //     locationX = robot_locationX;
    //     locationY = robot_locationY;
    // }


    public:
    void SetcanJump(){
        CanJump = true;
        JumpCharges = 3;
    }
    void ReduceJumpCharge(){
        JumpCharges -=1;
        if (JumpCharges == 0){
            cout << robot_name << " ran out of jump charges!" << endl;
            outFile << robot_name << " ran out of jump charges!" << endl;
            CanJump = false;
        }
    }
    void SetcanHide(){

        IsHiding = true;
        HideCharges = 3;
    }
    void ReduceHideCharge(){
        HideCharges -=1;
        if (HideCharges == 0){
            cout << robot_name << " ran out of hide charges!" << endl;
            outFile << robot_name << " ran out of hide charges!" << endl;
            IsHiding = false;
        }
    }
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
        respawnlist.push_back(this); //puts robot into respawn queue list

    }

    bool isAlive(){
        if(robot_lives <= 0){

            return false;
        }
        else{
            return true;
        }

    }


    void display_stats(){
        cout << "I am a " << robot_type << " named " << robot_name << " at X:" << robot_locationY << " Y:" << robot_locationX << endl;
    }

    virtual void think() = 0; //abstract functions declaration
    virtual void move() = 0;
    virtual void look() = 0;
    virtual void shoot(int x , int y) = 0;
    virtual void applyUpgrade() = 0;
    virtual void scout() = 0;
    virtual void track() = 0;
    void TakeTurn(){
        if(this->robot_locationX != -1 && this->robot_locationY != -1 && isAlive()){
        think();
        }
    }

};

 //Note: delete objects afterwards to reduce memory leaks


class MovingRobot : virtual public Robot{

    private:

    protected:
    void move(){
        if(CanJump == true){
            grid[robot_locationY][robot_locationX] = '-';
           // this->set_locationX(rand() % battlefieldlength);
            robot_locationX = (rand() % battlefieldlength) ;
            robot_locationY = (rand() % battlefieldwidth) ;
            //this->set_locationY(rand() % battlefieldwidth);
            cout << robot_name << " has jumped to coordinates X: "<< robot_locationX+1 << " Y: " << robot_locationY+1 << "!" << endl; //+1 becuase grid is dumb
            this->ReduceJumpCharge();
        }
        else{
        int randommove = rand() % 9 + 1;
        //int randommove = 8
        switch (randommove){
            case 1:
            //if(robotlo)
            if(robot_locationX + 1 != battlefieldwidth){  //only checks if value is on boundary, not beyond
            grid[robot_locationY][robot_locationX] = '-'; //set old location to become empty
            robot_locationX += 1; //right
            cout << robot_name << " moved to the right!" << endl;
            outFile << robot_name << " moved to the right!" << endl;
            }
            else{cout << robot_name << " moved to the right and hit a wall!" << endl; outFile << robot_name << " moved to the right and hit a wall!" << endl;}
            break;
            case 2:
            if(robot_locationX - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX -= 1; //left
            cout << robot_name << " moved to the left!" << endl;
            outFile << robot_name << " moved to the left!" << endl;
            }
            else{cout << robot_name << " moved to the left and hit a wall" << endl; outFile << robot_name << " moved to the left and hit a wall" << endl;}
            break;
            case 3:
            if(robot_locationX + 1 != battlefieldwidth && robot_locationY - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX += 1; //top right
            robot_locationY -= 1;
            cout << robot_name << " moved to the top right!" << endl;
            outFile << robot_name << " moved to the top right!" << endl ;
            }
            else{cout << robot_name << "moved to the top right and hit a wall!" << endl; outFile << robot_name << "moved to the top right and hit a wall!" << endl;}
            break;
            case 4:
            if(robot_locationX - 1 != -1 && robot_locationY - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX -= 1; //top left
            robot_locationY -= 1;
            cout << robot_name << " moved to the top left!" << endl;
            outFile << robot_name << " moved to the top left!" << endl;
            }
            else{cout << robot_name << " moved to the top left and hit a wall!" << endl; outFile << robot_name << " moved to the top left and hit a wall!" << endl;}
            break;
            case 5:
            if(robot_locationY - 1 != -1){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationY -= 1;   //top
            cout << robot_name << " moved up!" << endl;
            outFile << robot_name << " moved up!" << endl;
            }
            else {cout << robot_name << " moved to the top and bonked its head on a wall!" << endl; outFile << robot_name << " moved to the top and bonked its head on a wall!" << endl;}
            break;
            case 6:
            if(robot_locationY + 1 != battlefieldlength){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationY += 1; //bottom
            cout << robot_name << " moved down!" << endl;
            outFile << robot_name << " moved down!" << endl;
            }
            else{cout << robot_name << " moved to the bottom and hit a wall!" << endl; outFile << robot_name << " moved to the bottom and hit a wall!" << endl;}
            break;
            case 7:
            if(robot_locationX - 1 != -1 && robot_locationY + 1 != battlefieldlength){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationY += 1; //bottom left
            robot_locationX -= 1;
            cout << robot_name << " moved down left!" << endl;
            outFile << robot_name << " moved down left!" << endl;
            }
            else{cout << robot_name << " moved to the bottom left and hit a wall!" << endl; outFile << robot_name << " moved to the bottom left and hit a wall!" << endl;}
            break;
            case 8:
            if(robot_locationX + 1 != battlefieldwidth && robot_locationY + 1 != battlefieldlength){
            grid[robot_locationY][robot_locationX] = '-';
            robot_locationX += 1;
            robot_locationY += 1; //bottom right
            cout << robot_name << " moved down right!" << endl;
            outFile << robot_name << " moved down right!" << endl;
            } else{cout << robot_name << " moved to the bottom right and hit a wall!" << endl; outFile << robot_name << " moved to the bottom right and hit a wall!" << endl;}
            break;
            case 9:
            cout << robot_name << " chose to not move!" << endl;
            outFile << robot_name << " chose to not move!" << endl;
            break;


        }
    }
}
};

class ShootingRobot : virtual public Robot{

   //1. input parameters (choose where to shoot)
    //2. Check if hit, check if suicide, check if valid
    //3. Check for ammo count, if ammo count == 0, self delete
    //4. Range is 8
    bool ShotSuccess = false;

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
        SemiAutoShots = 3;
        ShotSuccess = false;

        if(this->get_locationX() == x && this->get_locationY() == y){
                cout << "Don't shoot yourself you dummy!" << endl;
                outFile << "Don't shoot yourself you dummy!" << endl;
                return;
            }
            do{
            for(int i =0;i < r;i++){
            if(robots[i]->get_locationX() == x && robots[i]->get_locationY() == y){
                cout << this->robot_name <<" attempts shooting at " << robots[i]->get_name() << "!" << endl;
                outFile << this->robot_name <<" attempts shooting at " << robots[i]->get_name() << "!" << endl;
                this->use_ammo();
                cout << this->robot_name << " has " << this->get_shells() << " shells left." << endl;
                outFile << this->robot_name << " has " << this->get_shells() << " shells left." << endl;
                int hit_probability = rand() % 100 + 1; //choose from 0 to 100
                //int hit_probability = 69;
                if(hit_probability < 70){
                    if(robots[i]->IsHiding == false){
                    cout << this->robot_name << " hit and destroyed " << robots[i]->get_name() << "!" << endl;  //then upgrades
                    outFile << this->robot_name << " hit and destroyed " << robots[i]->get_name() << "!" << endl;
                    robots[i]->decrease_lives();
                    cout << robots[i]->get_name() << " now has " << robots[i]->get_lives() << " lives left." << endl;
                    outFile << robots[i]->get_name() << " now has " << robots[i]->get_lives() << " lives left." << endl;

                    //cout << robots[i]->get_locationY() << " " << robots[i]->get_locationX() << endl;
                    grid[robots[i]->get_locationY()][robots[i]->get_locationX()] = '-'; //remove destroyed robot from map, correct the syntax if relevant
                    robots[i]->set_locationX(0);
                    robots[i]->set_locationY(0); // set it to 0 because setlocation -= 1;
                    this->applyUpgrade();
                    ShotSuccess = true;
                    }
                    else{
                        cout << robot_name << " tried to hit  " << robots[i]->get_name() << " but " << robots[i]->get_name() << " is hidden!" << endl;
                        outFile << robot_name << " tried to hit  " << robots[i]->get_name() << " but " << robots[i]->get_name() << " is hidden!" << endl;
                    }

                }
                else if(SemiAutoShot == true && SemiAutoShots > 0){
                    cout << "Missed! Reshooting..." << endl;
                    outFile << "Missed! Reshooting..." << endl;
                    SemiAutoShots -= 1;
                    if(SemiAutoShots == 0){
                        cout << robot_name << " missed all 3 of his shots!" << endl;
                        outFile << robot_name << " missed all 3 of his shots!" << endl;
                        ShotSuccess = true;
                    }
                }

                else{
                    cout << this->robot_name << " missed! "  << endl;
                    outFile << this->robot_name << " missed! "  << endl;
                    ShotSuccess=true;
                }
            }

        }
    }while(!ShotSuccess);
    if(this->get_shells() <= 0){
            cout << get_name() <<  " has ran out of shells and will self destruct!" << endl;
            outFile << get_name() <<  " has ran out of shells and will self destruct!" << endl;
            this->decrease_lives();
            grid[robot_locationY][robot_locationX] = '-'; //remove destroyed robot from map, correct the syntax if relevant
            robot_locationX = -1; //dead means at -1
            robot_locationY = -1; // set it to 0 because setlocation -= 1;
        }

    }

};

class SeeingRobot : virtual public Robot { 

    protected:

    void track(){
        int randomtarget = rand() % r;
        int X = robots[randomtarget]->get_locationX();
        int Y = robots[randomtarget]->get_locationY();
        if(X != -1 && Y != -1){
        cout << robot_name << " is tracking " << robots[randomtarget]->get_name() << " at " << robots[randomtarget]->get_locationX() << " " << robots[randomtarget]->get_locationY() << endl;
        outFile << robot_name << " is tracking " << robots[randomtarget]->get_name() << " at " << robots[randomtarget]->get_locationX() << " " << robots[randomtarget]->get_locationY() << endl;

    }
    }


    void scout(){

        bool validtarget = false;
        int x,y;
        cout << robot_name << " scouts the whole battlefield!" << endl;
        outFile << robot_name << " scouts the whole battlefield!" << endl;
        while(validtarget== false){
        int randomtarget = rand() % r;
        x = robots[randomtarget]->get_locationX();
        y = robots[randomtarget]->get_locationY();
        //cout << x << " " << y << endl;
        if(x != -1 && y != -1 ){
        for(int i=0; i < r; i++){
            int targetX = robots[i]->get_locationX();
            int targetY = robots[i]->get_locationY();
            if(targetX == x && targetY == y && robots[i] != this ){        //if its not at -1 and at itself
                cout << robot_name << " decides to shoot at " << robots[i]->get_name() << endl;
                outFile << robot_name << " decides to shoot at " << robots[i]->get_name() << endl;
                shoot(x,y);
                validtarget = true;
            }
        }
    }

    }
    }


    void look(){

        cout << robot_name <<" is looking around..." << endl;
        outFile << robot_name <<" is looking around..." << endl;
        int range;
        if (CanLongShot) {
                cout << robot_name << " can shoot further with LongShotBot upgrade!" << endl;
                outFile << robot_name << " can shoot further with LongShotBot upgrade!" << endl;
                range = 2;
        } else {
        range = 1;
        }

        int dx[] = {-1, 0, 1, 1, 1, 0, -1, -1};
        int dy[] = {-1, -1, -1, 0, 1, 1, 1, 0};


        for (int i = 0; i < 8; i++) {
                for (int step = 1; step <= range; step++) {
                    int a = robot_locationX + dx[i] * step;
                    int b = robot_locationY + dy[i] * step;
            //cout << grid[b][a] << " "  << a << " " << b << endl;

            if (a >= 0 && a < battlefieldwidth && b >= 0 && b < battlefieldlength){    //check if a and b is out of bounds,if true, dont look there
            if(grid[b][a] != '-'){

                switch(i){
                    case 0:
                    cout << robot_name << " found a target on the top left!" << endl;  //target = a and b, seen
                    outFile << robot_name << " found a target on the top left!" << endl;
                    foundEnemy = true;
                    shoot(a,b);
                    break;
                    case 1:
                    cout << robot_name << " found a target on top!" << endl;
                    outFile << robot_name << " found a target on top!" << endl;
                    foundEnemy = true;
                    shoot(a,b);
                    break;
                    case 2:
                    cout << robot_name << " found a target on top right!" << endl;
                    outFile << robot_name << " found a target on top right!" << endl;
                    foundEnemy = true;
                    shoot(a,b);
                    // targetX = a;
                    // targetY = b;
                    break;
                    case 3:
                    foundEnemy = true;
                    cout << robot_name << " found a target on the right!" << endl;
                    outFile << robot_name << " found a target on the right!" << endl;
                    shoot(a,b);
                    break;
                    case 4:

                    foundEnemy = true;
                    cout << robot_name << " found a target on below right!" << endl;
                    outFile << robot_name << " found a target on below right!" << endl;
                    shoot(a,b);
                    break;
                    case 5:

                    foundEnemy = true;
                    cout << robot_name << " found a target below!" << endl;
                    outFile << robot_name << " found a target below!" << endl;
                    shoot(a,b);
                    break;
                    case 6:
                    foundEnemy = true;
                    cout << robot_name << " found a target on below left!" << endl;
                    outFile << robot_name << " found a target on below left!" << endl;
                    shoot(a,b);
                    break;
                    case 7:
                    foundEnemy = true;
                    cout << robot_name << " found a target on the left!" << endl;
                    outFile  << robot_name << " found a target on the left!" << endl;
                    shoot(a,b);
                    break;
                }
            }

        }
        }
    }
    }
};

class ThinkingRobot : virtual public Robot{ 

    void think() {
        foundEnemy = false;
        cout << robot_name << " is thinking..." << endl;
        outFile  << robot_name << " is thinking..." << endl;

        if(canTrack == true){
            track();
            TrackCharges -= 1;
            if(TrackCharges == 0){
                cout << robot_name << " has run out of Track charges!";
                outFile << robot_name << " has run out of Track charges!";
                canTrack = false;
            }
        }
        if(canScout == true){
            scout();
            ScoutCharges -=1;
            if(ScoutCharges == 0){
                cout << robot_name << " has run out of Scout charges!";
                outFile << robot_name << " has run out of Scout charges!";
                canScout = false;
            }
        }
        else {
            this->look();
            if(this->foundEnemy == false){
            cout << robot_name << " looked around but didnt found anyone!" << endl;
            outFile << robot_name << " looked around but didnt found anyone!" << endl;
            move();
        }



            if(IsHiding == true){
            this->ReduceHideCharge();// at the end of a turn, reduce hidecharges
            }
            }
        cout << endl;


    }
};

class GenericRobot : public ShootingRobot,public MovingRobot,public SeeingRobot,public ThinkingRobot //derived robot class from robots
{
    private:
        int upgrades_left;
    public:

    void applyUpgrade() {
        if (getUpgradeCount() >= 3) {
            cout << robot_name << " cannot be upgraded anymore." << endl;
            outFile << robot_name << " cannot be upgraded anymore." << endl;
            return;
        }
        vector<string> allAreas = {"moving", "shooting", "seeing","buff"};
        vector<string> notChosenAreas;
        vector<string> MovingUpgrades = {"HideBot","JumpBot"};
        vector<string> ShootingUpgrades = {"LongShotBot","SemiAutoBot","ThirtyShotBot"};
        vector<string> SeeingUpgrades = {"ScoutBot","TrackBot"};
        // Step 3: Find which areas have NOT been chosen yet

        for (string area : allAreas) {
            if (!hasUpgradeArea(area)) {
                notChosenAreas.push_back(area);
            }
        }
        // Step 4: Pick the first available area for simplicity (or choose randomly)
        int randChosenArea = rand() % notChosenAreas.size();
        string chosenArea = notChosenAreas[randChosenArea];
        //string chosenArea = "shooting";
        //addUpgradeArea(chosenArea); // Record that this area has been used

          if (chosenArea == "moving") {
            int randMovingUpgrade = rand() % MovingUpgrades.size();
            string chosenUpgrade = MovingUpgrades[randMovingUpgrade];
            //string chosenUpgrade = "HideBot";
            cout << robot_name << " upgrades with " << chosenUpgrade << "!" << endl;
            outFile << robot_name << " upgrades with " << chosenUpgrade << "!" << endl;
            if (chosenUpgrade == "Hidebot") {
                this->SetcanHide();
            }
            else if(chosenUpgrade == "JumpBot"){
                this->SetcanJump();
            }


        } else if (chosenArea == "shooting") {
            int randShootingUpgrade = rand() % ShootingUpgrades.size();
            string chosenUpgrade = ShootingUpgrades[randShootingUpgrade];
            //string chosenUpgrade = "LongShotBot";
            cout << robot_name << " upgrades with " << chosenUpgrade << "!" << endl;
            outFile << robot_name << " upgrades with " << chosenUpgrade << "!" << endl;
            if(chosenUpgrade == "LongShotBot"){
                CanLongShot = true;
            }
            else if (chosenUpgrade == "SemiAutoBot"){
                SemiAutoShot = true;
            }
            else if (chosenUpgrade == "ThirtyShotBot"){
                shells = 30;
                cout << robot_name << "upgrades to ThirtyShotBot and is locked and loaded with 30 shells!" << endl;
            }

        } else if (chosenArea == "seeing") {
            int randSeeingUpgrade = rand() % SeeingUpgrades.size();
            string chosenUpgrade = SeeingUpgrades[randSeeingUpgrade];
            //string chosenUpgrade = "ScoutBot";
            cout << robot_name << " upgrades with " << chosenUpgrade << "!" << endl;
            outFile << robot_name << " upgrades with " << chosenUpgrade << "!" << endl;
            if(chosenUpgrade == "ScoutBot"){
                canScout = true;
            }
            else if (chosenUpgrade == "TrackBot"){
                canTrack = true;
            }
        }


    }




};


// === FUNCTION PROTOTYPES ===
void DisplayBattlefield();
void AnalyseFile(string line);
void respawnRobot();
void SpawnRobots();
// === MAIN PROGRAM ===
int main(){
    srand (time(0)); //to make random number work, else same number will keluar everytime
    // vector <Robot> robotsvector;  //if robotsvector is declared here, its value is empty
    ifstream MyReadFile(filetoread);
    // int max_robots = robotamount;
    // Robot* robots[max_robots];
    // Variable to store each line from the file
    string line;
    bool win_con1 = false; //win by last robot standing
    bool win_con2 = false; //win by turn step running out

    // Read each line from the file and print it
    while (getline(MyReadFile, line)) {
        // Process each line as needed
        AnalyseFile(line);
        cout << line << endl;
        outFile << line << endl;
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

    outFile << endl;
    outFile << "Battlefield length = " << battlefieldlength << endl;
    outFile << "Battlefield width = " << battlefieldwidth << endl;
    outFile << "Steps = "<< steps << endl;
    outFile << "Amount of robots = "<< robotamount << endl;
    outFile << endl;



    for (int i =0;i < battlefieldlength;i++){
        // string line(battlefieldwidth,'-');                                        
        //grid[] += string (battlefieldwidth,'-');   
        grid.push_back(string (battlefieldwidth,'-'));   
    }
    DisplayBattlefield();
    // robots[0]->TakeTurn(); //one robot interaction
    // DisplayBattlefield();
    // respawnRobot();
    // DisplayBattlefield();
     Robot* W = nullptr; //declaring W to ensure the scope covers the methods from robot class
     string input;
    int halfway = steps / 2;
     while(steps > 0){

        int bots_left = 0; //bot counter
        if (steps == halfway){
            SpawnRobots();
            DisplayBattlefield();
            cout << endl;
        }

        for(int i = 0; i < r; i++){
            robots[i]->TakeTurn();                      //comment these out to make it run only for one robot
        }
        respawnRobot();
        DisplayBattlefield();



      steps -=1;
      turncounter +=1;

      for(int i = 0 ; i < r ; i++){
            if(robots[i]->isAlive()){
                bots_left++;
                 W = robots[i];

            }
              }
              if(bots_left ==1){
                win_con1 = true;
                cout << W->get_name() << " has won the game!" << endl;
                outFile << W->get_name() << " has won the game!" << endl;
                exit(0);



              }
   }
   if(steps <= 0 && win_con1 == false){

    win_con2 = true;
    cout << "The game has ran out of turns! The robots have come to a stalemate!" << endl << endl;
    outFile << "The game has ran out of turns! The robots have come to a stalemate!" << endl << endl;

    cout << "B-b-baka w-what are you d-doing. I-i need one.. one more turn to win!" << endl;
    outFile << "B-b-baka w-what are you d-doing. I-i need one.. one more turn to win!" << endl;
    
    cout << "Y/N?" << endl;
    outFile << "Y/N?" << endl;
    cin >> input ;

    if(input== "y" || input =="Y"){


        while(win_con1 != true){


            int bots_left = 0; //bot counter
        for(int i = 0; i < r; i++){
            robots[i]->TakeTurn();                      //comment these out to make it run only for one robot
        }
        respawnRobot();
        DisplayBattlefield();


      steps -=1;
      turncounter +=1;

      for(int i = 0 ; i < r ; i++){
            if(robots[i]->isAlive()){
                bots_left++;
                 W = robots[i];

            }
              }
              if(bots_left ==1){
                win_con1 = true;
                cout << W->get_name() << " has won the game!" << endl;
                outFile << W->get_name() << " has won the game!" << endl;
                exit(0);


              }

        }

    }else{

        exit(0);
    }
   }
}
// === FUNCTION DECLARATIONS ===
void DisplayBattlefield(){
    for(int b=0;b < r;b++){
        //grid[robotsvector[b].get_locationX()-1][robotsvector[b].get_locationY()-1] = 'O'; //MINUS ONE BECAUSE ARRAYGRID START FROM 0
        //cout << robots[b]->get_locationX() << "," <<  robots[b]->get_locationY() << endl;
        int y = robots[b]->get_locationY();
        int x = robots[b]->get_locationX();
        //cout << x << " " << y << endl;
        if(y != -1 && x != -1){
            //grid[y][x] = 'R'; //grid[Y][X]                           
            grid[y][x] = robots[b]->get_name()[0];//segment error here 
        }

    }
    // Print top X-axis labels
    cout << endl;
    outFile << endl;
    cout << "Turn: " << turncounter << endl;
    outFile << "Turn: " << turncounter << endl;
    cout << "    "; // Padding before column numbers
    outFile << "    ";
    for (int i = 0; i < grid[0].size(); i++) {
        cout << setw(3) << (i + 1);
        outFile << setw(3) << (i + 1);
    }
    cout << endl;
    outFile << endl;

    for (int i = 0; i < grid.size(); i++) {
        cout << setw(2) << (i + 1) << " |"; // Y-axis label
        outFile << setw(2) << (i + 1) << " |";
        for (int j = 0; j < grid[i].size(); j++) {
            cout << setw(3) << grid[i][j]; // experiment with setw to make cell same width
            outFile  << setw(3) << grid[i][j];
        }
        cout << endl ;
        outFile << endl ;
    }
    cout << endl ;
    outFile << endl ;


}

void SpawnRobots(){
    //cout << r << endl;  //5
    int amountToSpawn = 3;
    bool occupied=false;
    vector <string> namelist = {"Gragas","Yuven","Zamane"};
    for(int i= 0;i < amountToSpawn;i++){
        robots[r] = new GenericRobot;
        robots[r]->set_name(namelist[i]);
        int X, Y;
        do {
            X = rand() % battlefieldlength;
            Y = rand() % battlefieldwidth;
        } while (grid[Y][X] != '-' || X ==0 || Y == 0 || X == battlefieldlength || Y == battlefieldwidth);
        robots[r]->set_locationX(X);
        robots[r]->set_locationY(Y);
        cout << endl;
        cout << robots[r]->get_name() << " Bot has spawned at " << "X:" << X-1 << " Y:"  << Y-1 << "!" << endl;
        outFile << endl;
        outFile << robots[r]->get_name() << " Bot has spawned at " << "X:" << X-1 << " Y:"  << Y-1 << "!" << endl;
        r++;

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
            robots[r]->set_locationX(rand() % battlefieldlength + 1); //set it to a random location
        }
        if (Y != "random"){
            robots[r]->set_locationY(stoi(Y));
        }
        else{
            robots[r]->set_locationY(rand() % battlefieldwidth + 1);
        }
        r++;

    }

void respawnRobot(){ //when called will loop thru the list of respawning robots and spawn all of them in random location

        vector<Robot*> Deaddead;
        //check for empty
        //check for robots number
        //check for if another robot is on the tile

            if(!respawnlist.empty()){
                Robot* R = respawnlist.front();

                if(R->isAlive()){
                        bool placed = false;

                        while(!placed){

                                int randX, randY;
                                do{

                                      randX = (rand() % battlefieldlength)+1; //generate location
                                      randY = (rand() % battlefieldwidth)+1;

                                }
                                while(randX ==0 || randY == 0 || randX == battlefieldlength || randY == battlefieldwidth);


                            if(grid[randY][randX] == '-' && !isalpha(grid[randY][randX])){ //if it is empty, if = false, program breaks

                                R->set_locationX(randY); //set location
                                R->set_locationY(randX);
                                //cout << R->get_locationY() << " " << R->get_locationX() << endl;
                                grid[R->get_locationY()][R->get_locationX()] = R->get_name()[0];
                                cout << R->get_name() << " respawned at coordinates " << R->get_locationX()+1 << " " << R->get_locationY()+1 << "!" << endl;
                                outFile << R->get_name() << " respawned at coordinates " << R->get_locationX()+1 << " " << R->get_locationY()+1 << "!" << endl;
                                placed = true;
                                respawnlist.erase(respawnlist.begin()); //remove the first element

                            }
                        }
                }

                else{

                    Deaddead.push_back(R); //if the robot is no longer alive, put it into another vector list
                    cout << R->get_name() <<  " is eliminated and will no longer respawn!" << endl;
                    outFile << R->get_name() <<  " is eliminated and will no longer respawn!" << endl << endl;
                    respawnlist.erase(respawnlist.begin()); //remove dead robot from respawnlist
                    }






            }else{
                cout << "Nobody died last turn." << endl;
                outFile << "Nobody died last turn." << endl;
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
        cout << "Invalid command! " << endl;
        outFile << "Invalid command! " << endl;
    }
}
