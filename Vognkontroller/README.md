# Vognkontroller


husk:
- -DTarget=Vognkontroller -DPlatform=Raspberry
- sudo apt-get install gtkmm-3.0
- sudo apt update
- sudo apt install libhidapi-hidraw0 libhidapi-libusb0
- Deployment, set root path, and mappings deployment path

og
- /home/pi/.config/autostart/Vognkontroller.desktop
  [Desktop Entry]
  Type=Application
  Name=Vognkontroller
  Exec=sudo /home/pi/Vognkontroller/cmake-build-vognkontroller-raspberry/Vognkontroller

