FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.30.6"

COPY ./reinstall-cmake.sh /tmp/
COPY ./.bash_aliases /tmp/.bash_aliases

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

RUN mv /tmp/.bash_aliases /root/.bash_aliases

RUN apt-get update && apt-get install -y \
    autoconf automake libtool pkg-config m4 \
    autoconf-archive build-essential \
    zip curl  wget locales

# Set up fzf-git
RUN locale-gen en_US.UTF-8

# Install Oh My Posh
RUN curl -s https://ohmyposh.dev/install.sh | bash -s

RUN git config --global --add safe.directory /usr/local/vcpkg \
    && cd /usr/local/vcpkg \
    && git fetch --unshallow --all --tags \
    && ./bootstrap-vcpkg.sh
