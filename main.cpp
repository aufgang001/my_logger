#include <my_logger.hpp>

#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>


void variadic_test()
{

    my_logger l("variadic", "./", false);
    l.log("host0", 1);
    l.log("host0", 1, 2);
    l.log("host0", 1, 2, 3);
    l.log("host0", 1, 2, 3, 4);
    l.log("host0", 1, 2, 3, 4, 5);
    l.log("host0", 1, 2, 3, 4, 5, 6);
}


void ipc_test(const std::string& file_suffix, const std::string& folder, bool ipc_save, const std::string& msg, unsigned int thread_num)
{
    my_logger l(file_suffix, folder, ipc_save);

    for (int i = 0; i < 1000; i++) {
        l.log("server", "send", msg, thread_num);
    }

}

int main(int, char**)
{
    std::cout << "this is the test" << std::endl;
    //variadic_test();

    unsigned int t_num = 10;
    std::vector<std::thread> v(t_num);

    for (unsigned int i = 0; i < t_num; ++i) {
        v[i] = std::thread(ipc_test, "ipc", "./", false, "XXXXXXXXXXXXXXX", i);
    }

    for (unsigned int i = 0; i < t_num; ++i) {
        v[i].join();
    }









    //bool ipc_save = false;
    //int pid;
    //switch ( pid = fork() ) {
    //case -1:   [> Fehler bei fork() <]
    //std::cout << "failed to fork" << std::endl;
    //break;
    //case 0: { [> Hier befinden Sie sich im Kindprozess   <]
    //my_logger l("host0", "./test_logging", ipc_save);
    //for (int i = 0; i < 1000; i++) {
    //l.log("server", "send", "child prozess hallo XXXX");
    //}
    //}
    //break;
    //default: { [> Hier befinden Sie sich im Elternprozess <]
    //my_logger l("host0", "./test_logging", ipc_save);
    //for (int i = 0; i < 1000; i++) {
    //l.log("server", "send", "parent prozess hallo YYYYYY");
    //}
    //}
    //break;
    //}


    //return 0;
}

