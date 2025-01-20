#include "PluginEditor.h"

const int PluginEditor::width = 140;
const int PluginEditor::height = 180;

// === Lifecycle ==============================================================
PluginEditor::PluginEditor (PluginProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setLookAndFeel(&lookAndFeel);
    setWantsKeyboardFocus(true);
    midGain.setSliderStyle(juce::Slider::LinearVertical);
    midGain.label.setPostfix(" dB");
    midGain.label.setShowPlusForPositive(true);
    midGain.label.setMaxDecimals(1);
    midGain.attachToParameter(&processorRef.tree, "mid-gain");
    addParameterControl(&midGain);
    sideGain.setSliderStyle(juce::Slider::LinearVertical);
    sideGain.label.setPostfix(" dB");
    sideGain.label.setShowPlusForPositive(true);
    sideGain.label.setMaxDecimals(1);
    sideGain.attachToParameter(&processorRef.tree, "side-gain");
    addParameterControl(&sideGain);
    setSize(width, height); // make sure setSize is the last function called
}

PluginEditor::~PluginEditor() { }

// === Graphics ===============================================================
void PluginEditor::paint(juce::Graphics &g)
{
    g.fillAll(findColour(CtmColourIds::normalBgColourId));
}

void PluginEditor::resized()
{
    int w = 56;
    int p = 8;
    midGain.setBounds((width / 2) - w - (p / 2), 40, w, height - 56);
    sideGain.setBounds((width / 2) + (p / 2), 40, w, height - 56);
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