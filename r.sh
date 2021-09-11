#!/usr/bin/env
#
# Run and Daemon Launch Versions
#


echo -e "\e[34mStarting Daemon..." ; echo ;

while :
do	
	node app.js
	git pull https://github.com/exoad/yAPI # Note the SSH & GPG keys are already set this means if you are going to use this, you must set that manually
  	echo -e "\e[31mPulled new updates..." ; echo ;
	sleep 5
done
