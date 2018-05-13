g++ Rasprive.cpp -o Rasprive

sudo cp RaspriveAutorun /etc/init.d/Rasprive

sudo chmod 755 /etc/init.d/Rasprive

sudo update-rc.d Rasprive defaults

sudo /etc/init.d/Rasprive start
