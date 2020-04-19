#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>


using namespace Chaf;

CImage::CImage()
{
	Init();
}

void CImage::Init()
{
	
	img_path = "";
}

unsigned char* CImage::Mat_to_Byte(const cv::Mat img)
{
    unsigned char* ptr = new unsigned char[long long(img.rows) * long long(img.cols) * 4];
    size_t bia = 0;
    for (size_t i = 0; i < img.rows; i++)
    {
        for (size_t j = 0; j < img.cols; j++)
        {
            for (int k = 2; k >=0; k--)
            {
                //*pt = img.at<cv::Vec3b>(i, j)[k];
                *(ptr+bia) = img.at<cv::Vec3b>(i, j)[k];
                bia++;
            }
            *(ptr + bia) = 255;
              bia++;
        }
    }
    return ptr;
}

bool CImage::LoadTextureFromFile(const char* filename, GLuint* out_texture)
{
    // Load from file
    unsigned char* image_data = Mat_to_Byte(img);

    /*size_t bia = 0;
    for (size_t i = 0; i < img.rows; i++)
    {
        for (size_t j = 0; j < img.cols; j++)
        {
            std::cout << "(";
            for (int k = 3; k >= 0; k--)
            {
                //*pt = img.at<cv::Vec3b>(i, j)[k];
                std::cout << int(*(image_data + bia)) << " ";
                bia++;
            }
            std::cout << ") ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    bia = 0;
    for (size_t i = 0; i < img.rows; i++)
    {
        for (size_t j = 0; j < img.cols; j++)
        {
            std::cout << "(";
            for (int k = 2; k >= 0; k--)
            {
                //*pt = img.at<cv::Vec3b>(i, j)[k];
                std::cout << int(*(image_dat + bia)) << "|";
                bia++;
            }
            std::cout << ") ";
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl << std::endl << "-----------------------------------------------------------------------------------------------------------------------------------------------" << std::endl << std::endl;
    */

    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}

void CImage::Load_Image(const std::string path, const std::string name)
{
	img_path = path;
    img_name = name;
    img = cv::imread(path);
    img_width = img.cols;
    img_height = img.rows;
	bool ret = LoadTextureFromFile(img_path.c_str(), &img_gui_texture);
    if(!ret)    
       std::cout << "Image Loading Failed!--"+img_path << std::endl;
}

void CImage::Object_Detection()
{
    yolo_detection.Load_Model();
    std::cout << "Load Model Complete" << std::endl;
	yolo_detection.Load_Image(img_path);
	yolo_detection.Process(false);
}

int CImage::width()
{
    return img_width;
}

int CImage::height()
{
    return img_height;
}

int CImage::texture()
{
    return img_gui_texture;
}

std::string CImage::name()
{
    return img_name;
}