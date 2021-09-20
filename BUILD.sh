#!/usr/bin/env
#
# Docker Build script
#
# STATE : [] [BO]
#

echo -e "\e[34mRunning Script..." ; echo ;

sudo apt install apt-transport-https ca-certificates curl gnupg-agent software-properties-common

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

sudo apt update

sudo apt install docker-ce docker-ce-cli containerd.io

echo ; echo -e "\e[32mDocker in check... " ; echo ;

sudo docker build -t yapi .

echo -e "\e[31mFinished building " ; echo -e "\e[34mRunning the Docker... " ; echo ;

sudo docker run -d yapi ; echo ;

docker ps
