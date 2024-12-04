#include <gtest/gtest.h>
#include "RecommendCommand.h"
#include <fstream>

class RecommendCommandTest : public ::testing::Test {
protected:
    std::string testFilePath = "test_database.txt";
    std::string EmptyFilePath = "empty.txt";
    RecommendCommand* recommendCommand;

    void SetUp() override {
        std::ofstream testFile(testFilePath);
        ASSERT_TRUE(testFile.is_open());
        testFile << "1 100 101 102 103\n";
        testFile << "2 101 102 104 105 106\n";
        testFile << "3 100 104 105 107 108\n";
        testFile << "4 101 105 106 107 109 110\n";
        testFile << "5 100 102 103 105 108 111\n";
        testFile << "6 100 103 104 110 111 112 113\n";
        testFile << "7 102 105 106 107 108 109 110\n";
        testFile << "8 101 104 105 106 109 111 114\n";
        testFile << "9 100 103 105 107 112 113 115\n";
        testFile << "10 100 102 105 106 107 109 110 116\n";

        testFile.close();
        FileDataBase* data=new FileDataBase();
        data->setfilepath(testFilePath);
        DataBaseServices* dbservices = new DataBaseServices(data);
        recommendCommand = new RecommendCommand(data,dbservices);
    }
    void SetUp2()  {
        std::ofstream testFile(testFilePath);
        ASSERT_TRUE(testFile.is_open());
        testFile.close();

        FileDataBase* data=new FileDataBase();
        data->setfilepath(EmptyFilePath);
        DataBaseServices* dbservices = new DataBaseServices(data);
        recommendCommand = new RecommendCommand(data,dbservices);
    }

    void TearDown() override {
        delete recommendCommand;
        std::remove(testFilePath.c_str());
    }
};



TEST_F(RecommendCommandTest, RecommendForValidUserAndMovie) {
    auto recommendations = recommendCommand->Recommend(1,104);
    std::vector<int> expected = {105, 106,111,110,112,113,107,108,109,114};

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(recommendations[i], expected[i]);
    }
}
TEST_F(RecommendCommandTest, RecommendForValidUserAndMovie2) {
    auto recommendations = recommendCommand->Recommend(1,105);
    std::vector<int> expected = {106,107 ,108 ,109 ,104, 110, 111, 112, 113, 115, 116 ,114 };

    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(recommendations[i], expected[i]);
    }
}



TEST_F(RecommendCommandTest, RecommendForUserDoesntExist) {

    EXPECT_FALSE(recommendCommand->checkInput("15", "104"));
}

TEST_F(RecommendCommandTest, RecommendForMovieDoesntExist) {

    EXPECT_FALSE(recommendCommand->checkInput("2", "120"));
}

TEST_F(RecommendCommandTest, EmptyDataBase) {
    SetUp2();
    EXPECT_FALSE(recommendCommand->checkInput("1", "103"));
}
