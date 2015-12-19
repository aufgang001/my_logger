#include "my_logger.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

my_logger::my_logger(bool set_ipc_save)
    : m_ipc_save(set_ipc_save)
    , m_host_name(get_host_name())
    , m_log_folder(get_log_folder())
    , m_counter(0)
{
}

my_logger::my_logger(const std::string& host_name, const std::string& log_folder, bool set_ipc_save)
    : m_ipc_save(set_ipc_save)
    , m_host_name(host_name)
    , m_log_folder(log_folder)
    , m_counter(0)
{
}

std::string my_logger::get_host_name() const
{
    if (const char* env_p = std::getenv("HOST")) {
        return env_p;
    } else {
        return "";
    }
}

std::string my_logger::get_log_folder() const
{
    if (const char* env_p = std::getenv("LOGFOLDER")) {
        return env_p;
    } else {
        return "";
    }
}

std::string my_logger::get_log_file_name(const std::string& file_name_suffix) const
{
    return  m_log_folder + m_host_name + "_" + file_name_suffix + ".my_logfile";
}

void my_logger::log(const std::string& file_name_suffix, const std::string& action, const std::string& description)
{
    if (!m_log_folder.empty() && !m_host_name.empty()) {
        std::stringstream content;

        std::string current_time = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
        content << m_counter++ << ";" << current_time << ";" << action << ";" << description;

        auto it = m_log_files.find(file_name_suffix);
        if (it == m_log_files.end()) {
            try {
                auto lprops = std::make_shared<log_props>(this, file_name_suffix);
                lprops->write_to_file(content);
                m_log_files.insert({file_name_suffix, lprops});
            } catch (boost::interprocess::interprocess_exception& ex) {
                std::cout << "ipc error: " << ex.what() << std::endl;
                exit(0);
            }

        } else {
            it->second->write_to_file(content);
        }
    }
}

///////////////////////////////////////////////
// log_props
///////////////////////////////////////////////

my_logger::log_props::log_props(const my_logger* logger, const std::string& file_name_suffix)
    : m_file_name_suffix(file_name_suffix)
    , m_can_log(false)
    , m_my_logger(logger)
    , m_mutex(boost::interprocess::open_or_create, get_mutex_name().c_str())
{
    m_file_handle = std::make_shared<std::ofstream>();
    m_file_handle->open(m_my_logger->get_log_file_name(m_file_name_suffix), std::ofstream::app);
    if (m_file_handle->is_open()) {
        m_can_log = true;
    } else {
        m_can_log = false;
        std::cout << "failed to open logfile: " << m_my_logger->get_log_file_name(m_file_name_suffix)<< std::endl;
    }
}

my_logger::log_props::~log_props()
{
    m_file_handle->close();
    boost::interprocess::named_mutex::remove(get_mutex_name().c_str());
}

void my_logger::log_props::write_to_file(const std::stringstream& content)
{
    //todo sync stuff
    if (m_can_log) {
        if (m_my_logger->m_ipc_save) {
            m_mutex.lock();
        }

        *m_file_handle << content.str() << std::endl;

        if (m_my_logger->m_ipc_save) {
            m_mutex.unlock();
        }
    }
}

std::string my_logger::log_props::get_mutex_name() const{
    return m_file_name_suffix + m_my_logger->m_host_name;
}
