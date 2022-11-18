#ifdef SERVER
#include"server.h"
#elif CLIENT
#include"client.h"
#endif




constexpr int PORT = 8888;


int main(int argc, char* argv[])
{

#ifdef SERVER
    char const* ip = "0.0.0.0";
    int port = PORT;

    
        for(int i = 1; i < argc;i++)
        {
            if(i + 1 >= argc)
            {
                printf("Missing argument exiting..\n");
                exit(-1);
            }

            else if(!strcmp(argv[i], "-ip"))
            {
                ip = argv[i + 1];
                i++;
            }

            else if(!strcmp(argv[i], "-port"))
            {
                port = atoi(argv[i + 1]);
                i++;
            }

            else
            {
                printf("Invalid argument exiting..\n");
                exit(-1);
            }
        }
    Server(ip, port).run();

#elif CLIENT
    char * ip = 0;
    int port = PORT;

    
        for(int i = 1; i < argc;i++)
        {
            if(i + 1 >= argc)
            {
                printf("Missing argument exiting..\n");
                exit(-1);
            }
            else if(!strcmp(argv[i], "-ip"))
            {
                ip = argv[i + 1];
                i++;
            }

            else if(!strcmp(argv[i], "-port"))
            {
                port = atoi(argv[i + 1]);
                i++;
            }

            else
            {
                printf("Invalid argument exiting..\n");
                exit(-1);
            }
        }
    Client(ip, port).run(); 
#endif
    return 0;
}


