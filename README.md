# Web  Controlled Media Player

WPlayer is a media player (based on mpv) that can be controlled from a web interface.

To build:

- git clone ....
- cd wplayer
- mkdir build
- cmake ../
- make
- make install (as root)


To start:

 - Edit the conf file in the config folder
 - wplayer -c config/wplayer.conf
 - Visit http://localhost:8080/
