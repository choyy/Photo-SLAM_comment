#!/bin/bash

./bin/tum_mono \
    ./ORB-SLAM3/Vocabulary/ORBvoc.txt \
    ./cfg/ORB_SLAM3/Monocular/TUM/usb_cam.yaml \
    ./cfg/gaussian_mapper/Monocular/TUM/tum_mono.yaml \
    ~/Downloads/datasets/TUM/endos_pigbone_dataset\
    ./results/tum_mono/endos_pigbone \
#    no_viewer
