#include "ui.h"
#include "fasterrcnn.h"


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

    yolo_detection.Load_Model();
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

    //AddMainMenuBar();
    if (!ImGui::Begin(w_name.c_str(), p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    AddMenuBar();
    
    if (ImGui::Button("yolo"))
    {
        Chaf::CFasterRCNN rcnn;
        //rcnn.Load_Model();
        //rcnn.Load_Image("../data/images/wolverine.jpg");
        //rcnn.Detection();
        CYolo yolo;
        yolo.Load_Model();
        yolo.Load_Image("../data/images/wolverine.jpg");
        yolo.Process();
    }

    
    ImGui::End();
}

void CUI::OpenImageFIleDialog()
{
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*\0.png\0.jpg\0.bmp\0.jpeg\0\0", ".");
}

void CUI::DisplayImageFIleDialog()
{
    // display
    if (ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk == true)
        {
            choose_file_name = ImGuiFileDialog::Instance()->GetFilepathName();
            choose_file_path = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }
        // close
        ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
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
        ImGui::EndMainMenuBar();
    }
    DisplayImageFIleDialog();
}
#include <iostream>
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
    ImGui::Begin("OpenGL Texture Text");
    ImGui::Text("pointer = %p", my_image_texture);
    ImGui::Text("size = %d x %d", my_image_width, my_image_height);
    ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
    ImGui::End();
}