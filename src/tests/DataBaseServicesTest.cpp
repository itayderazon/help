#include <gtest/gtest.h>
#include "DataBaseServices.h"
#include <fstream>
#include "FileDataBase.h"

class DataBaseServicesTest : public ::testing::Test {
protected:
    std::string testFilePath = "test_database.txt";
    std::string emptyFilePath = "empty_database.txt";
    DataBaseServices* dbServices;
    FileDataBase* fileDb;

    void SetUp() override {
        // Create test database file with user and movie data
        std::ofstream testFile(testFilePath);
        ASSERT_TRUE(testFile.is_open());
        testFile << "1 100 101 102\n";
        testFile << "2 103 104 105\n";
        testFile << "3 106 107 108\n";
        testFile.close();

        // Initialize FileDataBase and DataBaseServices
        fileDb = new FileDataBase();
        fileDb->setfilepath(testFilePath);
        dbServices = new DataBaseServices(fileDb);
        fileDb->UpdateLocalDataBase();
    }

    void SetUpEmpty() {
        // Create an empty test database
        std::ofstream emptyFile(emptyFilePath);
        ASSERT_TRUE(emptyFile.is_open());
        emptyFile.close();

        // Initialize FileDataBase and DataBaseServices
        fileDb = new FileDataBase();
        fileDb->setfilepath(emptyFilePath);
        dbServices = new DataBaseServices(fileDb);
    }

    void TearDown() override {
        delete dbServices;
        delete fileDb;
        std::remove(testFilePath.c_str());
        std::remove(emptyFilePath.c_str());
    }
};

TEST_F(DataBaseServicesTest, GetUserByID_UserExists) {
    SetUp();
    User* user = dbServices->getUserByID(1);
    ASSERT_NE(user, nullptr);
    EXPECT_EQ(user->getUserID(), 1);
}

TEST_F(DataBaseServicesTest, GetUserByID_UserDoesNotExist) {
    SetUp();
    User* user = dbServices->getUserByID(10); // ID not in database
    EXPECT_EQ(user, nullptr);
}

TEST_F(DataBaseServicesTest, HasWatchedMovie_ValidUserAndMovie) {
    SetUp();
    EXPECT_TRUE(dbServices->hasWatchedMovie(1, 101)); // User 1 watched Movie 101
}

TEST_F(DataBaseServicesTest, HasWatchedMovie_InvalidMovie) {
    SetUp();
    EXPECT_FALSE(dbServices->hasWatchedMovie(1, 109)); // Movie not watched by User 1
}

TEST_F(DataBaseServicesTest, MovieExists_ValidMovie) {
    SetUp();
    EXPECT_TRUE(dbServices->movieExists(102)); // Movie 102 exists
}

TEST_F(DataBaseServicesTest, MovieExists_InvalidMovie) {
    SetUp();
    EXPECT_FALSE(dbServices->movieExists(999)); // Movie does not exist
}

TEST_F(DataBaseServicesTest, EmptyDatabase) {
    SetUpEmpty();
    EXPECT_EQ(dbServices->getUserByID(1), nullptr);
    EXPECT_FALSE(dbServices->hasWatchedMovie(1, 101));
    EXPECT_FALSE(dbServices->movieExists(101));
}
