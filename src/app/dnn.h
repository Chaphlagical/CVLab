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

		void Process(cv::Mat input_img, bool& flag)
		{
			if (input_img.empty())
			{
				std::cout << "No image input!" << std::endl;
				return;
			}
			set_net_input(input_img);

			net.forward(outs, Get_Output_Name());
			postprocess(input_img);
			reload_net();
			flag = true;
		}

		void Process(cv::Mat input_img, cv::Mat& output_img, bool& flag)
		{
			if (input_img.empty())
			{
				std::cout << "No image input!" << std::endl;
				return;
			}

			set_net_input(input_img);

			net.forward(outs, Get_Output_Name());

			postprocess(input_img, output_img);
			reload_net();
			flag = true;
		}

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
		virtual void set_net_input(cv::Mat& input_img) = 0;
		virtual void reload_net() {}

		void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame)
		{
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

	protected:
		int inpWidth = 416; //Network input height
		int inpHeight = 416; //Network input width 		
		float confThreshold = 0.5;	//Confidence threshold
		float nmsThreshold = 0.4;	//Non maximum suppression threshold
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

