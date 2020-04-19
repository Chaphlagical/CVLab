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
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    AddMainMenuBar();
    DisplayImage();
           
}

void CUI::OpenImageFIleDialog()
{
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png\0.jpg\0.bmp\0.jpeg\0\0", ".");
}

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

void CUI::AddMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            AddMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Load"))
        {
            if (ImGui::MenuItem("Load Yolo Model"))
            {
                if (!yolo_detection.IsModelLoad())
                    yolo_detection.Load_Model();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    DisplayImageFIleDialog();
}

void CUI::AddMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            AddMenuFile();
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Load"))
        {
            if (ImGui::MenuItem("Load Yolo Model"))
            {
                if (!yolo_detection.IsModelLoad())
                    yolo_detection.Load_Model();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    DisplayImageFIleDialog();
}

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
    ImGui::Separator();

    if (ImGui::BeginMenu("Color Setting"))
    {
        ImGui::ColorEdit4("Color", clear_color);
        ImGui::EndMenu();
    }
}

void CUI::DisplayImage()
{
    auto flag_iter = CImg_flag_list.begin();
    auto cimg_iter = CImg_list.begin();
    while (flag_iter != CImg_flag_list.end() && cimg_iter != CImg_list.end())
    {
        bool flag = *flag_iter;
        if (flag)
        {
            ImGui::Begin((*cimg_iter).name().c_str(), &flag);
            ImageTool(*cimg_iter);
            const auto flag_iter_temp = flag_iter;
            *flag_iter_temp = flag;
            ImGui::Image((void*)(intptr_t)(*cimg_iter).texture(), ImVec2((*cimg_iter).width(), (*cimg_iter).height()));
            ImGui::End();
            flag_iter++; cimg_iter++;
        }
        else
        {
            flag_iter = CImg_flag_list.erase(flag_iter);
            cimg_iter = CImg_list.erase(cimg_iter);
        }
    }

}

void CUI::AddImage(const std::string path, const std::string name)
{
    CImage cimg;
    if (cimg.Load_Image(path, name))
    {
        bool flag = true;
        CImg_flag_list.push_back(flag);
        CImg_list.push_back(cimg);
    }
}

void CUI::ImageTool(CImage& cimg)
{
    if (ImGui::Button("Object Detection"))
    {
        if (!yolo_detection.IsModelLoad())
            yolo_detection.Load_Model();
        yolo_detection.Detection(cimg.input_img, cimg.output_img);
        if (yolo_detection.Get_Result(cimg.res_boxes, cimg.res_classes))
        {
            cimg.Set_DisplayImage(cimg.output_img);
            std::cout << "aa" << std::endl;
        }
    }
}