#include "utils/ft_filesystem.hpp"
#include "utils/ft_string.hpp"

#include <gtest/gtest.h>

TEST(testDirectory, testConstructor) {
    ft::directory dir1;
    ft::directory dir2("test");
    ft::directory dir3(std::string("test"));

    EXPECT_EQ(dir1, "");
    EXPECT_EQ(dir2, "test/");
    EXPECT_EQ(dir3, "test/");
}

TEST(testDirectory, testAssignment) {
    ft::directory dir1;
    ft::directory dir2("test");
    ft::directory dir3(std::string("test"));

    dir1 = "test";
    EXPECT_EQ(dir1, "test/");
    dir1 = ft::directory("test");
    EXPECT_EQ(dir1, "test/");
}

TEST(testDirectory, testExists) {
    ft::directory dir("test");

    EXPECT_EQ(dir.exists(), false);

    system("mkdir test");
    EXPECT_EQ(dir.exists(), true);
    system("rm -rf test");
}

TEST(testDirectory, testIsValid) {
    ft::directory dir("tmpDir");

    EXPECT_FALSE(dir.isValid());

    system("touch tmpDir");
    EXPECT_FALSE(dir.isValid());
    system("rm tmpDir");

    system("mkdir tmpDir");
    EXPECT_TRUE(dir.isValid());
    system("rm -rf tmpDir");

    system("mkdir ../tmpDir");
    EXPECT_FALSE(dir.isValid());
    system("rm -r ../tmpDir");
}

TEST(testDirectory, testIsFile) {
    ft::directory dir1;
    ft::directory dir2("test.tmp");

    EXPECT_EQ(dir1.isFile(), false);
    EXPECT_EQ(dir2.isFile(), false);

    system("touch test.tmp");
    EXPECT_EQ(dir1.isFile(), false);
    EXPECT_EQ(dir2.isFile(), true);
    system("rm test.tmp");
}

TEST(testDirectory, testIsCrawler) {
    EXPECT_EQ(ft::directory().isCrawler(), false);
    EXPECT_EQ(ft::directory(".").isCrawler(), false);
    EXPECT_EQ(ft::directory("../test/").isCrawler(), true);
    EXPECT_EQ(ft::directory("test/.././../test/../").isCrawler(), true);
    EXPECT_EQ(ft::directory("test/../../test/").isCrawler(), true);
    EXPECT_EQ(ft::directory("../").isCrawler(), true);
}

TEST(testFile, testConstructor) {
    ft::file file1;
    ft::file file2("file");
    ft::file file3(ft::file("file"));

    EXPECT_EQ(file1, "");
    EXPECT_EQ(file2, "file");
    EXPECT_EQ(file3, "file");
}

TEST(testFile, testFileExists) {
    ft::file file1;
    ft::file file2("file");

    EXPECT_EQ(file1.exists(), false);
    EXPECT_EQ(file2.exists(), false);

    system("touch file");
    EXPECT_EQ(file1.exists(), false);
    EXPECT_EQ(file2.exists(), true);
    system("rm file");
}

TEST(testFile, testIsDirectory) {
    ft::file file1;
    ft::file file2("file");

    EXPECT_EQ(file1.isDirectory(), false);
    EXPECT_EQ(file2.isDirectory(), false);

    system("mkdir file");
    EXPECT_EQ(file2.isDirectory(), true);
    system("rm -rf file");
}

TEST(testFile, testIsValid) {
    ft::file file("file");

    EXPECT_FALSE(file.isValid());

    system("touch file");
    EXPECT_TRUE(file.isValid());
    system("rm file");

    system("mkdir file");
    EXPECT_FALSE(file.isValid());
    system("rm -rf file");

    system("touch ../file");
    EXPECT_FALSE(file.isValid());
    system("rm ../file");
}

TEST(testFile, testIsCrawler) {
    EXPECT_EQ(ft::file().isCrawler(), false);
    EXPECT_EQ(ft::file(".").isCrawler(), false);
    EXPECT_EQ(ft::file("../test").isCrawler(), true);
    EXPECT_EQ(ft::file("test/.././../test/../").isCrawler(), true);
    EXPECT_EQ(ft::file("test/../../test").isCrawler(), true);
    EXPECT_EQ(ft::file("../").isCrawler(), true);
}

TEST(testFile, testGetExtension) {
    ft::file file1;
    ft::file file2("file");
    ft::file file3("file.txt");
    ft::file file4("file.tar.gz");
    ft::file file5("file.tar.gz.tmp");

    EXPECT_EQ(file1.getExtension(), "");
    EXPECT_EQ(file2.getExtension(), "");
    EXPECT_EQ(file3.getExtension(), ".txt");
    EXPECT_EQ(file4.getExtension(), ".gz");
    EXPECT_EQ(file5.getExtension(), ".tmp");
}

TEST(testFile, testGetDirectory) {
    ft::file file1;
    ft::file file2("file");
    ft::file file3("test/file.txt");
    ft::file file4("test/python/file.tar.gz");

    EXPECT_EQ(file1.getDirectory(), "./");
    EXPECT_EQ(file2.getDirectory(), "./");
    EXPECT_EQ(file3.getDirectory(), "test/");
    EXPECT_EQ(file4.getDirectory(), "test/python/");
}

TEST(testFile, testReadAndWriteToFile) {
    ft::file file("file");

    EXPECT_THROW(file.read(), ft::InvalidFileException);
    EXPECT_THROW(file.write("failing test"), ft::InvalidFileException);

    system("touch file");
    EXPECT_NO_THROW(file.write("testing write to a file\nTesting 123\n"));
    EXPECT_EQ(file.read(), "testing write to a file\nTesting 123\n");
    system("rm file");
}

TEST(testFile, testGetFilename) {
    ASSERT_EQ(ft::file("/dir/path/file.txt").getFilename(), "file.txt");
    ASSERT_EQ(ft::file("/dir/path/file.tar.gz").getFilename(), "file.tar.gz");
    ASSERT_EQ(ft::file("file.tar.gz").getFilename(), "file.tar.gz");
    ASSERT_EQ(ft::file("file").getFilename(), "file");
}