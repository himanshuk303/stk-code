//  SuperTuxKart - a fun racing game with go-kart
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "graphics/callbacks.hpp"
#include "guiengine/engine.hpp"
#include "modes/world.hpp"
#include "tracks/track.hpp"

using namespace video;

void NormalMapProvider::OnSetConstants(IMaterialRendererServices *srv, int)
{
    if (!firstdone)
    {
        s32 decaltex = 0;
        srv->setPixelShaderConstant("DecalTex", &decaltex, 1);

        s32 bumptex = 1;
        srv->setPixelShaderConstant("BumpTex", &bumptex, 1);

        s32 lightmapTex = (m_with_lightmap ? 2 : 0);
        srv->setPixelShaderConstant("LightMapTex", &lightmapTex, 1);

        s32 hasLightMap = (m_with_lightmap ? 1 : 0);
        srv->setPixelShaderConstant("HasLightMap", &hasLightMap, 1);

        // We could calculate light direction as coming from the sun (then we'd need to
        // transform it into camera space). But I find that pretending light
        // comes from the camera gives good results
        const float lightdir[] = {0.1852f, -0.1852f, -0.9259f};
        srv->setVertexShaderConstant("lightdir", lightdir, 3);


        firstdone = true;
    }
}

void WaterShaderProvider::OnSetConstants(IMaterialRendererServices *srv, int)
{
    m_dx_1 += GUIEngine::getLatestDt()*m_water_shader_speed_1;
    m_dy_1 += GUIEngine::getLatestDt()*m_water_shader_speed_1;

    m_dx_2 += GUIEngine::getLatestDt()*m_water_shader_speed_2;
    m_dy_2 -= GUIEngine::getLatestDt()*m_water_shader_speed_2;

    if (m_dx_1 > 1.0f) m_dx_1 -= 1.0f;
    if (m_dy_1 > 1.0f) m_dy_1 -= 1.0f;
    if (m_dx_2 > 1.0f) m_dx_2 -= 1.0f;
    if (m_dy_2 < 0.0f) m_dy_2 += 1.0f;

    srv->setVertexShaderConstant("delta1", &m_dx_1, 2);
    srv->setVertexShaderConstant("delta2", &m_dx_2, 2);

    if (m_fog)
    {
        Track* t = World::getWorld()->getTrack();

        const float fogStart = t->getFogStart();
        srv->setPixelShaderConstant("fogFrom", &fogStart, 1);

        const float fogEnd = t->getFogEnd();
        srv->setPixelShaderConstant("fogTo", &fogEnd, 1);

        const SColor fogColor = t->getFogColor();
        const float fogColorVec[] = {fogColor.getRed()/255.0f,
                               fogColor.getGreen()/255.0f,
                               fogColor.getBlue()/255.0f, 1.0f};
        srv->setVertexShaderConstant("fogColor", fogColorVec, 4);
    }

    if (!firstdone)
    {
        s32 decaltex = 0;
        srv->setPixelShaderConstant("DecalTex", &decaltex, 1);

        s32 bumptex = 1;
        srv->setPixelShaderConstant("BumpTex1", &bumptex, 1);

        bumptex = 2;
        srv->setPixelShaderConstant("BumpTex2", &bumptex, 1);

        // We could calculate light direction as coming from the sun (then we'd need to
        // transform it into camera space). But I find that pretending light
        // comes from the camera gives good results
        const float lightdir[] = {-0.315f, 0.91f, -0.3f};
        srv->setVertexShaderConstant("lightdir", lightdir, 3);

        firstdone = true;
    }
}

void GrassShaderProvider::OnSetConstants(IMaterialRendererServices *srv, int userData)
{
//    grass_shaders_times[userData] += GUIEngine::getLatestDt()*m_speed;
//    if (grass_shaders_times[userData] > M_PI*2) grass_shaders_times[userData] -= M_PI*2;

//    srv->setVertexShaderConstant("angle", &grass_shaders_times[userData], 1);
    srv->setVertexShaderConstant("amplitude", &m_amplitude, 1);

    int fog = (m_fog ? 1 : 0);
    srv->setVertexShaderConstant("fog", &fog, 1);

    if (m_fog)
    {
        Track* t = World::getWorld()->getTrack();

        const float fogStart = t->getFogStart();
        srv->setPixelShaderConstant("fogFrom", &fogStart, 1);

        const float fogEnd = t->getFogEnd();
        srv->setPixelShaderConstant("fogTo", &fogEnd, 1);

        const SColor fogColor = t->getFogColor();
        const float fogColorVec[] = {fogColor.getRed()/255.0f,
                               fogColor.getGreen()/255.0f,
                               fogColor.getBlue()/255.0f, 1.0f};
        srv->setVertexShaderConstant("fogColor", fogColorVec, 4);
    }

    if (!firstdone)
    {
        s32 tex = 0;
        srv->setVertexShaderConstant("tex", &tex, 1);

        firstdone = true;
    }
}

void SplattingProvider::OnSetConstants(IMaterialRendererServices *srv, int)
{
    if (!m_light_dir_calculated)
    {
        m_light_dir_calculated = true;
        m_light_direction = -World::getWorld()->getTrack()->getSunRotation().rotationToDirection();
        srv->setVertexShaderConstant("lightdir", &m_light_direction.X, 3);
    }

    if (!firstdone)
    {
        s32 tex_layout = 1;
        srv->setPixelShaderConstant("tex_layout", &tex_layout, 1);

        s32 tex_detail0 = 2;
        srv->setPixelShaderConstant("tex_detail0", &tex_detail0, 1);

        s32 tex_detail1 = 3;
        srv->setPixelShaderConstant("tex_detail1", &tex_detail1, 1);

        s32 tex_detail2 = 4;
        srv->setPixelShaderConstant("tex_detail2", &tex_detail2, 1);

        s32 tex_detail3 = 5;
        srv->setPixelShaderConstant("tex_detail3", &tex_detail3, 1);

        if (m_lightmap)
        {
            s32 tex_lightmap = 6;
            srv->setPixelShaderConstant("tex_lightmap", &tex_lightmap, 1);
        }

        firstdone = true;
    }
}

void SphereMapProvider::OnSetConstants(IMaterialRendererServices *srv, int)
{
    if (!firstdone)
    {
        s32 tex = 0;
        srv->setPixelShaderConstant("texture", &tex, 1);

        srv->setVertexShaderConstant("lightdir", &m_light_direction.X, 3);

        firstdone = true;
    }
}

void BubbleEffectProvider::OnSetConstants(IMaterialRendererServices *srv, int)
{
    if (m_visible && m_transparency < 1.0f)
    {
        m_transparency += GUIEngine::getLatestDt()*0.3f;
        if (m_transparency > 1.0f) m_transparency = 1.0f;
    }
    else if (!m_visible && m_transparency > 0.0f)
    {
        m_transparency -= GUIEngine::getLatestDt()*0.3f;
        if (m_transparency < 0.0f) m_transparency = 0.0f;
    }

    float time = (irr_driver->getDevice()->getTimer()->getRealTime() - initial_time) / 1000.0f;
    srv->setVertexShaderConstant("time", &time, 1);
    srv->setVertexShaderConstant("transparency", &m_transparency, 1);
}