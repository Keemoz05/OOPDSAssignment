#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

class robot(){
    private:
    string robot_type;
    string robot_name;
    int robot_locationX;
    int robot_locationY;
    int robot_lives;


    public:
    robot(); //define default constructor parameters
    robot(type string, name string, locationX int, locationY int){
        type = robot_type;
        name = robot_name;
        locationX = robot_locationX;
        locationY = robot_locationY;


    }
    get_robotInfo(); //member func prototypes
    get_lives();
    isAlive();

};

string filetoread = "examplefile.txt";
int battlefieldlength = 0;
int battlefieldwidth = 0;
int steps = 0;
int robots = 0;


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
        //create robot object with Robot robot1(Kidd,3,6)
    }
    else {
        cout << "invalid command ts pmo " << endl;
    }
}  

int main(){

    ifstream MyReadFile(filetoread);
    
    // Variable to store each line from the file
    string line;

    // Read each line from the file and print it
    while (getline(MyReadFile, line)) {
        // Process each line as needed
        AnalyseFile(line);
        cout << line << endl;

    }
    cout << endl;
    cout << "Battlefield length = " << battlefieldlength << endl;
    cout << "Battlefield width = " << battlefieldwidth << endl;
    cout << "Steps = "<< steps << endl;
    cout << "Amount of robots = "<< robots << endl;
}
