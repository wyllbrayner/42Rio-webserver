#ifndef SMT_HPP
#define SMT_HPP

#include <cstddef>

namespace smt {

template<typename T>
class unique_ptr {
    public:

        unique_ptr() : pointer(NULL) {}

        unique_ptr(T* ptr) : pointer(ptr) {}

        ~unique_ptr() {
            if (pointer != NULL) { delete pointer; }
        }

        T* operator->() { return this->pointer; }

        T& operator*() { return *(this->pointer); }

        operator bool() const { return (this->pointer != NULL); }

        operator bool() { return (this->pointer != NULL); }

    private:

        unique_ptr(unique_ptr const& other);
        unique_ptr& operator=(unique_ptr const& other);

        T* pointer;
};

template<typename T>
class shared_ptr {
    public:

        shared_ptr() : pointer(NULL), referenceCount(new unsigned int(0)) {}

        shared_ptr(T* ptr)
            : pointer(ptr), referenceCount(new unsigned int(1)) {}

        shared_ptr(shared_ptr const& other) {
            this->pointer = other.pointer;
            this->referenceCount = other.referenceCount;
            if (this->pointer != NULL) (*(this->referenceCount))++;
        }

        shared_ptr& operator=(shared_ptr const& other) {
            destroy();
            this->pointer = other.pointer;
            this->referenceCount = other.referenceCount;
            if (this->pointer != NULL) (*(this->referenceCount))++;
            return (*this);
        }

        ~shared_ptr() { destroy(); }

        bool operator==(shared_ptr const& other) const {
            return (this->pointer == other.pointer);
        }

        T* operator->() { return this->pointer; }

        T* operator->() const { return this->pointer; }

        T& operator*() { return *(this->pointer); }

        operator bool() const { return (this->pointer != NULL); }

    private:

        void destroy() {
            if (*referenceCount) (*referenceCount)--;
            if (*referenceCount == 0) {
                if (pointer != NULL) delete pointer;
                delete referenceCount;
            }
        }

        T*            pointer;
        unsigned int* referenceCount;
};

template<typename T>
shared_ptr<T> make_shared(T* t) {
    return (shared_ptr<T>(t));
}

} // namespace smt

#endif /* SMT_HPP */