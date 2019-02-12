#!/bin/bash

# ##################################################################################
#
# 				  Pi Stratum One
#                                 ==============
#
# A script to configure a Raspberry Pi with the HAB Supplies GPS HAT as a Stratum 1
# GPS disiplined Network Time Server
#
#
#
#
#
#
# #################################################################################

set -eu

source /etc/os-release

now=`date '+%H:%M:%S'`

# First carry out an OS check to ensure we are on the right version

if [ $VERSION_ID -eq 9 ]; then
    echo "$now : Running Raspbian Stretch. Starting Pi-StratumOne setup..."
else
    echo "You seem to be using {$PRETTY_NAME}. This installer is for Raspbian Stretch."
    exit 1
fi

# Disable IPv6
now=`date '+%H:%M:%S'`
echo "$now : Setting up the Pi networking options...."
sudo sed -i.bck '$s/$/ ipv6.disable=1/' /boot/cmdline.txt

# Set the hostname to be ntp-pi
sudo sed -i 's/127.0.1.1\traspberrypi/127.0.1.1\tntp-pi/g' /etc/hosts
sudo sed -i 's/127.0.0.1\tlocalhost/127.0.0.1\tlocalhost ntp-pi/g' /etc/hosts
sudo hostnamectl set-hostname ntp-pi


# Create alaises and shortcuts
now=`date '+%H:%M:%S'`
echo "$now : Creating shell aliases...."
echo "alias ll='ls -alF'" >> ~/.bash_aliases
echo "alias la='ls -A'"   >> ~/.bash_aliases
echo "alias l='ls -CF'"   >> ~/.bash_aliases
echo "$now : .... DONE"
echo " "

# Set timezone info to be Europe/London
now=`date '+%H:%M:%S'`
echo "$now : Applying TimeZone data........"
echo "Europe/London" | sudo tee /etc/timezone
echo "$now : .... DONE"
echo " "

# Configure Serial terminals, I2C and SPI
now=`date '+%H:%M:%S'`
echo "$now : Disabling Serial Terminal and Enabling /dev/serial0 .........."
sudo raspi-config nonint do_serial 2
now=`date '+%H:%M:%S'`
echo "$now : Enabling I2C.........."
sudo raspi-config nonint do_i2c 0
now=`date '+%H:%M:%S'`
echo "$now : Enabling SPI.........."
sudo raspi-config nonint do_spi 0

# Update and upgrade the package list, installed packages and distribution
now=`date '+%H:%M:%S'`
echo "$now : Updating and upgrading your apt packages"
now=`date '+%H:%M:%S'`
echo "$now : ..  Running update"
sudo apt-get -qq update > /dev/null
now=`date '+%H:%M:%S'`
echo "$now :  ... Running upgrade"
sudo apt-get -qqy upgrade > /dev/null
now=`date '+%H:%M:%S'`
echo "$now :  .... Running dist-upgrade"
sudo apt-get -qqy dist-upgrade > /dev/null
now=`date '+%H:%M:%S'`
echo "$now : Update/Upgrade complete."

# Install the required Raspian Packages for the Pi Stratum One
mapfile -t aptpackages < RequiredInstalls.txt

now=`date '+%H:%M:%S'`
echo "$now : Installing required Pi Stratum One packages..."
for package in "${aptpackages[@]}"; do
    now=`date '+%H:%M:%S'`
    echo "$now : $package..."
    sudo apt-get install $package -qqy > /dev/null
done

# Configure GPSD to autostart and run without a user logged in
now=`date '+%H:%M:%S'`
echo "$now : Configuring GPS daemon..."
sudo sed -i 's/USBAUTO="true"/USBAUTO="false"/g' /etc/default/gpsd
sudo sed -i 's:DEVICES="":DEVICES="/dev/serial0 /dev/pps0":g' /etc/default/gpsd
sudo sed -i 's:GPSD_OPTIONS="":GPSD_OPTIONS="-n":g' /etc/default/gpsd
echo KERNEL==\"ttyAMA0\", SUBSYSTEM==\"tty\", DRIVER==\"\", OWNER==\"root\", GROUP==\"tty\", MODE=\"0777\", SYMLINK+=\"gps0\" | sudo tee /etc/udev/rules.d/09-pps.rules
echo "dtoverlay=pps-gpio,gpiopin=18" | sudo tee --append  /boot/config.txt
echo "pps-gpio" | sudo tee --append /etc/modules

# Configure ssh authorised keys
now=`date '+%H:%M:%S'`
echo "$now :  Configuring SSH authorised keys ........."
mkdir -p ~/.ssh
touch ~/.ssh/authorized_keys
SSHKEYS="./sshkeys"
if [ -f "$SSHKEYS" ]; then {
	echo "$now : ....Found SSHKEYS.  Adding to authorized_keys"
	echo <./sshkeys >> ~/.ssh/authorized_keys
	chmod -R 700 ~/.ssh
	}
else {
	echo "$now : ....No SSHKEYS file found."
	}

# Configure NTP
now=`date '+%H:%M:%S'`
echo "$now :  Configuring NTP daemon  ........."
sudo systemctl stop ntp
sudo cp ./ntp.conf /etc/
sudo systemctl start ntp

# Set the u-Blox chip to be a stationary device
now=`date '+%H:%M:%S'`
echo "$now : Compiling gpsControl....."
cd ./gpscontrol
gcc -o gpscontrol gpscontrol.c >/dev/null
# We need to now issue the following command until we get confirmation

# sudo ./gpsControl -p -d /dev/ttyAMA0 = Portable Mode
# sudo ./gpsControl -s -d /dev/ttyAMA0 = Stationary Mode
sudo ./gpsControl -s -d /dev/ttyAMA0



# All done.  Exit and ask user to reboot.
now=`date '+%H:%M:%S'`
echo "$now : All done.  Please reboot..........>"










