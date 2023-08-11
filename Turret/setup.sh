#!/usr/bin/sh

#cd ~/
#mkdir repos && cd repos

read -p "Delete source repos when finished? (y:n) " delete

echo "Fetching repositories"
echo "-------------------------------------------"
if [ ! -d WiringPi ]; then
git clone https://github.com/WiringPi/WiringPi.git
fi
if [ ! -d opencv ]; then
git clone --branch 3.4 https://github.com/opencv/opencv.git
fi
if [ ! -d opencv_contrib]; then
git clone --branch 3.4 https://github.com/opencv/opencv_contrib.git
fi

mkdir build_cv
echo "Building WiringPi"
cd WiringPi/ && ./build
cd ../build_cv
echo "Building OpenCV"
#OpenCL disabled for raspberry pi
CMAKE_FLAGS = "-D WITH_OPENCL=OFF -D PYTHON3_EXECUTABLE=/usr/bin/python3 -D PYTHON3_INCLUDE_DIR=/usr/include/python3.9 -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules"
cmake $CMAKE_FLAGS ../opencv
make -j1
make install
echo "-------------------------------------------"
echo "BUILDS DONE"
cd ..

if [$delete == "y"]; then
echo "Removing WiringPi"
rm -rf WiringPi
echo "Removing OpenCV"
rm -rf opencv
rm -rf opencv_contrib
rm -rf build_cv
fi

