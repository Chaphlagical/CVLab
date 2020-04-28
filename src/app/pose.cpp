#include "pose.h"

using namespace Chaf;
using namespace std;

CPose::CPose()
{
	points.clear();
}

void CPose::Load_Model()
{
	model_path = "../data/models/openpose/pose/coco/pose_deploy_linevec.prototxt";
	weight_path = "../data/models/openpose/pose/coco/pose_iter_440000.caffemodel";
	net = cv::dnn::readNetFromCaffe(model_path, weight_path);
	is_model_load = true;
}

void CPose::set_net_input(cv::Mat& input_img)
{
	cv::imshow("a", input_img);
	static cv::Mat blob = cv::dnn::blobFromImage(input_img, 1.0/255, cv::Size(368, 368), cv::Scalar(0, 0, 0), false, false);
	net.setInput(blob);
	outs.clear();
}

void CPose::get_net_output(vector<cv::Mat>& outs)
{
	cv::Mat output;
	output = net.forward();
	outs.push_back(output);
}

void CPose::postprocess(cv::Mat input_img)
{
	cv::Mat output = outs.back();
	int H = output.size[2];
	int W = output.size[3];
	points.clear();

	for (int i = 0; i < nPoints; i++)
	{
		cv::Mat probMap(H, W, CV_32F, output.ptr(0, i));

		cv::Point2f p(-1. - 1);
		cv::Point maxLoc;
		double prob;
		cv::minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
		if (prob > 0.6)
		{
			p = maxLoc;
			p.x *= (float)input_img.cols / W;
			p.y *= (float)input_img.rows / H;

		}
		points.push_back(p);
	}
}

void CPose::postprocess(cv::Mat input_img, cv::Mat& output_img)
{
	cv::Mat output = outs.back();
	int H = output.size[2];
	int W = output.size[3];
	points.clear();
	output_img = input_img.clone();

	for (int i = 0; i < nPoints; i++)
	{
		cv::Mat probMap(H, W, CV_32F, output.ptr(0, i));
		cv::Mat temp;
		//cv::resize(probMap, temp, input_img.size());
		//cv::imshow("a", temp);
		//cv::waitKey(0);
		cv::Point2f p(-1. - 1);
		cv::Point maxLoc;
		double prob;
		cv::minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
		if (prob > 0.3)
		{
			p = maxLoc;
			p.x *= (float)input_img.cols / W;
			p.y *= (float)input_img.rows / H;
			//cv::circle(output_img, cv::Point((int)p.x, (int)p.y), 8, cv::Scalar(0, 255, 255), -1);
			//cv::putText(output_img, cv::format("%d", i), cv::Point((int)p.x, (int)p.y), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 2);
		}
		points.push_back(p);
	}
	draw_skeleton(output_img);
}

bool CPose::Get_Result(vector<cv::Point>& point_list)
{
	if (points.empty())
		return false;

	point_list = points;
	return true;
}

void CPose::draw_skeleton(cv::Mat& img)
{
	if (points.empty())
		return;

	for (int i = 0; i < nPairs; i++)
	{
		cv::Point2f partA = points[POSE_PAIRS[i][0]];
		cv::Point2f partB = points[POSE_PAIRS[i][1]];

		if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
			continue;

		cv::line(img, partA, partB, cv::Scalar(0, 255, 255), 8);
		cv::circle(img, partA, 8, cv::Scalar(0, 0, 255), -1);
		cv::circle(img, partB, 8, cv::Scalar(0, 0, 255), -1);
	}
}