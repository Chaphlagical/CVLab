#include "yolo.h"
#include <fstream>

using namespace Chaf;

CYolo::CYolo()
{
	Init();
}

void CYolo::Init()
{
	model_cfg_path = "";
	object_name_path = "";
	weight_path = "";

	class_names.clear();
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
	model_cfg_path = "../data/models/yolov3/yolov3.cfg";

	yolo_net = cv::dnn::readNetFromDarknet(model_cfg_path, weight_path);
	yolo_net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	yolo_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

	is_model_load = true;
}

void CYolo::Detection(cv::Mat input_img)
{
	if (input_img.empty())
	{
		std::cout << "No image input!" << std::endl;
		return;
	}

	static cv::Mat blob;
	cv::dnn::blobFromImage(input_img, blob, 1.0, cv::Size(inpWidth,inpHeight), cv::Scalar(0,0,0), true, false, CV_8U);
	
	yolo_net.setInput(blob, "", 0.00392, cv::Scalar{0,0,0});

	yolo_net.forward(outs, Get_Output_Name());

	postprocess(input_img);
}

void CYolo::Detection(cv::Mat input_img, cv::Mat& output_img)
{
	if (input_img.empty())
	{
		std::cout << "No image input!" << std::endl;
		return;
	}

	clock_t t = clock();

	static cv::Mat blob;
	cv::dnn::blobFromImage(input_img, blob, 1.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false, CV_8U);

	std::cout << clock() - t << std::endl;
	t = clock();

	yolo_net.setInput(blob, "", 0.00392, cv::Scalar{ 0,0,0 });
	std::cout << clock() - t << std::endl;
	t = clock();

	yolo_net.forward(outs, Get_Output_Name());

	std::cout << clock() - t << std::endl;
	t = clock();

	postprocess(input_img, output_img);

	std::cout << clock() - t << std::endl;
	t = clock();
}

std::vector<std::string> CYolo::Get_Output_Name()
{
	static std::vector<cv::String> names;
	if (names.empty()) {

		std::vector<int> outLayers = yolo_net.getUnconnectedOutLayers();
		std::vector<std::string> layersNames = yolo_net.getLayerNames();

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

void CYolo::drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) {

	cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 178, 50), 3);
	std::string label = cv::format("%.2f", conf);
	if (!class_names.empty()) {
		CV_Assert(classId < (int)class_names.size());
		label = class_names[classId] + ":" + label;
	}

	int baseLine;
	cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = std::max(top, labelSize.height);
	cv::rectangle(frame, cv::Point(left, top - round(1.5 * labelSize.height)), cv::Point(left + round(1.5 * labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
	cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0), 1);
}

bool CYolo::Get_Result(std::vector<cv::Rect>& boxes, std::vector<std::string>& classes)
{
	if (res_boxes.empty() || res_classes.empty())
		return false;
	boxes = res_boxes;
	classes = res_classes;
	return true;
}

bool CYolo::IsModelLoad()
{
	return is_model_load;
}