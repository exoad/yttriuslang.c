#!/usr/bin/env
#
# Run and Daemon Launch Versions
#

# should only be used during original development

echo -e "\e[34mStarting Daemon..." ; echo ;

while :
do	
	node app.js # restart the application
	git pull https://github.com/exoad/yAPI.git # Note the SSH & GPG keys are already set this means if you are going to use this, you must set that manually
  	echo -e "\e[31mPulled new updates..." ; echo -e "\e[0m "; echo ; # reset the colors
	sleep 10
done
