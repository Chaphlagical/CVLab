/*#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>*/
#include "dnn.h"


namespace Chaf
{
	class CYolo: 
		public CDNN
	{
	public:
		CYolo();

	public:
		void Load_Model();
		void Detection(cv::Mat input_img);
		void Detection(cv::Mat input_img, cv::Mat& output_img);
		bool Get_Result(std::vector<cv::Rect>& boxes, std::vector<std::string>& classes);
		bool IsDetected();

	private:
		std::vector<std::string> Get_Output_Name();
		void postprocess(cv::Mat input_img);
		void postprocess(cv::Mat input_img, cv::Mat& output_img);
		void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);

	 private:
		float confThreshold = 0.5;	//Confidence threshold
		float nmsThreshold = 0.4;	//Non maximum suppression threshold
		int inpWidth = 416; //Network input height
		int inpHeight = 416; //Network input width 		

		std::vector<cv::Rect> res_boxes;
		std::vector<std::string> res_classes;
		bool isdetected = false;
		
	};
}