/*#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>*/
#include "dnn.h"


namespace Chaf
{
	class CST :	//	Chaf Style Transfer
		public CDNN
	{
	public:
		CST();

	public:
		void Set_Model(int flag);
		void Load_Model();
		void Update(cv::Mat& input, cv::Mat& output);

	private:
		std::vector<std::string> Get_Output_Name() { return std::vector<std::string>(); }
		void postprocess(cv::Mat input_img, cv::Mat& output_img);
		void postprocess(cv::Mat input_img){ }
		void set_net_input(cv::Mat& input_img);
		void get_net_output(std::vector<cv::Mat>& outs);
		

	private:
		int model_flag = 0;

	private:
		std::vector<std::string> models_list = { "feathers.t7","starry_night.t7","composition_vii.t7","candy.t7",
		"mosaic.t7","the_scream.t7","udnie.t7","the_wave.t7","la_muse.t7" };
	};
}