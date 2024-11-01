# Use an official C++ image as the base
FROM ubuntu:22.04

# Set non-interactive frontend to suppress prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
RUN apt-get update && \
    apt-get install -y \
    cmake \
    g++ \
    libssl-dev \
    zlib1g-dev \
    libsqlite3-dev \
    git \
    wget \
    make \
    && rm -rf /var/lib/apt/lists/*

# Create a working directory
WORKDIR /app

# Copy the project files
COPY . .

# Run CMake
RUN cmake -S . -B build

RUN cmake --build build --target TRLBot -j 12

# Set the entry point to execute the bot
CMD ["./build/TRLBot"]

