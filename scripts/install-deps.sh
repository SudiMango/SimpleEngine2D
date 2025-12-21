#!/bin/bash

if [ ! -f "deps.txt" ]; then
    echo "Error: deps.txt not found in the current directory."
    exit 1
fi

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    echo "Error: /etc/os-release not found. Cannot detect OS."
    exit 1
fi

get_deps() {
    grep "^$1:" deps.txt | cut -d':' -f2
}

case "$OS" in
    fedora)
        echo "Detected Fedora. Reading dependencies..."
        PACKAGES=$(get_deps "fedora")
        
        if [ -z "$PACKAGES" ]; then
            echo "Error: No packages found for Fedora in deps.txt."
            exit 1
        fi

        sudo dnf install -y $PACKAGES
        ;;
    *)
        echo "Error: OS '$OS' is not supported. Please install deps manually."
        exit 1
        ;;
esac

echo "Installation process for $OS completed."