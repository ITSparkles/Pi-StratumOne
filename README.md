# Pi-StratumOne

Setup for converting a Raspberry Pi into a Stratum 1 Time Server using the HABSupplies GPS HAT board. 


If, like me, you are forever building, tweeking (read breaking) setups, then you will know how annoying it is to sit and retype all 
those commands in again to recreate a service or even a dedicated Raspberry Pi.

This is my attempt to make things easier for myself for my own Stratum One Time Server using GPS PPS as provided by the UBlox chip 
on the HAB Suplies HAT board. If, like me, you are forever building, tweeking (read breaking) setups, then you will know how 
annoying it is to sit and retype all those commands in again to recreate a service or even a dedicated Raspberry Pi.

# Steps to Install
This is my attempt to make things easier for myself for my own Stratum One Time Server using GPS PPS as provided by the UBlox chip 
on the [HAB Supplies GPS HAT Extension Board](https://store.uputronics.com/index.php?route=product/product&path=60_64&product_id=81 
"HAB Supplies") 


## Steps to Install
1. Install a fresh copy of the Raspian Lite operating system onto a SD card. 
2. If you are going to run this Pi headless, make sure to create an empty file in the /boot directory called "ssh" so it can be accessed over the network. 
3. Clone this Git and run the setup command (setup.sh). 
4. Once the script has completed, remember to change the password for the Pi user and reboot.



Credits : all credits are due to HAB Supplies and the very informative blog post [here](https://ava.upuaut.net/?p=951 "UPU Blog")
          And a few other changes to see if I could make this a complete standalone, off net!






