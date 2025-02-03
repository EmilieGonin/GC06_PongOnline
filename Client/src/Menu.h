
#include "raylib.h"


class Menu {
public:
    Menu();
    ~Menu();

    void Run();  

private:
    char username[20];
    char serverIP[20];
    bool editUser;
    bool editIP;
   

    void Draw();       
    void HandleInput(); 
};

