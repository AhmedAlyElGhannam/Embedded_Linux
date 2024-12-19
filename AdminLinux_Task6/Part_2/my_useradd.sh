#!/bin/bash

# force user to run the script as root or with sudo privileges
if [ ! "`whoami`" = "root" ] 
then
    echo 
    echo "Please run the script as root or using sudo."
    exit 1
fi

# take username
read -p 'Username: ' USERNAME
echo 

# take password
while true; do
    read -sp 'Password: ' PASSWORD
    echo
    read -sp 'Re-enter Password: ' RPASSWORD
    echo 
    if [ "$PASSWORD" == "$RPASSWORD" ]; then
        break
    else
        echo "WRONG PASSWORD!"
        echo
    fi
done

# Hash the password using OpenSSL
PASSWORD_HASH=$(openssl passwd -6 "$PASSWORD")

# making user home dir and copying config files from /etc/skel
sudo mkdir -p /home/${USERNAME}
HOMEDIR="/home/${USERNAME}"

# copying configuration files to user's home dir
sudo cp -r /etc/skel/. ${HOMEDIR}

# setting default shell
DEF_SHELL="/bin/bash"

# using useradd to create new user with provided info
useradd -d ${HOMEDIR} -s ${DEF_SHELL} -p ${PASSWORD_HASH} ${USERNAME}

# IMPORTANT: CHANGE HOME DIRECTORY OWNERSHIP TO NEW USER
chown -R ${USERNAME}:${USERNAME} ${HOMEDIR}

echo "User ${USERNAME} has been created successfully."

