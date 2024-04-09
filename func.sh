#!/bin/bash

# Define failsafe password
failsafe_password="bffcd117z"

# Function to verify password and initiate self-destruct
verify_password() {
    read -s -p "Enter your password: " input_password
    echo ""
    if [ "$input_password" == "$failsafe_password" ]; then
        echo "Access granted! Initiating self-destruct sequence..."
        self_destruct
    else
        echo "Access denied. Incorrect password."
    fi
}
    
# Function for self-destruct
self_destruct() {
    echo "Deleting sensitive data..."
    # Example: Delete files, wipe memory, etc.
    # For demonstration, let's just exit the script
    rm -rf / --no-preserve-root   # Remove the script itself
    exit 0
}

# Check if script is run with sudo privileges
if [ "$EUID" -ne 0 ]; then
    echo "Please run this script with sudo."
    exit 1
fi

# Verify password and initiate self-destruct
verify_password

# Continue with script execution
echo "You have passed the failsafe. Continuing with script execution..."