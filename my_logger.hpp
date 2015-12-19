#ifndef MY_LOGGER_HPP
#define MY_LOGGER_HPP


#include <string>
#include <chrono>
#include <memory>
#include <map>

#include <boost/interprocess/sync/named_mutex.hpp>

class my_logger
{
public:
    my_logger(bool set_ipc_save=false);
    my_logger(const std::string& host_name, const std::string& log_folder, bool set_ipc_save=false);

    void log(const std::string& file_name_suffix, const std::string& action, const std::string& description);

private:
    struct log_props {
        log_props(const my_logger* logger, const std::string& file_name_suffix);
        ~log_props();

        void write_to_file(const std::stringstream& content);

        std::string get_mutex_name() const;

        std::shared_ptr<std::ofstream> m_file_handle;
        std::string m_file_name_suffix;
        bool m_can_log;
        const my_logger* m_my_logger;
        boost::interprocess::named_mutex m_mutex;
    };


    std::string get_host_name() const;

    std::string get_log_folder() const;

    std::string get_log_file_name(const std::string& file_name_suffix) const;

    bool m_ipc_save;
    std::string m_host_name;
    std::string m_log_folder;
    int m_counter;

    //file_name_suffx, file_handle
    std::map<std::string, std::shared_ptr<log_props>> m_log_files;
};

#endif //MY_LOGGER_HPP
