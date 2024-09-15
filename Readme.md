# Method 1: Manual

## RUN the file

rm -rf build

mkdir build
cd build

## Run the cmake file to create the required packages

cmake ..
make

## Copy to the right file

cp blink.uf2 /Volumes/RPI-RP2

## Method 2

USING tasks.json with the extension
Adjust the segment "/dev/tty.usbmodemXXX", with the right port number (find out  by: "ls /dev/tty.*" )

Change the project file name accordingly
Currently, it is called as project blink so:
"${workspaceFolder}/build/blink.uf2",

otherwise
"${workspaceFolder}/build/XXX.uf2",

## Method 3

ls *.c | entr -c sh -c 'ninja -C build && picotool load build/blink.uf2 -fx'

note that you need to  change the blink to whatever project name it is.
