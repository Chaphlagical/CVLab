#ifndef  DNN_H
#define DNN_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

namespace Chaf
{
	class CDNN
	{
	public:
		CDNN() {};
		virtual void Load_Model() = 0;
		virtual void Detection(cv::Mat input_img) = 0;
		virtual void Detection(cv::Mat input_img, cv::Mat& output_img) =0;
		virtual bool Get_Result(std::vector<cv::Rect>& boxes, std::vector<std::string>& classes) = 0;
		bool IsModelLoad() { return is_model_load; };

	protected:
		virtual void Init() 
		{
			model_path = "";
			object_name_path = "";
			weight_path = "";

			class_names.clear();
		};
		virtual std::vector<std::string> Get_Output_Name() = 0;
		virtual void postprocess(cv::Mat input_img) = 0;
		virtual void postprocess(cv::Mat input_img, cv::Mat& output_img) = 0;
		virtual void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) {};

	protected:
		std::string model_path;
		std::string object_name_path;
		std::string weight_path;
		std::vector<std::string> class_names;
		std::vector<cv::Mat> outs;
		bool is_model_load = false;
		cv::dnn::Net net;
	};
}






#endif // ! DNN_H

