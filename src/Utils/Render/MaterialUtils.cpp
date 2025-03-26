#include "MaterialUtils.hpp"

#include "../../SDK/Client/Render/Tessellator/RenderMaterialGroup.hpp"

static mce::MaterialPtr* uiFillColor = nullptr;
static mce::MaterialPtr* uiTextured = nullptr;
static mce::MaterialPtr* particlesAlpha = nullptr;
static mce::MaterialPtr* uiCrosshair = nullptr;
static mce::MaterialPtr* nametagDepthTested = nullptr;
static mce::MaterialPtr* selection_overlay_opaque = nullptr;

void MaterialUtils::update() {
    if (uiFillColor == nullptr)
        uiFillColor = mce::RenderMaterialGroup::createUI(HashedString("ui_fill_color"));

    if (uiTextured == nullptr)
        uiTextured = mce::RenderMaterialGroup::createUI(HashedString("ui_textured"));

    if (particlesAlpha == nullptr)
        particlesAlpha = mce::RenderMaterialGroup::createUI(HashedString("particles_alpha"));

    if (uiCrosshair == nullptr)
        uiCrosshair = mce::RenderMaterialGroup::createUI(HashedString("ui_crosshair"));
    if (nametagDepthTested == nullptr)
        nametagDepthTested = mce::RenderMaterialGroup::createUI(HashedString("name_tag_depth_tested"));
    if (selection_overlay_opaque == nullptr)
        selection_overlay_opaque = mce::RenderMaterialGroup::createUI(HashedString("selection_box"));
}

mce::MaterialPtr* MaterialUtils::getUIFillColor() {
    return uiFillColor;
}

mce::MaterialPtr* MaterialUtils::getUITextured() {
    return uiTextured;
}

mce::MaterialPtr* MaterialUtils::getParticlesAlpha() {
    return uiTextured;
}

mce::MaterialPtr* MaterialUtils::getUICrosshair() {
    return uiCrosshair;
}

mce::MaterialPtr* MaterialUtils::getNametag() {
    return nametagDepthTested;
}

mce::MaterialPtr* MaterialUtils::getSelectionOverlay() {
    return selection_overlay_opaque;
}