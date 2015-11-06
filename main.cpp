#include <my_logger.hpp>

#include <unistd.h>
#include <iostream>



int main(int, char**)
{
    std::cout << "this is the test" << std::endl;
    bool ipc_save = false;
    int pid;
    switch ( pid = fork() ) {
    case -1:   /* Fehler bei fork() */
        std::cout << "failed to fork" << std::endl;
        break;
    case 0: { /* Hier befinden Sie sich im Kindprozess   */
        my_logger l("host0", "./test_logging", ipc_save);
        for (int i = 0; i < 1000; i++) {
            l.log("server", "send", "child prozess hallo XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
        }
    }
    break;
    default: { /* Hier befinden Sie sich im Elternprozess */
        my_logger l("host0", "./test_logging", ipc_save);
        for (int i = 0; i < 1000; i++) {
            l.log("server", "send", "parent prozess hallo YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY");
        }
    }
    break;
    }


    return 0;
}
