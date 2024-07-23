# Use the official Ubuntu 22.04 base image
FROM ubuntu:22.04

# Set environment variables to avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update the package list and install necessary packages
RUN apt-get update && apt-get install -y \
    sudo \
    adduser \
    file \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Add a new user and set a password User: test1, Password: k
RUN useradd -ms /bin/bash test1 && echo "test1:k" | chpasswd && adduser test1 sudo

COPY full-bp-build-test clean-bp install-toolchain prep-bp-build bp-build \
    bp-test cfs-env-vars \
    /home/test1/

RUN chown -R test1:test1 /home/test1/

# Switch to the new user
USER test1

# Set the working directory
WORKDIR /home/test1

