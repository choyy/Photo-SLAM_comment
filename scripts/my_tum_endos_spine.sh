#!/bin/bash

./bin/tum_mono \
    ./ORB-SLAM3/Vocabulary/ORBvoc.txt \
    ./cfg/ORB_SLAM3/Monocular/TUM/endos_spine.yaml \
    ./cfg/gaussian_mapper/Monocular/TUM/tum_mono.yaml \
    ~/Downloads/datasets/TUM/endos_spine_dataset3\
    ./results/tum_mono/endos_spine \
#    no_viewer
