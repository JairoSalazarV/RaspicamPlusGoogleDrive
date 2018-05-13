# RaspicamPlusGoogleDrive

//Set WiFi

//Set User Password, Camera, SSH, Hostname

cd Documents

mkdir RASPRIVE

git clone --depth=1 https://github.com/JairoSalazarV/Rasprive

cd /Rasprive/

g++ Rasprive.cpp -o Rasprive

//Get Token from Dropbox App and Run
/home/pi/Documents/Rasprive/Dropbox-Uploader/dropbox_uploader.sh  

crontab -e 

//Add line
@reboot /home/pi/Documents/Rasprive/autorun.sh

