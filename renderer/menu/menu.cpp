#include "menu.hpp"

#include <array>
#include <functional>

#include "../renderer.hpp"

#include "../../engine/engine.hpp"

#include "../../dependencies/rawdata/person.hpp"
#include "../../dependencies/rawdata/skeleton.hpp"
#include "../../dependencies/libs/imgui_tricks.hpp"

#include "../../dependencies/xorstring/xorstring.hpp"

#include "config/config.hpp"

#include "../../modules/aimbot/aimbot.hpp"
#include "../../modules/visuals/visuals.hpp"

#include "../../settings/settings.hpp"

static auto current_tab = 0;

void Draw_Main( float, float )
{
    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 0,0 } );

    ImGui::SetCursorPos( { 24, 235 } );
    ImGui::BeginChild( xs( "Main" ), { 337, 530 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );
    {
        switch ( current_tab )
        {
        case 0:
            renderer::imgui::elements::functions::checkbox( xs( "Aimbot" ), xs( "enables aimbot" ), std::get< bool >( menu::config::config_map[features::aimbot_enable] ) );

            renderer::imgui::elements::functions::hotkey( "##1", &std::get< int >( menu::config::config_map[features::aimbot_hotkey] ) );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::information( xs( "Smoothness" ), xs( "sets the smoothness of the aimbot" ), std::get< int >( menu::config::config_map[features::aimbot_smoothness] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##2", &std::get< int >( menu::config::config_map[features::aimbot_smoothness] ), 0, 10, "" );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::information( xs( "Bone" ), xs( "assigns the aiming bone" ), std::get< int >( menu::config::config_map[features::aimbot_bone] ), "%i" );

            renderer::imgui::elements::functions::combo( "##3", &std::get< int >( menu::config::config_map[features::aimbot_bone] ), "head\0\rneck\0\rchest\0\rpelvis\0\0", 1 );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::information( xs( "Max Distance" ), xs( "sets the distance interval to aim at" ), std::get< int >( menu::config::config_map[features::aimbot_max_distance] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##4", &std::get< int >( menu::config::config_map[features::aimbot_max_distance] ), 0, 150, "" );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Show Snap Lines" ), xs( "shows snap lines" ), std::get< bool >( menu::config::config_map[features::aimbot_snap_lines] ) );

            renderer::imgui::elements::functions::checkbox( xs( "Show FOV" ), xs( "shows circular fov" ), std::get< bool >( menu::config::config_map[features::aimbot_show_fov] ) );

            renderer::imgui::elements::functions::information( xs( "FOV" ), xs( "sets the aimbot fov" ), std::get< int >( menu::config::config_map[features::aimbot_fov] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##5", &std::get< int >( menu::config::config_map[features::aimbot_fov] ), 0, 100, "" );

            renderer::imgui::elements::functions::color_picker( "##6", std::get< engine::structures::vector4_t >( menu::config::config_map[features::aimbot_fov_color] ).color );

            renderer::imgui::elements::functions::information( xs( "FOV Segments" ), xs( "adjusts the aimbot fov shape" ), std::get< int >( menu::config::config_map[features::aimbot_fov_segments] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##7", &std::get< int >( menu::config::config_map[features::aimbot_fov_segments] ), 0, 10, "" );
        break;
        case 1:
            
        break;
        case 2:
            
        break;
        case 3:
            renderer::imgui::elements::functions::information( xs( "Max Distance" ), xs( "sets the max visual distance" ), std::get< int >( menu::config::config_map[features::display_max_distance ] ), "%i" );

            renderer::imgui::elements::functions::slider_int( "##1", &std::get< int >( menu::config::config_map[features::display_max_distance ] ), 0, 1000, "" );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Bounding Box" ), xs( "draws a rectangular box on visual entities" ), std::get< bool >( menu::config::config_map[features::display_bounding_box] ) );

            renderer::imgui::elements::functions::color_picker( "##11", std::get< engine::structures::vector4_t >( menu::config::config_map[features::bounding_box_color] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Filled Box" ), xs( "draws a filled rectangular box on visual entities" ), std::get< bool >( menu::config::config_map[features::display_filled_box ] ) );

            renderer::imgui::elements::functions::color_picker( "##12", std::get< engine::structures::vector4_t >( menu::config::config_map[features::filled_box_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Corner Box" ), xs( "draws a cornered box on visual entities" ), std::get< bool >( menu::config::config_map[features::display_corner_box ] ) );

            renderer::imgui::elements::functions::color_picker( "##13", std::get< engine::structures::vector4_t >( menu::config::config_map[features::corner_box_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Head Hitbox" ), xs( "draws a head square on visual entities" ), std::get< bool >( menu::config::config_map[features::display_head_hitbox ] ) );

            renderer::imgui::elements::functions::color_picker( "##14", std::get< engine::structures::vector4_t >( menu::config::config_map[features::hitbox_color ] ).color );

            renderer::imgui::elements::functions::separator( );

            renderer::imgui::elements::functions::checkbox( xs( "Entity Name" ), xs( "draws entity's name" ), std::get< bool >( menu::config::config_map[features::display_players_name] ) );

            renderer::imgui::elements::functions::color_picker( "##17", std::get< engine::structures::vector4_t >( menu::config::config_map[features::player_names_color ] ).color );

            renderer::imgui::elements::functions::separator( );    

            renderer::imgui::elements::functions::checkbox( xs( "Distance" ), xs( "draws distance of entity" ), std::get< bool >( menu::config::config_map[features::display_distance ] ) );

            renderer::imgui::elements::functions::color_picker( "##18", std::get< engine::structures::vector4_t >( menu::config::config_map[features::distance_color ] ).color );
            break;
        case 4:
            renderer::imgui::elements::functions::button( xs( "Save Config" ), { 328, 35 }, renderer::imgui::elements::button_flags::BUTTON_PURPLE );

            ImGui::NewLine( );

            renderer::imgui::elements::functions::button( xs( "Load Config" ), { 328, 35 }, renderer::imgui::elements::button_flags::BUTTON_GRAY );
            break;
        default:
            break;
        }
    }
    ImGui::EndChild( );

    ImGui::PopStyleVar( );
}

void renderer::menu::draw( )
{
    {
        modules::aimbot::visual::draw_fov( );
    } modules::visuals::callback( );

    if ( renderer::values::renderer->is_menu_open )
    {
        ImGui::SetNextWindowSize( { 375, 770 }, ImGuiCond_Once );

        ImGui::Begin( xs( "UnrealWindow" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove );
        {
            auto DrawList = ImGui::GetWindowDrawList( );
            auto Position = ImGui::GetWindowPos( );

            DrawList->AddText( renderer::imgui::elements::fonts::druk_wide_bold, 30, { Position.x + 24, Position.y + 36 }, ImColor{ 255, 255, 255 }, xs( "Fortnite" ) );

            DrawList->AddRectFilled( { Position.x + 24, Position.y + 73 }, { Position.x + 24 + 187, Position.y + 73 + 2 }, ImColor{ 255, 255, 255, 10 } );
            DrawList->AddRectFilled( { Position.x + 24, Position.y + 73 }, { Position.x + 24 + 88, Position.y + 73 + 2 }, ImColor{ 127, 128, 246 } );

            ImGui::SetCursorPos( { 24, 107 } );

            ImGui::BeginGroup( );
            {
                renderer::imgui::elements::functions::tab( xs( "LEGIT" ), ICON_FA_SHIELD_CHECK, current_tab, 0 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "RAGE" ), ICON_FA_SHIELD, current_tab, 1 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "MISC" ), ICON_FA_FOLDER, current_tab, 2 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "VISUALS" ), ICON_FA_BOLT, current_tab, 3 );

                ImGui::SameLine( );

                renderer::imgui::elements::functions::tab( xs( "CFG" ), ICON_FA_ARCHIVE, current_tab, 4 );

            }
            ImGui::EndGroup( );

            Draw_Main( 0.f, 0.f );
        }
        ImGui::End( );
    }
    else
    {
        ImGui::Begin( xs( "UnrealWindow_Watermark" ), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove );
        { 
            auto DrawList = ImGui::GetWindowDrawList( );

            ImGui::SetWindowSize( { 220, 40 } );

            ImGui::SetWindowPos( { 0, 0 } );

            DrawList->AddText( renderer::imgui::elements::fonts::druk_wide_bold, 25, { 15, 5 }, ImColor{ 255, 255, 255, 70 }, xs( "loaded" ) );
        }
        ImGui::End( );
    }
}