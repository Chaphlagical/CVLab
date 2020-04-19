#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

namespace Chaf
{
	class CFasterRCNN
	{
	public:
		CFasterRCNN();

	public:
		void Init();
		void Load_Model();
		void Load_Image(const std::string img_path);
		void Detection();

	private:
		float score_threshold = 0.3;
		int Width = 300; //Network input height
		int Height = 300; //Network input width 

		std::string pb_file;
		std::string pbtxt_file;
		std::string object_name_path;

		std::vector<std::string> class_names;
		cv::Mat outs;

		cv::dnn::Net net_;
		cv::Mat input_img;

	};
}