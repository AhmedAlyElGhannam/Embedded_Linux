#!/bin/bash

LOCAL=$(whoami)
HELLO=$(hostname)
BASH_RC_FILE="${HOME}/.bashrc"

if [ -e "$BASH_RC_FILE" ];
then
	echo "LOCAL=\$(whoami)" >> "$BASH_RC_FILE"
	echo "export HELLO=\$(hostname)" >> "$BASH_RC_FILE"
fi

kitty
