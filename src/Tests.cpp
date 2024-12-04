#include "gtest/gtest.h"
#include "FileDataBase.h"
#include <fstream>

// Helper function to create a temporary test file with specified content
void createTestFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        throw std::runtime_error("Unable to create test file");
    }
}

// Test fixture for FileDataBase tests
class FileDataBaseTest : public ::testing::Test {
protected:
    FileDataBase db;               // Database instance for testing
    std::string testFile = "test_database.txt";  // Temporary file for testing

    // Set up test environment
    void SetUp() override {
        db.setfilepath(testFile);  // Set file path in the database
        std::string fileContent = 
            "1 100 101 102 103\n"
            "2 101 102 104 105 106\n"
            "3 100 104 105 107 108\n";
        createTestFile(testFile, fileContent); // Create test file with sample data
    }

    // Clean up after each test
    void TearDown() override {
        std::remove(testFile.c_str()); // Remove test file
    }
};

// Test UpdateLocalDataBase for valid file processing
TEST_F(FileDataBaseTest, UpdateLocalDatabase_ValidFile) {
    ASSERT_NO_THROW(db.UpdateLocalDataBase()); // Ensure no exceptions

    auto users = db.getUsers();
    ASSERT_EQ(users.size(), 3); // Validate number of users

    // Verify user 1
    auto user1It = std::find_if(users.begin(), users.end(),
                                [](const User& user) {
                                    return user.getUserID() == 1;
                                });
    ASSERT_NE(user1It, users.end());
    EXPECT_EQ(user1It->getUserID(), 1);
    EXPECT_EQ(user1It->getMovies().size(), 4); // Validate movies count
    EXPECT_EQ(user1It->getMovies()[0].getMovieID(), 100); // Validate movie ID
}

// Test adding a new user to file and local database
TEST_F(FileDataBaseTest, AddUser_AddsToFileAndLocalDB) {
    User newUser;
    newUser.setUserID(4); // Define a new user
    std::vector<Movie> newMovies = { Movie(200), Movie(201) };
    newUser.setMovies(newMovies);

    ASSERT_NO_THROW(db.addNewUser(newUser)); // Add user

    // Verify local database
    auto users = db.getUsers();
    auto it = std::find_if(users.begin(), users.end(),
                           [&newUser](const User& user) {
                               return user.getUserID() == newUser.getUserID();
                           });
    ASSERT_NE(it, users.end());
    EXPECT_EQ(it->getMovies().size(), 2); // Validate movies added
    EXPECT_EQ(it->getMovies()[0].getMovieID(), 200);
    EXPECT_EQ(it->getMovies()[1].getMovieID(), 201);

    // Verify file content
    std::ifstream file(testFile);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;

    // Check if new user data is written to file
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        int userID;
        lineStream >> userID;

        if (userID == 4) {
            std::vector<int> movieIDs;
            int movieID;
            while (lineStream >> movieID) {
                movieIDs.push_back(movieID);
            }
            EXPECT_EQ(movieIDs.size(), 2);
            EXPECT_EQ(movieIDs[0], 200);
            EXPECT_EQ(movieIDs[1], 201);
            found = true;
            break;
        }
    }
    file.close();

    EXPECT_TRUE(found); // Ensure new user data was found
}

// Test adding an existing user throws an exception and makes no changes
TEST_F(FileDataBaseTest, AddUser_ExistingUser_ThrowsAndNoChange) {
    User existingUser;
    existingUser.setUserID(1); // Existing user ID
    std::vector<Movie> newMovies = { Movie(300) };
    existingUser.setMovies(newMovies);

    EXPECT_THROW(db.addNewUser(existingUser), std::runtime_error); // Should throw

    // Verify no changes in local database
    auto users = db.getUsers();
    EXPECT_EQ(users.size(), 3); // No new user added

    // Verify no changes in file
    std::ifstream file(testFile);
    ASSERT_TRUE(file.is_open());

    std::string fileContent;
    std::ostringstream buffer;
    buffer << file.rdbuf();
    fileContent = buffer.str();
    file.close();

    std::string originalFileContent = 
        "1 100 101 102 103\n"
        "2 101 102 104 105 106\n"
        "3 100 104 105 107 108\n";

    EXPECT_EQ(fileContent, originalFileContent); // File should remain unchanged
}

