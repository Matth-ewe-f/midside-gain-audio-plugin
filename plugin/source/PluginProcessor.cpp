#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterFactory.h"

// === Lifecycle ==============================================================
PluginProcessor::PluginProcessor()
	: AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	),
	tree(*this, nullptr, "PARAMETERS", createParameters())
{
#if PERFETTO
    MelatoninPerfetto::get().beginSession();
#endif
}

PluginProcessor::~PluginProcessor()
{
#if PERFETTO
    MelatoninPerfetto::get().endSession();
#endif
}

juce::AudioProcessorValueTreeState::ParameterLayout
PluginProcessor::createParameters()
{
	juce::AudioProcessorValueTreeState::ParameterLayout parameters;
	parameters.add(ParameterFactory::createGainParameter(
		"mid-gain", "Mid Gain", -12, 12, 0
	));
	parameters.add(ParameterFactory::createGainParameter(
		"side-gain", "Side Gain", -12, 12, 0
	));
	return parameters;
}

// === Plugin Information =====================================================
bool PluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

#if !JucePlugin_IsSynth
	// Check if the input matches the output
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif
	return true;
#endif
}

// === Process Audio ==========================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	juce::ignoreUnused(sampleRate, samplesPerBlock);
	midGainSmooth.reset(samplesPerBlock);
	midGainSmooth.setCurrentAndTargetValue(1);
	sideGainSmooth.reset(samplesPerBlock);
	sideGainSmooth.setCurrentAndTargetValue(1);
}

void PluginProcessor::releaseResources() { }

void PluginProcessor::processBlock
(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
	juce::ignoreUnused(midiMessages);
	auto numInputChannels = getTotalNumInputChannels();
	auto numOutputChannels = getTotalNumOutputChannels();
	// zeroes out any unused outputs (if there are any)
	for (auto i = numInputChannels; i < numOutputChannels; i++)
		buffer.clear(i, 0, buffer.getNumSamples());
	// get updated gain values
	float midGain = *tree.getRawParameterValue("mid-gain");
	midGainSmooth.setTargetValue(pow(10.0f, midGain / 20.0f));
	float sideGain = *tree.getRawParameterValue("side-gain");
	sideGainSmooth.setTargetValue(pow(10.0f, sideGain / 20.0f));
	// process the audio
	float* left = buffer.getWritePointer(0);
	float* right = buffer.getWritePointer(1);
	for (int i = 0;i < buffer.getNumSamples();i++)
	{
		float mid = left[i] + right[i];
		float side = left[i] - right[i];
		mid *= midGainSmooth.getNextValue();
		side *= sideGainSmooth.getNextValue();
		left[i] = (mid + side) / 2;
		right[i] = (mid - side) / 2;
	}
}

// === Factory Functions ======================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
	return new PluginProcessor();
}

juce::AudioProcessorEditor *PluginProcessor::createEditor()
{
	return new PluginEditor(*this);
}

// === State ==================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock &destData)
{
	auto state = tree.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
	if (xml.get() != nullptr && xml->hasTagName(tree.state.getType()))
		tree.replaceState(juce::ValueTree::fromXml(*xml));
}

