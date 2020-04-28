#include "dnn.h"

namespace Chaf
{
	class CPose :
		public CDNN
	{
	public:
		CPose();

	public:
		void Load_Model();
		bool Get_Result(std::vector<cv::Point>& point_list);

	private:
		std::vector<std::string> Get_Output_Name() { return std::vector<std::string>(); };
		void postprocess(cv::Mat input_img);
		void postprocess(cv::Mat input_img, cv::Mat& output_img);
		void set_net_input(cv::Mat& input_img);
		void get_net_output(std::vector<cv::Mat>& outs);
		void draw_skeleton(cv::Mat& img);

	public:
		enum BODY_PAIR
		{
			Nose = 0,
			Neck = 1,
			RShoulder = 2,
			RElbow=3,
			RWrist=4,
			LShoulder=5,
			LElbow=6,
			LWrist=7,
			RHip=8,
			RKnee=9,
			RAnkle=10,
			LHip=11,
			LKnee=12,
			LAnkle=13,
			REye=14,
			LEye=15,
			REar=16,
			LEar=17,
			Background=18
		};

		int POSE_PAIRS[17][2] = {  {Neck, RShoulder} ,{Neck, LShoulder},{RShoulder, RElbow},
				   {RElbow, RWrist},{LShoulder, LElbow},{LElbow, LWrist},
				   {Neck, RHip},{RHip, RKnee},{RKnee, RAnkle},{Neck, LHip},
				   {LHip, LKnee},{LKnee, LAnkle},{Neck, Nose},{Nose, REye},
				   {REye, REar},{Nose, LEye},{LEye, LEar} };

	private:
		std::vector<cv::Rect> res_boxes;
		std::vector<std::string> res_classes;

		int nPoints = 18;
		int nPairs = 17;
		std::vector<cv::Point> points;
	};
}