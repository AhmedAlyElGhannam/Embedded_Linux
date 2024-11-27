#!/bin/bash

# create a directory called myDirectory in user home folder
cd ~
mkdir myDirectory

# inside myDirectory, create another directory named secondDirectory
cd myDirectory
mkdir secondDirectory

# within secondDirectory, create a file named myNotePaper
cd secondDirectory
touch myNotePaper

# copy myNotePaper from secondDirectory to myDirectory
cp myNotePaper ~/myDirectory

# rename the copied file in myDirectory to myOldNotePaper
mv ~/myDirectory/myNotePaper ~/myDirectory/myOldNotePaper

