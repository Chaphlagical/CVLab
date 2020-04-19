#include "image.h"

using namespace Chaf;

CImage::CImage()
{
	Init();
}

void CImage::Init()
{
	yolo_detection.Load_Model();
	img_path = "";
}

void CImage::Load_Image(const std::string path)
{
	img_path = path;

}

void CImage::Object_Detection()
{
	yolo_detection.Load_Image(img_path);
	yolo_detection.Process();
}