//TODO: Mask-RCNN FrameWork

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>
#include "dnn.h"

namespace Chaf
{
	class CMaskRCNN:
		public CDNN
	{
	public:
		CMaskRCNN();
		void Load_Model();
		bool Get_Result(std::vector<cv::Mat>& mask);

	private:
		std::vector<std::string> Get_Output_Name();
		void postprocess(cv::Mat input_img);
		void postprocess(cv::Mat input_img, cv::Mat& output_img);
		void set_net_input(cv::Mat& input_img);
		void draw_mask(cv::Mat& img, int classid, cv::Rect box, cv::Mat& object_mask);

	private:
		std::vector<cv::Scalar> colors_list;
		std::vector<cv::Mat> mask_list;
	};


}