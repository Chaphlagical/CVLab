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

bool CImage::LoadTexture(GLuint* out_texture)
{
    // Load from file
    unsigned char* image_data = Mat_to_Byte(display_img);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, display_img.cols, display_img.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}

bool CImage::Load_Image(const std::string path, const std::string name)
{
	img_path = path;
    img_name = name;
    input_img = cv::imread(path);
    output_img = input_img.clone();
    display_img = output_img.clone();
    img_width = input_img.cols;
    img_height = input_img.rows;
	bool ret = LoadTexture(&img_gui_texture);
    if (!ret)
        return false;
    return true;
}

int CImage::width()
{
    return display_img.cols;
}

int CImage::height()
{
    return display_img.rows;
}

int CImage::texture()
{
    return img_gui_texture;
}

std::string CImage::name()
{
    return img_name;
}

void CImage::Set_DIsplay_Input()
{
    display_img = input_img.clone();
    LoadTexture(&img_gui_texture);
}

void CImage::Set_Display_Output()
{
    display_img = output_img.clone();
    LoadTexture(&img_gui_texture);
}

void CImage::Set_Select()
{
    if (!select_img.empty())
    {
        output_img = select_img.clone();
        input_img = select_img.clone();
        display_img = select_img.clone();
        res_boxes.clear(); res_classes.clear();
        img_height = select_img.rows;
        img_width = select_img.cols;
        LoadTexture(&img_gui_texture);
    }
}

void CImage::Reset_Image_Size()
{
    cv::resize(output_img, display_img, cv::Size(img_width, img_height));
    LoadTexture(&img_gui_texture);
}

void CImage::Set_Image_Size(const int width, const int height, const int set_flag)
{
    if (width > 0 && height > 0)
    {
        if(set_flag==0)
            cv::resize(output_img, display_img, cv::Size(width, height));
        else if(set_flag==1)
            cv::resize(display_img, display_img, cv::Size(width, height));
        LoadTexture(&img_gui_texture);
    }
}

void CImage::Reset()
{
    output_img = input_img.clone();
    Set_Display_Output();
}

void CImage::Select_Boundingbox(ImVec2 pos)
{
    ImVec2 scale_vec;
    scale_vec.x = (float)input_img.cols / (float)display_img.cols;
    scale_vec.y = (float)input_img.rows / (float)display_img.rows;
    pos.x *= scale_vec.x;
    pos.y *= scale_vec.y;
    if (res_boxes.size() > 0)
    {
        for (size_t i = 0; i < res_boxes.size(); i++)
        {
            if (pos.x - res_boxes[i].x < res_boxes[i].width && pos.y - res_boxes[i].y < res_boxes[i].height
                &&pos.x>res_boxes[i].x&&pos.y>res_boxes[i].y)
            {
                select_box = res_boxes[i];
                select_img = input_img(select_box);
                display_img = output_img.clone();

                for (size_t x = select_box.x; x < (size_t)select_box.x+ (size_t)select_box.width; x++)
                {
                    for (size_t y = select_box.y; y < (size_t)select_box.y+ (size_t)select_box.height; y++)
                    {
                        if (display_img.at<cv::Vec3b>(y, x)[0] < 255 - 100)
                        {
                            display_img.at<cv::Vec3b>(y, x)[0] +=100;
                        }
                        else
                            display_img.at<cv::Vec3b>(y, x)[0] = 255;
                    }
                }
                LoadTexture(&img_gui_texture);
                return;
            }
            else
            {
                select_img.release();
                Set_Display_Output();
            }
        }
    }
}