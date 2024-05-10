#!/bin/bash

../bin/tum_mono \
    ../ORB-SLAM3/Vocabulary/ORBvoc.txt \
    ../cfg/ORB_SLAM3/Monocular/TUM/tum_freiburg1_desk.yaml \
    ../cfg/gaussian_mapper/Monocular/TUM/tum_mono.yaml \
    ~/Downloads/datasets/TUM/rgbd_dataset_freiburg2_xyz \
    ../results/tum_mono/rgbd_dataset_freiburg2_xyz \
#    no_viewer
