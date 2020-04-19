#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>


namespace Chaf
{
	class CYolo
	{
	public:
		CYolo();

	public:
		void Load_Model();
		void Load_Image(const std::string img_path);
		void Process(bool display);
		
	private:
		void Init();
		std::vector<std::string> Get_Output_Name();
		void postprocess();
		void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);

	 private:
		float confThreshold = 0.5;	//Confidence threshold
		float nmsThreshold = 0.4;	//Non maximum suppression threshold
		int inpWidth = 416; //Network input height
		int inpHeight = 416; //Network input width 

		std::string model_cfg_path;
		std::string object_name_path;
		std::string weight_path;

		std::vector<std::string> class_names;
		std::vector<cv::Mat> outs;

		cv::dnn::Net yolo_net;
		cv::Mat input_img;
		cv::Mat output_img;

	};
}