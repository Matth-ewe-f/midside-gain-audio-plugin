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
    static const int width;
    static const int height;
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
