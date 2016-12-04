# Web  Controlled Media Player

WPlayer is a media player (based on mpv) that can be controlled from a web interface.

## Build

```sh
git clone ...
mkdir wplayer/build && cd wplayer/build
cmake ../
make
make install # as root
```

### Dependencies

##### Fedora
Requires [RPMFusion](https://rpmfusion.org/Configuration)
```
sudo dnf install boost-devel boost-static openssl-devel openssl-static mpv-libs-devel log4cplus-devel uchardet-devel file-devel libcdio-devel libcdio-paranoia-devel alsa-lib-devel mesa-libEGL-devel enca-devel mesa-libgdm-devel jack-audio-connnection-kit-devel ffmpeg-devel libbluray-devel libass-devel mesa-libwayland-egl-devel libguess-devel libvdpau-devel libva-devel libXrandr-devel rubberband-devel libXinerama-devel SDL2-devel pulseaudio-libs-devel libsmbclient-devel libwayland-cursor-devel libv4l-devel libXScrnSaver-devel libXv-devel libxkbcommon-devel
```


## Start

 - Edit the conf file in the config folder
 - `wplayer -c config/wplayer.conf`
 - Visit http://localhost:8080/
