#include "PluginEditor.h"

const int PluginEditor::sliderW = 64;
const int PluginEditor::sliderH = 132;
const int PluginEditor::sliderTitleH = 22;
const int PluginEditor::paddingX = 6;
const int PluginEditor::paddingY = 12;
const int PluginEditor::margins = 16;

// === Lifecycle ==============================================================
PluginEditor::PluginEditor (PluginProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setLookAndFeel(&lookAndFeel);
    setWantsKeyboardFocus(true);
    midGain.setSliderStyle(juce::Slider::LinearVertical);
    midGain.slider.setColorOverride(juce::Colour::fromRGB(217, 43, 214));
    midGain.label.setPostfix(" dB");
    midGain.label.setMainFont(juce::FontOptions(13));
    midGain.label.setPostfixFont(juce::FontOptions(13));
    midGain.label.setShowPlusForPositive(true);
    midGain.label.setMaxDecimals(1);
    midGain.attachToParameter(&processorRef.tree, "mid-gain");
    addParameterControl(&midGain);
    sideGain.setSliderStyle(juce::Slider::LinearVertical);
    sideGain.slider.setColorOverride(juce::Colour::fromRGB(34, 190, 230));
    sideGain.label.setPostfix(" dB");
    sideGain.label.setMainFont(juce::FontOptions(13));
    sideGain.label.setPostfixFont(juce::FontOptions(13));
    sideGain.label.setShowPlusForPositive(true);
    sideGain.label.setMaxDecimals(1);
    sideGain.attachToParameter(&processorRef.tree, "side-gain");
    addParameterControl(&sideGain);
    int w = sliderW * 2 + margins * 3 + paddingX * 4;
    int h = sliderTitleH + sliderH + margins * 2 + paddingY * 2;
    setSize(w, h);
}

PluginEditor::~PluginEditor() { }

// === Graphics ===============================================================
void PluginEditor::paint(juce::Graphics &g)
{
    g.fillAll(findColour(CtmColourIds::normalBgColourId));
    g.setColour(findColour(CtmColourIds::darkBgColourId));
    int x1 = margins;
    int y = margins;
    int w = sliderW + paddingX * 2;
    int h = sliderTitleH + sliderH + paddingY * 2;
    int r = 16;
    g.fillRoundedRectangle(x1, y, w, h, r);
    int x2 = w + margins * 2;
    g.fillRoundedRectangle(x2, y, w, h, r);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(16));
    auto center = juce::Justification::centredTop;
    g.drawText("Mid", x1, y + paddingY, w, sliderTitleH, center);
    g.drawText("Side", x2, y + paddingY, w, sliderTitleH, center);
}

void PluginEditor::resized()
{
    int x1 = margins + paddingX;
    int y = margins + paddingY + sliderTitleH;
    midGain.setBounds(x1, y, sliderW, sliderH);
    int x2 = sliderW + margins * 2 + paddingX * 3;
    sideGain.setBounds(x2, y, sliderW, sliderH);
}

// === Helper Functions =======================================================
void PluginEditor::addParameterControl(ParameterControl* control)
{
    addAndMakeVisible(control->slider);
    addAndMakeVisible(control->label);
}

void PluginEditor::addComboBoxControl(ComboBoxControl* control)
{
    addAndMakeVisible(control->comboBox);
    addAndMakeVisible(control->title);
}