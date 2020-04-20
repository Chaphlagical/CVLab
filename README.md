# Photo-Wake-Up

Photo Wake Up Project: [https://grail.cs.washington.edu/projects/wakeup/](https://grail.cs.washington.edu/projects/wakeup/)

Original HMR (End-to-end Recovery of Human Shape and Pose) GitHub: [https://github.com/akanazawa/hmr](https://github.com/akanazawa/hmr) Angjoo Kanazawa, Michael J. Black, David W. Jacobs, Jitendra Malik CVPR 2018

Paper [link](http://openaccess.thecvf.com/content_CVPR_2019/papers/Weng_Photo_Wake-Up_3D_Character_Animation_From_a_Single_Photo_CVPR_2019_paper.pdf)

## Requirements

* C++ 11
* CMake 3.16.3  
* ImGUI
* OpenCV 4.2.0
* GLFW

## Feature


- [ ] 

- [x] Yolov3 object detection for images
- [ ] Yolov3 object detection for videos
- [ ] Yolov3 object detection for camera flow
- [ ] Mask RCNN for images
- [ ] Open Pose for human pose estimation

## Introduction

### Download Models

#### yolov3

[coco.names](https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names)

[yolov3.cfg](https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg?raw=true)

[yolov3.weights](https://pjreddie.com/media/files/yolov3.weights)

Place them in  `./data/models/yolov3/`

### Object Detection

Using OpenCV + Yolo detect people

![image-20200419233642340](README.assets/image-20200419233642340.png)