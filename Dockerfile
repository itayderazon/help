# Use an official image with CMake and GCC pre-installed
FROM ubuntu:20.04

# Set environment variables to prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire project into the container
RUN mkdir -p /app/data

COPY . /app

# Set the working directory to the project root
WORKDIR /app

# Create a build directory, configure, and build the project
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    cmake --build .
    
VOLUME /app/data

# Set the default command to run the application
CMD ["./build/App"]