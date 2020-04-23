#include "maskrcnn.h"
#include <fstream>

using namespace Chaf;

CMaskRCNN::CMaskRCNN()
{

}

void CMaskRCNN::Load_Model()
{
	// Initialize object classes names
	object_name_path = "../data/models/mask_rcnn/mscoco_labels.names";
	std::ifstream ifs(object_name_path.c_str());
	std::string line;
	if (ifs.is_open())
	{
		class_names.clear();
		line = "";
		while (std::getline(ifs, line))class_names.push_back(line);
	}

	std::string color_path = "../data/models/mask_rcnn/colors.txt";
	std::ifstream color_ifs(color_path.c_str());
	if (color_ifs.is_open())
	{
		colors_list.clear();
		line = "";
		while (std::getline(color_ifs, line))
		{
			char* pEnd;
			double r, g, b;
			//字符串转换成浮点数
			r = strtod(line.c_str(), &pEnd);
			g = strtod(pEnd, NULL);
			b = strtod(pEnd, NULL);
			colors_list.push_back(cv::Scalar(r, g, b, 255.0));
		}
	}
	
	// load network model
	weight_path = "../data/models/mask_rcnn/mask_rcnn_inception_v2_coco_2018_01_28.pb";
	 model_path = "../data/models/mask_rcnn/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";

	net = cv::dnn::readNetFromTensorflow(weight_path, model_path);
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

	is_model_load = true;
}

void CMaskRCNN::set_net_input(cv::Mat& input_img)
{
	static cv::Mat blob;
	cv::dnn::blobFromImage(input_img, blob, 1.0, cv::Size(input_img.cols, input_img.rows), cv::Scalar(0, 0, 0), true, false, CV_8U);
	net.setInput(blob);
	outs.clear();
}

bool CMaskRCNN::Get_Result(std::vector<cv::Mat>& mask)
{
	if (mask_list.empty())
		return false;
	mask.clear();
	mask = mask_list;
	return true;
}

std::vector<std::string> CMaskRCNN::Get_Output_Name()
{
	std::vector<std::string> outputname(2);
	outputname[0] = "detection_out_final";
	outputname[1] = "detection_masks";
	return outputname;
}


void CMaskRCNN::postprocess(cv::Mat input_img)
{
	cv::Mat outDetections = outs[0];
	cv::Mat outMasks = outs[1];
	mask_list.clear();

	const int numDetections = outDetections.size[2];

	const int numClasses = outMasks.size[1];

	outDetections = outDetections.reshape(1, outDetections.total() / 7);

	for (int i = 0; i < numDetections; ++i)
	{

		float score = outDetections.at<float>(i, 2);

		if (score > confThreshold)
		{
			int classId = static_cast<int>(outDetections.at<float>(i, 1));
			int left = static_cast<int>(input_img.cols * outDetections.at<float>(i, 3));
			int top = static_cast<int>(input_img.rows * outDetections.at<float>(i, 4));
			int right = static_cast<int>(input_img.cols * outDetections.at<float>(i, 5));
			int bottom = static_cast<int>(input_img.rows * outDetections.at<float>(i, 6));

			left = std::max(0, std::min(left, input_img.cols - 1));
			top = std::max(0, std::min(top, input_img.rows - 1));
			right = std::max(0, std::min(right, input_img.cols - 1));
			bottom = std::max(0, std::min(bottom, input_img.rows - 1));
			cv::Rect box = cv::Rect(left, top, right - left + 1, bottom - top + 1);

			cv::Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));
			resize(objectMask, objectMask, cv::Size(box.width, box.height));
			mask_list.push_back(objectMask);
		}
	}
}


void CMaskRCNN::postprocess(cv::Mat input_img, cv::Mat& output_img)
{
	cv::Mat outDetections = outs[0];
	cv::Mat outMasks = outs[1];

	const int numDetections = outDetections.size[2];

	const int numClasses = outMasks.size[1];

	outDetections = outDetections.reshape(1, outDetections.total() / 7);

	output_img = input_img.clone();

	for (int i = 0; i < numDetections; ++i)
	{

		float score = outDetections.at<float>(i, 2);

		if (score > confThreshold)
		{
			int classId = static_cast<int>(outDetections.at<float>(i, 1));
			int left = static_cast<int>(input_img.cols * outDetections.at<float>(i, 3));
			int top = static_cast<int>(input_img.rows * outDetections.at<float>(i, 4));
			int right = static_cast<int>(input_img.cols * outDetections.at<float>(i, 5));
			int bottom = static_cast<int>(input_img.rows * outDetections.at<float>(i, 6));

			//防止框画在外面
			left = std::max(0, std::min(left, input_img.cols - 1));
			top = std::max(0, std::min(top, input_img.rows - 1));
			right = std::max(0, std::min(right, input_img.cols - 1));
			bottom = std::max(0, std::min(bottom, input_img.rows - 1));
			cv::Rect box = cv::Rect(left, top, right - left + 1, bottom - top + 1);

			std::string label = "";
			//获取标签
			if (!class_names.empty())
			{
				CV_Assert(classId < (int)class_names.size());
				label = class_names[classId] + ":";
			}
			
			cv::Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));
			resize(objectMask, objectMask, cv::Size(box.width, box.height));
			mask_list.push_back(objectMask);
			draw_mask(output_img, classId, box, objectMask);
		}
	}
}

void CMaskRCNN::draw_mask(cv::Mat& img, int classid, cv::Rect box, cv::Mat& object_mask)
{
	cv::Scalar color = colors_list[classid % colors_list.size()];
	cv::Mat mask = (object_mask > confThreshold);
	cv::Mat colorROI = (0.3 * color + 0.7 * img(box));
	colorROI.convertTo(colorROI, CV_8UC3);
	std::vector<cv::Mat> contours;
	cv::Mat hierarchy;
	mask.convertTo(mask, CV_8U);
	findContours(mask, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
	drawContours(colorROI, contours, -1, color, 5, cv::LINE_8, hierarchy, 100);
	colorROI.copyTo(img(box), mask);

	cv::putText(img, class_names[classid], cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0), 1);
}

void CMaskRCNN::reload_net()
{
	net = cv::dnn::readNetFromTensorflow(weight_path, model_path);
}