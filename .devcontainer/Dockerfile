FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.30.6"

COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

RUN apt-get update && apt-get install -y \
    autoconf automake libtool pkg-config m4 \
    autoconf-archive build-essential

RUN git config --global --add safe.directory /usr/local/vcpkg \
    && cd /usr/local/vcpkg \
    && git fetch --unshallow --all --tags \
    && ./bootstrap-vcpkg.sh
