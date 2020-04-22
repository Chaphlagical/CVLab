#include "ui.h"


using namespace Chaf;

CUI::CUI()
{
    w_name = "Demo";
    Init();
}

CUI::CUI(std::string name)
{
    w_name = name;
    Init();
}

void CUI::Init()
{
    no_titlebar = false;
    no_scrollbar = false;
    no_menu = false;
    no_move = false;
    no_resize = false;
    no_collapse = false;
    no_close = false;
    no_nav = false;
    no_background = false;
    no_bring_to_front = false;

    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    clear_color[0] = 0.45;
    clear_color[1] = 0.55;
    clear_color[2] = 0.60;
    clear_color[3] = 1.00;

}

ImVec4 CUI::GetColor()
{
    return ImVec4(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
}

void CUI::ShowUI(bool* p_open_flag)
{
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup
    p_open = p_open_flag;
    //TODO:: show main UI

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    AddMainMenuBar();

    DisplayImage();
    
}

//  Add a File Dialog, place where you want to open a file
void CUI::OpenImageFIleDialog()
{
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".jpg\0.png\0.bmp\0.jpeg\0\0", ".");
}

//  Display image file dialog, place it at the end of menubar function
void CUI::DisplayImageFIleDialog()
{
    // display
    if (ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk == true)
        {
            std::string path = ImGuiFileDialog::Instance()->GetCurrentPath()+"/"+ImGuiFileDialog::Instance()->GetCurrentFileName();
            std::string name= ImGuiFileDialog::Instance()->GetCurrentFileName();
            std::cout << path;
            AddImage(path, name);
        }
        // close
        ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
        
        //DisplayImage();
    }
    
}

// Add a main menu bar
void CUI::AddMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            AddMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Setting"))
        {
            if (ImGui::BeginMenu("Color Setting"))
            {
                ImGui::ColorEdit4("Color", clear_color);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    DisplayImageFIleDialog();
}

//  Add menu bar
void CUI::AddMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            AddMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

//  Add menu file
void CUI::AddMenuFile()
{
    //ImGui::MenuItem("(dummy menu)", NULL, false, false);

    if (ImGui::MenuItem("Open", "Ctrl+O")) 
    {
        OpenImageFIleDialog();

    }

    /*if (ImGui::BeginMenu("Open Recent"))
    {
    }*/
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
}

ImVec2 CUI::Get_window_mouse_pos()
{
    ImVec2 pos;
    float delta_x = ImGui::GetWindowWidth() - ImGui::GetContentRegionAvail().x;
    float delta_y = ImGui::GetWindowHeight() - ImGui::GetContentRegionAvail().y;
    pos.x = ImGui::GetMousePos().x - ImGui::GetWindowPos().x - delta_x;
    pos.y = ImGui::GetMousePos().y - ImGui::GetWindowPos().y - delta_y;
    return pos;
}

//////////////////////////////////////// Image UI //////////////////////////////////////////////////

//  display images list
void CUI::DisplayImage()
{
    if(CImg_list.size()>0)
        ImageMenu();

    auto flag_iter = CImg_flag_list.begin();
    auto cimg_iter = CImg_list.begin();
    
    while (flag_iter != CImg_flag_list.end() && cimg_iter != CImg_list.end())
    {
        bool hide_flag = (*flag_iter)[0];
        bool open_flag = (*flag_iter)[1];

        if (!open_flag)
        {
            flag_iter = CImg_flag_list.erase(flag_iter);
            cimg_iter = CImg_list.erase(cimg_iter);
        }
        else
        {
            bool temp_flag = hide_flag;
            if (hide_flag)
            {
                ImGui::Begin((*cimg_iter).name().c_str(), &temp_flag, window_flags);
                ImageTool(*cimg_iter);
                (*flag_iter)[1] = temp_flag;
                ImGui::Image((void*)(intptr_t)(*cimg_iter).texture(), ImVec2((*cimg_iter).width(), (*cimg_iter).height()));
                ImGui::End();
            }
            flag_iter++; cimg_iter++;
        }
        
    }

}

//  add an image to the list
void CUI::AddImage(const std::string path, const std::string name)
{
    CImage cimg;
    if (cimg.Load_Image(path, name))
    {
        std::vector<bool> temp_vec = { true,true };
        CImg_flag_list.push_back(temp_vec);

        CImg_list.push_back(cimg);
    }
}

//  add an image tool
void CUI::ImageTool(CImage& cimg)
{   
    int select_flag = 0;
    if (ImGui::IsMouseClicked(1))
    {
        cimg.Select_Boundingbox(Get_window_mouse_pos());
        select_flag = 1;
    }
        

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Algorithm"))
        {
            if (ImGui::MenuItem("Object Detection"))
            {
                if (!yolo_detection.IsModelLoad())
                    yolo_detection.Load_Model();
                yolo_detection.Detection(cimg.input_img, cimg.output_img);
                if (yolo_detection.Get_Result(cimg.res_boxes, cimg.res_classes))
                {
                    cimg.Set_Display_Output();
                }
            }
            ImGui::EndMenu();
        }       
        ImGui::EndMenuBar();
    }    
    if (cimg.width() != ImGui::GetContentRegionAvail().x || cimg.height() != ImGui::GetContentRegionAvail().y)
        cimg.Set_Image_Size(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y, select_flag);
}

//  add an image menu
void CUI::ImageMenu()
{
    bool controller_flag = true;
    if (CImg_flag_list.size() == 0)
        controller_flag = !controller_flag;
    ImGui::Begin("Image Menu", &controller_flag);
    ImGui::Text("Images Displayment");
    auto flag_iter = CImg_flag_list.begin();
    auto cimg_iter = CImg_list.begin();
    
    while (flag_iter != CImg_flag_list.end() && cimg_iter != CImg_list.end())
    {
        bool temp_flag = (*flag_iter)[0];
        ImGui::Checkbox((*cimg_iter).name().c_str(), &temp_flag);
        (*flag_iter)[0] = temp_flag;
        flag_iter++; cimg_iter++;
    }

    ImGui::Text("Load Models");
    if (ImGui::Button("Load Yolo Model"))
    {
        if (!yolo_detection.IsModelLoad())
            yolo_detection.Load_Model(); 
    }
    if(yolo_detection.IsModelLoad())
        ImGui::Text("Yolo Model Loaded!");

    ImGui::End();    
    if (!controller_flag)
    {
        CImg_flag_list.clear();
        CImg_list.clear();
    }
}

