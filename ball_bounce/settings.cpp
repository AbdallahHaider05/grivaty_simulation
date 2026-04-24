#include "settings.h"
#include "imgui.h"
#include "point.h"
#include <string> 
using namespace std;
void showObjectsPropertiesSettings(World& world) {
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(world.width * 0.2, world.heigth), ImGuiCond_Always);

    ImGui::Begin("Simulation Settings", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::PushItemWidth(100);
    ImGui::InputFloat("##timeVal", &world.timefactor,0.0f,0.0f,"%.2f");
    ImGui::SameLine();
    ImGui::PushID(0);
    if (ImGui::Button("-")) { world.timefactor /= 1.1f; }
    ImGui::SameLine();
    if (ImGui::Button("+")) { world.timefactor *= 1.1f; }
    ImGui::PopID();

    ImGui::SameLine();


    ImGui::Text("Time factor");

    ImGui::InputFloat("##zoomVal", &world.zoom, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushID(1);

    if (ImGui::Button("-")) { world.zoom /= 1.1f; }
    ImGui::SameLine();
    if (ImGui::Button("+")) { world.zoom *= 1.1f; }
    ImGui::PopID();

    ImGui::SameLine();
    ImGui::Text("Zoom");

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Separator();


    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Objects Properties");
    ImGui::Dummy(ImVec2(0.0f, 10.0f));




    for (int i = 0; i < world.bodies.size(); i++) {
        Body& body = world.bodies[i];
        ImGui::PushID(i);
        string name = " Object # " + to_string(i);
        

        ImGui::Text(name.c_str());
        ImGui::SameLine();
        if (world.BodyFollow != i) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

            if (ImGui::Button("Follow")) {
                world.BodyFollow = i;

            }
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));

            if (ImGui::Button("unFollow")) {
                world.BodyFollow = -1;

            }
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));


     
        if (ImGui::Button("delete")) {
            body.shown = false;
            if (world.BodyFollow == i) {
                world.BodyFollow -= 1;
           }


        }

        ImGui::PopStyleColor();

        Point point = world.bodies[i].postion;
        string postion = "x: " + to_string(point.x) + " y:" + to_string(point.y);
        ImGui::Text(postion.c_str());

        Point velocityVector = world.bodies[i].speed;
        string speed = "speed x: " + to_string(velocityVector.x) + " speed y:" + to_string(velocityVector.y);
        ImGui::Text(speed.c_str());




        if (ImGui::InputFloat("Mass", &body.mass, 5000.0f, 5000.f, "%.2f")) {
            body.updateRedius();
            world.updateGpuVram();
        }
            


        ImGui::PopID(); 
        ImGui::Separator();

        
    }

    ImGui::End();
}