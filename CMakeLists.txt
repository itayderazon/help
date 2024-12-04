cmake_minimum_required(VERSION 3.10)

project(App)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add the main executable
add_executable(App
    src/App.cpp
    src/FileDataBase.cpp
    src/DataBaseServices.cpp
    src/RecommendCommand.cpp
    src/Input.cpp
    src/Output.cpp
    src/Movie.cpp
    src/User.cpp
    src/ParseInput.cpp
    src/main.cpp
    src/Add.cpp
    # Add other source files as needed
)