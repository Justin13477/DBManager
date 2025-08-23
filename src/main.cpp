
#include "App.h"


// Make a composite class and bring all functionality into it and manage all app data/state in a single place 

// TODO: 
// fix crash when deleting currently selected table
// add config tab for db and log file load
// add theme menu
// add json export of vec<vec<string>> 


int main(void)
{  
    // add some restart logic here or wrap it in another file
    // create struct for loading init data 
    // have init struct in core data types
    std::string appName = "Window";
    std::string logFileName = "log.txt";

    bool running = true;
    
    while(running)
    {
        // here 
        App app(appName, logFileName);
        app.Run(running);
        
    }
    

}