#include "utils/ft_array.hpp"

namespace ft {

array::array(void) {}

array::array(std::vector<std::string> const& vec) : m_vec(vec) {}

array::array(array const& src) { *this = src; }

array::~array(void) {}

array& array::operator=(array const& rhs) {
    if (this != &rhs) { m_vec = rhs.m_vec; }
    return *this;
}

array::operator char**(void) const {
    char** arr = new char*[m_vec.size() + 1];

    for (size_t i = 0; i < m_vec.size(); ++i) {
        arr[i] = new char[m_vec[i].size() + 1];
        strcpy(arr[i], m_vec[i].c_str());
    }
    arr[m_vec.size()] = NULL;
    return (arr);
}

void array::erase(char** arr) {
    for (int i = 0; arr[i]; i++) { delete[] arr[i]; }
    delete[] arr;
}

} // namespace ft
