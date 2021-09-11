

LWS_VERSION=3.1.0













if [ $(id -u) = 0 ]; then
    echo -n "Warning: Running script as root/superuser. Continue? (Y/N): "
    read yesno

    if [ "$yesno" = "Y" ] || [ "$yesno" = "y" ]; then
        echo -n
    else
        exit 1;
    fi
fi

sudo apt update && sudo apt upgrade
sudo apt install checkinstall libmicrohttpd-dev libjansson-dev libcurl4-openssl-dev\
                 git \
                 build-essential make cmake gcc\
                 zlib1g-dev libssl-dev libuv1-dev































echo "Installing libwebsockets"
git clone https://github.com/warmcat/libwebsockets
cd libwebsockets
    git checkout v$(LWS_VERSION)
    mkdir build
    cd build
        cmake .. -DCMAKE_BUILD_TYPE=Debug -DLWS_WITH_LIBUV=ON -DLWS_WITH_LATENCY=ON -DLWS_WITH_IPV6=ON -DLWS_WITH_PLUGINS=ON
        make
        sudo checkinstall --pkgname libwebsockets-dev --pkgversion "$(LWS_VERSION)"
        sudo ldconfig
    cd ..
cd ..



git clone https://github.com/dxing97/libdiscord.git
cd libdiscord
    mkdir build && cd build
        cmake ..
        make
        echo "finished installing dependencies and compiled libdiscord and test apps"







