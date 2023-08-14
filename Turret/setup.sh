#!/usr/bin/sh

#cd ~/
#mkdir repos && cd repos

set -e

read -p "Install dependencies? (y:n) " install_packages
read -p "Delete source repos when finished? (y:n) " delete

if [ $install_packages = "y" ]; then
	echo "-------------------------------------------"
	echo "Installing dependencies"
	echo "-------------------------------------------"
	# GUI and dev tools
	sudo apt install libgtk2.0-dev pkg-config
	# Photo/Video processing
	sudo apt install libjpeg-dev libtiff5-dev libjasper-dev
	sudo apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
	sudo apt install libxvidcore-dev libx264-dev
	# Optimization packages
	sudo apt install libatlas-base-dev gfortran
fi

echo "-------------------------------------------"
echo "Fetching repositories"
echo "-------------------------------------------"
WIRING_PI_DIR=$(echo $(find $HOME -type d -name "WiringPi") | cut -d ' ' -f 1)
OPENCV_DIR=$(echo $(find $HOME -type d -name "opencv") | cut -d ' ' -f 1)
OPENCV_CONTRIB_DIR=$(echo $(find $HOME -type d -name "opencv_contrib") | cut -d ' ' -f 1)

if [ ! $WIRING_PI_DIR ]; then
	git clone https://github.com/WiringPi/WiringPi.git
	WIRING_PI_DIR=$PWD
else
	echo "Found WiringPi at $WIRING_PI_DIR"
fi

if [ ! $OPENCV_DIR ]; then
	git clone --branch 3.4 https://github.com/opencv/opencv.git
	OPENCV_DIR=$PWD
else
	echo "Found opencv at $OPENCV_DIR"
fi

if [ ! $OPENCV_CONTRIB_DIR ]; then
	git clone --branch 3.4 https://github.com/opencv/opencv_contrib.git
	OPENCV_CONTRIB_DIR=$PWD
else
	echo "Found opencv_contrib at $OPENCV_CONTRIB_DIR"
fi

#echo "Building WiringPi"
#cd $WIRING_PI_DIR && ./build

echo "Building OpenCV"
cd $OPENCV_DIR/..
if [ ! -d build ]; then
	mkdir build/
fi
cd build
#OpenCL disabled for raspberry pi
CMAKE_FLAGS="-D WITH_OPENCL=OFF \
	-D PYTHON3_EXECUTABLE=/usr/bin/python3 \
	-D PYTHON3_INCLUDE_DIR=/usr/include/python3.9 \
	-DOPENCV_EXTRA_MODULES_PATH=$OPENCV_CONTRIB_DIR/modules"
cmake $CMAKE_FLAGS ../opencv
make -j1
sudo make install
echo "-------------------------------------------"
echo "BUILDS DONE"
echo "-------------------------------------------"
cd ..

if [$delete = "y"]; then
	echo "Removing WiringPi"
	rm -rf $WIRING_PI_DIR
	echo "Removing OpenCV"
	rm -rf $OPENCV_DIR
	rm -rf $OPENCV_DIR/../build/
	rm -rf $OPENCV_CONTRIB_DIR
fi

