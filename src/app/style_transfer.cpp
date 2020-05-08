#include "style_transfer.h"

using namespace Chaf;
using namespace std;

CST::CST()
{

}

void CST::Set_Model(int flag)
{
	model_flag = flag;
}

void CST::Load_Model()
{
	model_path= "../data/models/style_transfer/"+models_list[model_flag];

	net = cv::dnn::readNetFromTorch(model_path);
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

	is_model_load = true;
}

void CST::get_net_output(vector<cv::Mat>& outs)
{
	outs.clear();
	cv::Mat output;
	output = net.forward();
	cv::Mat(output.size[2], output.size[3], CV_32F, output.ptr<float>(0, 0)) += 103.939;
	cv::Mat(output.size[2], output.size[3], CV_32F, output.ptr<float>(0, 1)) += 116.779;
	cv::Mat(output.size[2], output.size[3], CV_32F, output.ptr<float>(0, 2)) += 123.68;

	cv::dnn::imagesFromBlob(output, outs);
	//outs.push_back(output);
}

void CST::set_net_input(cv::Mat& input_img)
{
	cv::Mat blob = cv::dnn::blobFromImage(input_img, 1.0, input_img.size(), cv::Scalar(103.939, 116.779, 123.68), false, false);
	net.setInput(blob);
}

void CST::postprocess(cv::Mat input_img, cv::Mat& output_img)
{
	output_img.release();
	outs[0].convertTo(output_img, CV_8UC3);
}

void CST::Update(cv::Mat& input, cv::Mat& output)
{
	bool temp;
	Process(input, output, temp);
}