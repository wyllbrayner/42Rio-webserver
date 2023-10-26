#include "utils/Route.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>

TEST(SmartPointerTests, TestUniquePtr) {
    int* numberPtr = new int(42);

    smt::unique_ptr<int> intUniquePtr(numberPtr);
    ASSERT_EQ(*intUniquePtr, 42);
}

TEST(SmartPointerTests, TestTwoUniquePtr) {
    int* numberPtr = new int(42);

    smt::unique_ptr<int> intUniquePtr(numberPtr);
    smt::unique_ptr<int> intOtherUniquePtr;

    // The code commented bellow should NOT compile
    // smt::unique_ptr<int> intOtherUniquePtr(intUniquePtr);
    // intOtherUniquePtr = intUniquePtr;
    ASSERT_EQ(*intUniquePtr, 42);
}

TEST(SmartPointerTests, TestSharedPtr) {
    int* numberPtr = new int(42);

    smt::shared_ptr<int> intSharedPtr(numberPtr);

    ASSERT_EQ(*intSharedPtr, 42);
}

TEST(SmartPointerTests, TestTwoSharedPtr) {
    int* numberPtr = new int(42);

    smt::shared_ptr<int> intSharedPtr(numberPtr);
    smt::shared_ptr<int> intOtherSharedPtr(intSharedPtr);

    ASSERT_EQ(*intSharedPtr, 42);
    ASSERT_EQ(*intOtherSharedPtr, 42);
}

TEST(SmartPointerTests, TestVectorSharedPtr) {
    int* numberPtr = new int(42);

    smt::shared_ptr<int> intSharedPtr(numberPtr);
    smt::shared_ptr<int> intOtherSharedPtr(intSharedPtr);

    std::vector<smt::shared_ptr<int> > vec;
    vec.push_back(intSharedPtr);
    vec.push_back(intOtherSharedPtr);
    ASSERT_EQ(*vec.at(0), 42);
    ASSERT_EQ(*vec.at(1), 42);
}

TEST(SmartPointerTests, TestVectorSharedPtrSockets) {
    ft::file* addrPtr = new ft::file("./websites");

    smt::shared_ptr<ft::file> RouteSharedPtr(addrPtr);
    smt::shared_ptr<ft::file> RouteOtherSharedPtr(RouteSharedPtr);

    std::vector<smt::shared_ptr<ft::file> > vec;
    vec.push_back(RouteSharedPtr);
    vec.push_back(RouteOtherSharedPtr);
    ASSERT_EQ(std::string(*vec.at(0)), std::string(*vec.at(1)));
}

TEST(SmartPointerTests, TestMakeShared) {
    ASSERT_NO_THROW(smt::shared_ptr<ft::file> Route =
                        smt::make_shared(new ft::file("./websites/")));
}