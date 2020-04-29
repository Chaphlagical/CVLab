# CVLab

An app based on OpenCV and dear-imgui for computer vision demo of OpenCV

## Requirements

* C++ 11
* CMake 3.16.3  
* ImGUI
* OpenCV 4.2.0
* GLFW

## Feature


- [x] Yolov3 object detection for images
- [ ] Yolov3 object detection for videos
- [ ] Yolov3 object detection for camera flow
- [x] Mask RCNN for images
- [x] Open Pose for single human pose estimation
- [ ] Open Pose for multi-human pose estimation

## Introduction

### Download Models

#### yolov3

[coco.names](https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names)

[yolov3.cfg](https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg?raw=true)

[yolov3.weights](https://pjreddie.com/media/files/yolov3.weights)

Place them in  `./data/models/yolov3/`

### Object Detection

YoloV3

![yolo](http://home.ustc.edu.cn/~chaf/image/yolo.png)

### Object Segmentation

Mask-RCNN

![image-20200423225346132](http://home.ustc.edu.cn/~chaf/image/mask.png)

### Pose Estimation

![image-20200429134717224](http://home.ustc.edu.cn/~chaf/image/pose.png)