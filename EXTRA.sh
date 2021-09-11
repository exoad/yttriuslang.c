#!/usr/bin/env
#
# v 1.0 Revision version (if any) : 1
#

echo -e "\e[34mChoices:\n 1. Logs \n2. Images\n3. Containers\n4. EXEC into current" ; echo -e "\e[39m" ; echo ;
read VARK;

if [ VARK == "1" ]; then
  sudo docker logs
  exit
elif [ VARK == "2" ]; then
  sudo docker images
  exit
elif [ VARK == "3" ]; then
  sudo docker ps
  exit
elif [ VARK == "4"]; then
  echo -e "\e[32mGet container ID from list: \n" ; echo ;
  sudo docker ps
  read DOCKERID
  docker exec -it $DOCKERID /bin/bash
  exit
endif
