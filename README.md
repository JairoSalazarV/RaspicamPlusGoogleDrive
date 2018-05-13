# RaspicamPlusGoogleDrive

cd Documents

git clone --depth=1 https://github.com/JairoSalazarV/Rasprive

cd /Rasprive/

g++ Rasprive.cpp -o Rasprive

/home/pi/Documents/Rasprive/Dropbox-Uploader/dropbox_uploader.sh

//Get Token from Dropbox App

crontab -e

/Add line
@reboot /home/pi/Documents/Rasprive/autorun.sh
