#!/bin/bash

rm ceam-keyring.gpg
wget https://repo.poltavski.ru/rpi/pubkey.asc -O pubkey.asc
gpg -o ceam-keyring.gpg --dearmor pubkey.asc
rm pubkey.asc
sudo cp ceam-keyring.gpg /usr/share/keyrings/
sudo sh -c "echo 'deb [signed-by=/usr/share/keyrings/ceam-keyring.gpg] https://repo.poltavski.ru/rpi bookworm contrib' > /etc/apt/sources.list.d/ceam.list" 
rm ceam-keyring.gpg
