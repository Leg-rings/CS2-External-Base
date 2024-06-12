#include "../Precompiled.h"

const char* arrFlagsNames[ ] =
{
    u8"金钱旗帜",
    u8"护甲旗帜",
    u8"工具钳旗帜",
    u8"拆除旗帜",
    u8"开镜旗帜"
};

void Gui::Initialize( unsigned int uFontFlags )
{
    // create fonts
    ImGuiIO& io = ImGui::GetIO( );

    ImGui::StyleColorsDark( );
        
    ImFontConfig imVerdanaConfig = { };
    imVerdanaConfig.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Bold;
    Fonts::Default = io.Fonts->AddFontFromFileTTF( X( "C:\\Windows\\Fonts\\msyh.ttc" ), 20, &imVerdanaConfig, io.Fonts->GetGlyphRangesChineseFull( ) );
    Fonts::ESP = io.Fonts->AddFontFromFileTTF( X( "C:\\Windows\\Fonts\\msyh.ttc" ), 10, &imVerdanaConfig, io.Fonts->GetGlyphRangesChineseFull( ) );

    m_bInitialized = ImGuiFreeType::BuildFontAtlas( io.Fonts, uFontFlags );
}

void Gui::Update( ImGuiIO& io )
{
    Gui::m_vecScreenSize = io.DisplaySize;

    io.MouseDrawCursor = m_bOpen;
    if (m_bOpen)
    {
        POINT p;
        if (GetCursorPos( &p ))
        {
            // set imgui mouse position
            io.MousePos = ImVec2( static_cast< float >( p.x ), static_cast< float >( p.y ) );
        }
    }

    // update any menu variables here
}

