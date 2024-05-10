#!/bin/bash

../bin/tum_mono \
    ../ORB-SLAM3/Vocabulary/ORBvoc.txt \
    ../cfg/ORB_SLAM3/Monocular/TUM/my_desk.yaml \
    ../cfg/gaussian_mapper/Monocular/TUM/tum_mono.yaml \
    ~/Downloads/datasets/record_data2_tum_format/data \
    ../results/tum_mono/my_desk \
#    no_viewer
