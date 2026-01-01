#!/bin/bash

show_usage() {
    echo "Usage: ./new_project <project_root> <project_name>"
}

if [ "$#" -ne 2 ]; then
    show_usage
    exit 1
fi

if [ "$1" = "-h" ]; then
    show_usage
    exit 0
fi

cd ~/repos/embedded

echo "Making directory: ./projects/$1/$2"
mkdir -p ./projects/"$1"/"$2"

echo "Copying over template..."
cp -r stm32f401re_boilerplate/* ./projects/"$1"/"$2"

# need to put in the new project boilerplate to gitignore
echo "projects/$1/$2/startup/*" >> .gitignore
echo "projects/$1/$2/system/*" >> .gitignore
echo "projects/$1/$2/linker/*" >> .gitignore
