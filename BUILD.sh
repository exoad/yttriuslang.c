#!/usr/bin/env
#
# Docker Build script
#

cd ~/.yAPI

sudo docker build -t yapi .

echo -e "\e[31mFinished building " ; echo -e "\e[34mRunning the Docker... " ; echo ;

sudo docker run -d yapi ; echo ;

docker ps
