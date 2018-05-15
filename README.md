# RaspicamPlusGoogleDrive

//Set WiFi

//Set User Password, Wifi Country, Timezone, Hostname, Camera, SSH

cd Documents

mkdir RASPRIVE

cd RASPRIVE

nano test.Rasprive //Agrega algo de contenido

cd ..

git clone --depth=1 https://github.com/JairoSalazarV/Rasprive

cd /Rasprive/

chmod a+x autorun.sh

g++ Rasprive.cpp -o Rasprive

//Check that Dropbox-Uploader is working

/home/pi/Documents/Rasprive/Dropbox-Uploader/dropbox_uploader.sh upload /home/pi/Documents/RASPRIVE/test.Rasprive /IRR_CG_0002/test.Rasprive

//Get Token from https://www.dropbox.com/developers/apps

//Once Dropbox-Uploader is working, check that Rasprive is working

/home/pi/Documents/Rasprive/autorun.sh
 
//Once Rasprive is working, add a deamon

crontab -e 

//Add line

@reboot /home/pi/Documents/Rasprive/autorun.sh

//Reboot and check that RASPRIVE (locally) and Dropbox.com/FOLDER is currently synchronizing

//Once everythink is Ok, clear folders and FINISH, next reboot will start to updating

