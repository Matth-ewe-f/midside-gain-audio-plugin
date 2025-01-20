#pragma once
#include "PluginProcessor.h"
#include "CtmLookAndFeel.h"
#include "ParameterControl.h"
#include "ComboBoxControl.h"

class PluginEditor final : public juce::AudioProcessorEditor
{
public:
    // === Lifecycle ==========================================================
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    // === Graphics ===========================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // === Layout Constants ===================================================
    static const int sliderW;
    static const int sliderH;
    static const int sliderTitleH;
    static const int paddingX;
    static const int paddingY;
    static const int margins;
    // === Member Variables ===================================================
    PluginProcessor& processorRef;
    CtmLookAndFeel lookAndFeel;
    ParameterControl midGain;
    ParameterControl sideGain;

    // === Helper Functions ===================================================
    void addParameterControl(ParameterControl*);
    void addComboBoxControl(ComboBoxControl*);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