void Gui::Render( )
{
    if (!m_bInitialized)
		return;

    ImGuiIO& io = ImGui::GetIO( );
    ImDrawList* pForeGroundDrawList = ImGui::GetForegroundDrawList( );

    // update our stuff first
    Gui::Update( io );

    ImGui::SetNextWindowSize( ImVec2( 830, 500 ) );
    ImGui::Begin( X( u8"边手淫边写的狗屎" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse );
    {
        if (ImGui::Button( X( u8"自瞄" ), ImVec2( 100, 0 ) ) ) Tabs::m_iCurrentTab = 0;
        ImGui::SameLine( );
        if ( ImGui::Button( X( u8"扳机" ), ImVec2( 100, 0 ) ) ) Tabs::m_iCurrentTab = 1;
        ImGui::SameLine( );
        if ( ImGui::Button( X( u8"视觉" ), ImVec2( 100, 0 ) ) ) Tabs::m_iCurrentTab = 2;
        ImGui::SameLine();
        if (ImGui::Button(X(u8"配置"), ImVec2(100, 0))) Tabs::m_iCurrentTab = 3;

        switch (Tabs::m_iCurrentTab)
        {
        case 0:
            ImGui::Checkbox( X( u8"启用" ), &Config::Get<bool>(g_Variables.m_bEnableAimBot));
            if (Config::Get<bool>(g_Variables.m_bEnableAimBot))
            {
                ImGui::Text( X( u8"自瞄热键" ) );
                Keybind( X( u8"自瞄热键" ), &Config::Get<KeyBind_t>(g_Variables.m_AimBotKey) );
         
                ImGui::SliderFloat( X( u8"自瞄范围" ), &Config::Get<float>(g_Variables.m_flAimbotFOV), 0.0f, 180.0f, X( "%.f" ) );
                ImGui::SliderFloat( X( u8"自瞄平滑度" ), &Config::Get<float>(g_Variables.m_flAimbotSmooth), 0.0f, 100.0f, X( "%.f" ) );
            }
            break;
        case 1:
            ImGui::Checkbox( X( u8"启用" ), &Config::Get<bool>(g_Variables.m_bEnableTriggerbot));
            if (Config::Get<bool>(g_Variables.m_bEnableTriggerbot))
            {
                ImGui::Text( X( u8"扳机热键" ) );
                Keybind( X( u8"扳机热键" ), &Config::Get<KeyBind_t>(g_Variables.m_TriggerBotKey));

                ImGui::SliderInt( X( u8"扳机延迟" ), &Config::Get<int>(g_Variables.m_iTriggerBotDelay), 0, 1000, X( "%dms" ) );
            }
            break;
        case 2:
            ImGui::Checkbox( X( u8"启用" ), &Config::Get<bool>(g_Variables.m_bEnableVisuals));
            if (Config::Get<bool>(g_Variables.m_bEnableVisuals))
            {       
                ImGui::Checkbox( X( u8"团队检查" ), &Config::Get<bool>(g_Variables.m_bVisualsIgnoreTeammates));
                ImGui::Checkbox( X( u8"可见性检查" ), &Config::Get<bool>(g_Variables.m_bVisualsOnlyWhenSpotted));
                
                ImGui::Checkbox( X( u8"盒子" ), &Config::Get<bool>(g_Variables.m_bBox));
                if (Config::Get<bool>(g_Variables.m_bBox))
                    SimpleColorPicker( X( u8"盒子颜色" ), &Config::Get<Color>(g_Variables.m_colBox));

                ImGui::Checkbox( X( u8"健康值" ), &Config::Get<bool>(g_Variables.m_bHealthBar));

                ImGui::Checkbox( X( u8"护甲值" ), &Config::Get<bool>(g_Variables.m_bArmorBar));
                if (Config::Get<bool>(g_Variables.m_bArmorBar))
                    SimpleColorPicker( X( u8"护甲值颜色" ), &Config::Get<Color>(g_Variables.m_colArmorBar));

                ImGui::Checkbox( X( u8"玩家名称" ), &Config::Get<bool>(g_Variables.m_bName));

                ImGui::Checkbox( X( u8"玩家距离" ), &Config::Get<bool>(g_Variables.m_bDistance));

                ImGui::Checkbox( X( u8"捕捉线" ), &Config::Get<bool>(g_Variables.m_bSnapLines));
                if (Config::Get<bool>(g_Variables.m_bSnapLines))
                    SimpleColorPicker( X( u8"捕捉线颜色" ), &Config::Get<Color>(g_Variables.m_colSnapLines));

                MultiComboBox( X( u8"旗帜" ), arrFlagsNames, Config::Get<std::vector<bool>>(g_Variables.m_vecFlags), IM_ARRAYSIZE(arrFlagsNames));

                SimpleColorPicker( X( u8"视觉轮廓颜色" ), &Config::Get<Color>(g_Variables.m_colOutline));
            }
            break;
        case 3:
            ImGui::BeginColumns( X( "##Config-Columns" ), 2, ImGuiColumnsFlags_NoResize );
            {
                static int nSelected = -1;
                if (ImGui::BeginListBox(X("##Config-List"), ImVec2(-1, -1)))
                {
                    for (size_t uIndex = 0U; uIndex < Config::vecFileNames.size(); uIndex++)
                    {
                        if (ImGui::Selectable(Config::vecFileNames.at(uIndex).c_str(), uIndex == nSelected, ImGuiSelectableFlags_DontClosePopups, ImVec2(0, 0)))
                            nSelected = uIndex;
                    }

                    ImGui::EndListBox();
                }

                ImGui::NextColumn();

                static std::string strConfigFile;
                ImGui::InputTextWithHint(X("##config.file"), X(u8"输入配置文件名称"), &strConfigFile);
                if (ImGui::Button(X(u8"创建新配置"), ImVec2(0, 0)))
                {
                    Config::Save(strConfigFile);
                    strConfigFile.clear();
                    Config::Refresh();
                }

                if (ImGui::Button(X(u8"刷新配置"), ImVec2(0, 0)))
                    Config::Refresh();

                if (nSelected != -1)
                {
                    if (ImGui::Button(X(u8"保存配置"), ImVec2(0, 0)))
                    {
                        Config::Save(Config::vecFileNames.at(nSelected));
                    }

                    if (ImGui::Button(X(u8"载入配置"), ImVec2(0, 0)))
                    {
                        Config::Load(Config::vecFileNames.at(nSelected));
                    }

                    if (ImGui::Button(X(u8"移除配置"), ImVec2(0, 0)))
                    {
                        Config::Remove(Config::vecFileNames.at(nSelected));
                        Config::Refresh();

                        nSelected = -1;
                    }
                }
				
			}
            ImGui::EndColumns();
            break;
        default:
            ImGui::Text( X( "Something went wrong" ) );
            break;

        }  
    }
    ImGui::End( );
}