// Test adding new movies to an existing user
TEST_F(FileDataBaseTest, AddMovies_AddsToFileAndLocalDB) {
    std::vector<Movie> newMovies = { Movie(109), Movie(110), Movie(111) }; // New movies
    int userID = 1; // Existing user ID

    ASSERT_NO_THROW(db.addNewMovies(userID, newMovies)); // Add movies

    // Verify file content
    std::ifstream file(testFile);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        int id;
        lineStream >> id;

        if (id == userID) {
            std::vector<int> movieIDs;
            int movieID;
            while (lineStream >> movieID) {
                movieIDs.push_back(movieID);
            }
            EXPECT_GE(movieIDs.size(), 7); // Verify all movies
            EXPECT_EQ(movieIDs[movieIDs.size() - 3], 109);
            EXPECT_EQ(movieIDs[movieIDs.size() - 2], 110);
            EXPECT_EQ(movieIDs[movieIDs.size() - 1], 111);
            found = true;
            break;
        }
    }
    file.close();

    EXPECT_TRUE(found); // Ensure changes were found

    // Verify changes in local database
    auto users = db.getUsers();
    auto userIt = std::find_if(users.begin(), users.end(),
                               [](const User& user) {
                                   return user.getUserID() == 1;
                               });
    ASSERT_NE(userIt, users.end());
    const auto& userMovies = userIt->getMovies();
    EXPECT_GE(userMovies.size(), 7); // Validate movies count
    EXPECT_EQ(userMovies[userMovies.size() - 3].getMovieID(), 109);
    EXPECT_EQ(userMovies[userMovies.size() - 2].getMovieID(), 110);
    EXPECT_EQ(userMovies[userMovies.size() - 1].getMovieID(), 111);
}

// Test adding movies to a non-existent user throws an exception
TEST_F(FileDataBaseTest, AddMovies_InvalidUser_Throws) {
    std::vector<Movie> newMovies = { Movie(112), Movie(113) }; // New movies
    int nonExistentUserID = 999; // Non-existent user ID

    EXPECT_THROW(db.addNewMovies(nonExistentUserID, newMovies), std::runtime_error); // Should throw
}

// Test UpdateLocalDataBase correctly populates the database
TEST_F(FileDataBaseTest, UpdateLocalDataBase_PopulatesUsersMap) {
    std::string fileContent = 
        "1 100 101 102 103\n"
        "2 101 102 104 105 106\n"
        "3 100 104 105 107 108\n";
    createTestFile(testFile, fileContent); // Prepare file

    ASSERT_NO_THROW(db.UpdateLocalDataBase()); // Update local database

    auto users = db.getUsers();
    ASSERT_EQ(users.size(), 3); // Validate number of users

    // Verify user 1
    auto user1It = std::find_if(users.begin(), users.end(),
                                 [](const User& user) {
                                     return user.getUserID() == 1;  // Compare user ID directly
                                 });
    ASSERT_NE(user1It, users.end());
    const std::vector<Movie>& user1Movies = user1It->getMovies();
    EXPECT_EQ(user1Movies.size(), 4);
    EXPECT_EQ(user1Movies[0].getMovieID(), 100);
    EXPECT_EQ(user1Movies[1].getMovieID(), 101);
    EXPECT_EQ(user1Movies[2].getMovieID(), 102);
    EXPECT_EQ(user1Movies[3].getMovieID(), 103);

    // Verify user 2
    auto user2It = std::find_if(users.begin(), users.end(),
                                 [](const User& user) {
                                     return user.getUserID() == 2;  // Compare user ID directly
                                 });
    ASSERT_NE(user2It, users.end());
    const std::vector<Movie>& user2Movies = user2It->getMovies();
    EXPECT_EQ(user2Movies.size(), 5);
    EXPECT_EQ(user2Movies[0].getMovieID(), 101);
    EXPECT_EQ(user2Movies[1].getMovieID(), 102);
    EXPECT_EQ(user2Movies[2].getMovieID(), 104);
    EXPECT_EQ(user2Movies[3].getMovieID(), 105);
    EXPECT_EQ(user2Movies[4].getMovieID(), 106);

    // Verify user 3
    auto user3It = std::find_if(users.begin(), users.end(),
                                 [](const User& user) {
                                     return user.getUserID() == 3;  // Compare user ID directly
                                 });
    ASSERT_NE(user3It, users.end());
    const std::vector<Movie>& user3Movies = user3It->getMovies();
    EXPECT_EQ(user3Movies.size(), 5);
    EXPECT_EQ(user3Movies[0].getMovieID(), 100);
    EXPECT_EQ(user3Movies[1].getMovieID(), 104);
    EXPECT_EQ(user3Movies[2].getMovieID(), 105);
    EXPECT_EQ(user3Movies[3].getMovieID(), 107);
    EXPECT_EQ(user3Movies[4].getMovieID(), 108);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
