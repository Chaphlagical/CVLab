#include "fasterrcnn.h"
#include <fstream>

using namespace Chaf;

CFasterRCNN::CFasterRCNN()
{
	Init();
}

void CFasterRCNN::Init()
{
	pb_file = "";
	pbtxt_file = "";

	class_names.clear();
}

void CFasterRCNN::Load_Model()
{
	// Initialize object classes names
	object_name_path = "../data/models/coco.names";
	std::ifstream ifs(object_name_path.c_str());
	std::string line;
	if (ifs.is_open())
	{
		class_names.clear();
		line = "";
		while (std::getline(ifs, line))class_names.push_back(line);
	}


	// load net_work model
	pb_file = "../data/models/fasterrcnn/faster_rcnn_resnet50_coco_2018_01_28/frozen_inference_graph.pb";
	pbtxt_file = "../data/models/fasterrcnn/faster_rcnn_resnet50_coco_2018_01_28/faster_rcnn_resnet50_coco_2018_01_28.pbtxt";

	net_ = cv::dnn::readNetFromTensorflow(pb_file, pbtxt_file);
	net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

void	CFasterRCNN::Load_Image(const std::string img_path)
{
	input_img = cv::imread(img_path);
}

void CFasterRCNN::Detection()
{
	if (input_img.empty())
	{
		std::cout << "No Image" << std::endl;
		return;
	}

	net_.setInput(cv::dnn::blobFromImage(input_img, 1, cv::Size(Width, Height),cv::Scalar(), true, false, CV_8U));

	outs = net_.forward();
	//std::cout<<outs.at<uchar>(0,0,0,0);
	


	//std::cout << v;

}