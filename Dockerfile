# Use an official Ubuntu base image
FROM ubuntu:20.04

# Set environment variable to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package lists and install dependencies:
# - build-essential installs gcc, g++, and make (which meets the required versions)
# - cmake (version in Ubuntu 20.04 is typically 3.16 or above)
# - libsdl2-dev provides SDL2 development libraries (version 2.0 or above)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        libsdl2-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /workspace

# (Optional) Copy your project files into the container if you want them baked in:
# COPY . /workspace

# Default command: launch a shell so you can work interactively
CMD ["/bin/bash"]
