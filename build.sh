sudo docker run --rm -it \
    -v "$PWD:/usr/src/tdesktop" \
    -e CONFIG=Debug \
    tdesktop:centos_env \
    /usr/src/tdesktop/Telegram/build/docker/centos_env/build.sh \
    -D TDESKTOP_API_ID=17349 \
    -D TDESKTOP_API_HASH=344583e45741c457fe1862106095a5eb