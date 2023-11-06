#include "webserv.hpp"

void print_log(std::string const &msg)
{
    std::cerr << msg << "\n";
}

void make_socket_nonblock(int fd)
{
    // int flags = fcntl(fd, F_GETFL, 0);
    // if (flags == -1)
    //     throw(FailToGetFdStatus());
    // if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    //     throw(FailToSetFdStatus());
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
        throw(FailToSetFdStatus());
}

bool directory_exist(std::string const &dir)
{
    DIR *pDir;
    bool bExists = false;

    pDir = opendir(dir.c_str());

    if (pDir != NULL)
    {
        bExists = true;
        (void)closedir(pDir);
    }

    return bExists;
}

bool uri_exist(std::string const &file)
{
    std::ifstream is(file.c_str());
    if (!is)
        return false;
    return true;
}

bool	bigger_than(std::string const &a, std::string const &b)
{
    if (a.size() != b.size())
        return a.size() > b.size();

    for (size_t i = 0 ; i < a.size() ; ++i)
        if (a[i] != b[i])
            return a[i] > b[i];


    return false;
}

bool	is_number(std::string const &num)
{
    if (num.size() == 0)
        return false;
    if (num[0] == '0')
        return num.size() == 1;
    for (size_t i = 0 ; i < num.size() ; ++i)
        if (!isdigit(num[i]))
            return false;

    return true;
}

int		string_to_int(std::string const &str)
{
    int ans = 0;
    for (size_t i = 0 ; i < str.size() ; ++i)
    {
        ans = 10 * ans + str[i] - '0';
    }
    return ans;
}


bool	is_executable(std::string const &file_name)
{
    return access(file_name.c_str(), X_OK) == 0;
}

bool	is_readable(std::string const &file_name)
{
    return access(file_name.c_str(), R_OK) == 0;
}

std::string get_file_type(std::string const &file_uri)
{
    size_t dot = file_uri.find_last_of(".");

    if (dot == std::string::npos)
        return "";
    return file_uri.substr(dot + 1);
}

std::string get_file_content(std::string const &content)
{
    std::ifstream input_file(content.c_str());
    if (!input_file.is_open())
        throw(UnsupportedMediaType); // todo : check ?

    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

std::string file_or_directory(std::string const &uri)
{
    return directory_exist(uri) ? "Directory" : "File";
}

// static std::string plus_one(std::string number)
// {
//     bool plus_one = false;
//     for (int i = static_cast<int>(number.size() - 1) ; i >= 0 ; --i)
//     {
//         if (plus_one)
//         {
//             if (number[i] == '9')
//             {
//                 plus_one = true;
//                 number[i] = '1';
//             }
//             else if (number[i] == '8')
//             {
//                 plus_one = true;
//                 number[i] = '0';
//             }
//             else
//             {
//                 number[i] += 2;
//                 return number;
//             }
//         }
//         else
//         {
//             if (number[i] == '9')
//             {
//                 plus_one = true;
//                 number[i] = '0';
//             }
//             else
//             {
//                 ++number[i];
//                 return number;
//             }
//         }
//     }
//     if (plus_one)
//         number.insert(0, "1");
//     return number;
// }

void	change_file_name(std::string &file_name)
{
    // todo
    // std::cerr << file_name <<"\n";
    // std::string current_suffix = "0";
    // if (file_name[file_name.size() - 1] == ')')
    // {
    //     size_t left_parenthese_index = file_name.size() - 2;
    //     while (--left_parenthese_index >= 0)
    //         if (file_name[left_parenthese_index] == '(')
    //             break ;
    //     if (left_parenthese_index >= 0)
    //     {
    //         std::string number = file_name.substr(left_parenthese_index + 1, file_name.size() - left_parenthese_index - 2);
    //         if (is_number(number))
    //         {
    //             file_name.erase(left_parenthese_index);
    //             current_suffix = plus_one(number);
    //         }
    //     }
    // }
    // file_name += "(" + current_suffix + ")";
    while (uri_exist(file_name))
    {
        size_t dot = file_name.find_last_of(".");
        if (dot == std::string::npos)
            file_name += "_";
        else
            file_name.insert(dot, "_");
    }
}

void	delete_file(std::string const &file_name)
{
    std::remove(file_name.c_str());

    if (uri_exist(file_name))
        throw(Conflict);
}

std::string ft::to_string(int n)
{
    std::ostringstream convert;
    convert << n;
    return convert.str();
}

std::string ft::to_string(size_t n)
{
    std::ostringstream convert;
    convert << n;
    return convert.str();
}

std::string ft::to_string(long long n)
{
    std::ostringstream convert;
    convert << n;
    return convert.str();
}
