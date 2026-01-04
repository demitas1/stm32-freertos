FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    wget \
    curl \
    unzip \
    libusb-1.0-0-dev \
    libudev-dev \
    libncurses6 \
    libpython3-dev \
    python3 \
    python3-pip \
    gdb-multiarch \
    openocd \
    stlink-tools \
    && rm -rf /var/lib/apt/lists/*

# ARM GCC Toolchain
ARG ARM_GCC_VERSION=13.3.rel1
RUN wget -q https://developer.arm.com/-/media/Files/downloads/gnu/${ARM_GCC_VERSION}/binrel/arm-gnu-toolchain-${ARM_GCC_VERSION}-x86_64-arm-none-eabi.tar.xz \
    && tar -xf arm-gnu-toolchain-${ARM_GCC_VERSION}-x86_64-arm-none-eabi.tar.xz -C /opt \
    && rm arm-gnu-toolchain-${ARM_GCC_VERSION}-x86_64-arm-none-eabi.tar.xz

ENV PATH="/opt/arm-gnu-toolchain-${ARM_GCC_VERSION}-x86_64-arm-none-eabi/bin:${PATH}"

WORKDIR /projects

CMD ["/bin/bash"]
