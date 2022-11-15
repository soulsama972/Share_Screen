
#ifdef SERVER
#include"server.h"
#elif CLIENT
#include"client.h"
#endif

//#define SERVER 1
//#define CLIENT 1

int main(int argc, char* argv[])
{

#ifdef SERVER
    Server().run();
#elif CLIENT
    if (__argc != 2) 
    {
        printf("please pass to command line the host ip \n");
        return 0;
    }
    Client(argv[1]).run(); 
#endif
    return 0;
}


