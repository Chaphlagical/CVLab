#include "yolo.h"
#include <fstream>

using namespace Chaf;

CYolo::CYolo()
{
	 Init();
}

void CYolo::Load_Model()
{
	// Initialize object classes names
	object_name_path = "../data/models/yolov3/coco.names";
	std::ifstream ifs(object_name_path.c_str());
	std::string line;
	if (ifs.is_open())
	{
		class_names.clear();
		line = "";
		while (std::getline(ifs, line))class_names.push_back(line);
	}
	

	// load network model
	weight_path = "../data/models/yolov3/yolov3.weights";
	model_path = "../data/models/yolov3/yolov3.cfg";

	net = cv::dnn::readNetFromDarknet(model_path, weight_path);
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

	is_model_load = true;
}

std::vector<std::string> CYolo::Get_Output_Name()
{
	static std::vector<cv::String> names;
	if (names.empty()) {

		std::vector<int> outLayers = net.getUnconnectedOutLayers();
		std::vector<std::string> layersNames = net.getLayerNames();

		names.resize(outLayers.size());
		for (size_t i = 0; i < outLayers.size(); i++) {
			names[i] = layersNames[outLayers[i] - 1];
		}
	}
	return names;
}

void CYolo::postprocess(cv::Mat input_img)
{
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;
	for (size_t i = 0; i < outs.size(); i++) {
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			cv::Point classIdPoint;
			double confidence;
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold)
			{
				int centerX = (int)(data[0] * input_img.cols);
				int centerY = (int)(data[1] * input_img.rows);
				int width = (int)(data[2] * input_img.cols);
				int height = (int)(data[3] * input_img.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
	}

	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

	res_boxes.clear();
	res_classes.clear();

	for (size_t i = 0; i < indices.size(); i++) {
		int idx = indices[i];
		cv::Rect box = boxes[idx];
		res_boxes.push_back(box);
		std::string label = "";
		if (!class_names.empty()) {
			CV_Assert(classIds[idx] < (int)class_names.size());
			label = class_names[classIds[idx]];
		}
		res_classes.push_back(label);
	}
}

void CYolo::set_net_input(cv::Mat& input_img)
{
	static cv::Mat blob;
	cv::dnn::blobFromImage(input_img, blob, 1.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false, CV_8U);
	net.setInput(blob, "", 0.00392, cv::Scalar{ 0,0,0 });
	outs.clear();
}

void CYolo::postprocess(cv::Mat input_img, cv::Mat& output_img)
{
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;
	for (size_t i = 0; i < outs.size(); i++) {
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			cv::Point classIdPoint;
			double confidence;
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold)
			{
				int centerX = (int)(data[0] * input_img.cols);
				int centerY = (int)(data[1] * input_img.rows);
				int width = (int)(data[2] * input_img.cols);
				int height = (int)(data[3] * input_img.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
	}
	
	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

	res_boxes.clear();
	res_classes.clear();
	output_img = input_img.clone();

	for (size_t i = 0; i < indices.size(); i++) {
		int idx = indices[i];
		cv::Rect box = boxes[idx];
		drawPred(classIds[idx], confidences[idx], box.x, box.y,
			box.x + box.width, box.y + box.height, output_img);
		res_boxes.push_back(box);

		std::string label = "";
		if (!class_names.empty()) {
			CV_Assert(classIds[idx] < (int)class_names.size());
			label = class_names[classIds[idx]];
		}
		res_classes.push_back(label);
	}
}

bool CYolo::Get_Result(std::vector<cv::Rect>& boxes, std::vector<std::string>& classes)
{
	if (res_boxes.empty() || res_classes.empty())
		return false;
	boxes.clear(); classes.clear();
	boxes = res_boxes;
	classes = res_classes;
	return true;
}
