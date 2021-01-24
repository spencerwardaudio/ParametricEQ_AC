/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


std::vector<ParametricEQ_ACAudioProcessor::FilterInstance> createDefaultBands()
{
    std::vector<ParametricEQ_ACAudioProcessor::FilterInstance> defaults;
    
    for(int i = 0; i < 5; ++i)
    {
        int k = 0;
        
        if (i == 0) { k = 0; }
        else if (i == 4) { k = 2; }
        else { k = 1; }
    
        defaults.push_back(ParametricEQ_ACAudioProcessor::FilterInstance ("Freq" + String(i),  static_cast<ParametricEQ_ACAudioProcessor::FilterType>(static_cast<int>(ParametricEQ_ACAudioProcessor::HighPass) + k), (float)jmap(i, 0, 4, 20, 18000), 0.707f));
    }
    return defaults;
}

AudioProcessorValueTreeState::ParameterLayout  createParameterLayout()
{
    
    std::vector<std::unique_ptr<AudioProcessorParameterGroup>> parameters;
    
    const float maxGain = Decibels::decibelsToGain (24.0f);
    auto defaults = createDefaultBands();
    
    {
        auto param = std::make_unique<AudioParameterFloat>("Output", TRANS ("Output"), NormalisableRange<float> (0.0f, 2.0f, 0.01f), 0.8f, "Output level");

        auto group = std::make_unique<AudioProcessorParameterGroup> ("global", TRANS ("Globals"), "|", std::move (param));
        
        parameters.push_back (std::move (group));
    }
    
    
    std::vector<std::unique_ptr<AudioProcessorParameterGroup>> group;

    for(int i = 0; i < 5; ++i)
    {
             auto freqParameter = std::make_unique<AudioParameterFloat> ("Freq" + String(i),
                                                                         "Freq" + String(i),
                                                                         NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                         defaults [i].Freq,
                                                                         String(),
    
                                                                            AudioProcessorParameter::genericParameter,
                                                                            [](float value, int) { return (value < 1000) ?
                                                                                String (value, 0) + " Hz" :
                                                                                String (value / 1000.0, 2) + " kHz"; },
                                                                            [](String text) { return text.endsWith(" kHz") ?
                                                                                text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                                text.dropLastCharacters (3).getFloatValue(); });


             auto qltyParameter = std::make_unique<AudioParameterFloat> ("Q" + String(i),
                                                                         "Q" + String(i),
                                                                         NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                         defaults [i].Q,
                                                                         String(),
                                                                         AudioProcessorParameter::genericParameter,
                                                                         [](float value, int) { return String (value, 1) + " Q"; },
                                                                         [](const String& text) { return text.getFloatValue(); });

             
             auto gainParameter = std::make_unique<AudioParameterFloat> ("Gain" + String(i),
                                                                           "Gain" + String(i),
                                                                           NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                               std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                         defaults [i].Gain, String(),
                                                                          AudioProcessorParameter::genericParameter,
                                                                         [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                         [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());} );
    
            auto activeParameter = std::make_unique<AudioParameterBool>("ActiveState" + String(i), "ActiveState" + String(i), defaults[i].ActiveState, String(), [](float value, int){ return value > 0.5 ? TRANS("F Active") : TRANS("F  Bypassed"); });


            auto groupItem = std::make_unique<AudioProcessorParameterGroup> ("band" + String(i), defaults [i].name, "|",
                                                                        
                                                                        std::move (freqParameter),
                                                                        std::move (qltyParameter),
                                                                        std::move (gainParameter),
                                                                        std::move (activeParameter));
                                                    
            parameters.push_back (std::move (groupItem));
        }

    return { parameters.begin(), parameters.end() };
    
}

//==============================================================================
ParametricEQ_ACAudioProcessor::ParametricEQ_ACAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),  treeState(*this, nullptr, "Parameter", createParameterLayout())

#endif
{
    frequencies.resize(300);

    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequencies [i] = 20.0 * std::pow (2.0, i/ 30.0);
    }

    magnitudes.resize(frequencies.size());
    
    filterInstances = createDefaultBands();
    
    for (size_t i = 0; i < 5; ++i)
    {
        filterInstances [i].magnitudes.resize(frequencies.size(), 1.0);
    }
        
    for(int i = 0; i < 5; ++i)
    {
        treeState.addParameterListener ("Freq" + String(i), this);
        treeState.addParameterListener ("Q" + String(i), this);
        treeState.addParameterListener ("Gain" + String(i), this);
        treeState.addParameterListener ("ActiveState" + String(i), this);
    }

    treeState.addParameterListener ("Output", this);
    
    treeState.state = ValueTree (JucePlugin_Name);

    mPresetManager = std::make_unique<ACPresetManager>(this);
    
}

ParametricEQ_ACAudioProcessor::~ParametricEQ_ACAudioProcessor()
{
    inputAnalyser.stopThread (1000);
}

//==============================================================================
const String ParametricEQ_ACAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParametricEQ_ACAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParametricEQ_ACAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParametricEQ_ACAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParametricEQ_ACAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParametricEQ_ACAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParametricEQ_ACAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParametricEQ_ACAudioProcessor::setCurrentProgram (int index)
{
}

const String ParametricEQ_ACAudioProcessor::getProgramName (int index)
{
    return {};
}

void ParametricEQ_ACAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ParametricEQ_ACAudioProcessor::prepareToPlay (double newSampleRate, int samplesPerBlock)
{
        sampleRate = newSampleRate;

        dsp::ProcessSpec spec;
        spec.sampleRate = newSampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = getTotalNumOutputChannels();

    for (size_t i=0; i < 5; ++i) {
    updateBand(i);
    }
    
    filterChain1.get<5>().setGainLinear (*treeState.getRawParameterValue ("Output"));
    
    updatePlot();

    filterChain1.prepare(spec);
    
    inputAnalyser.setupAnalyser  (int (sampleRate), float (sampleRate));
}

void ParametricEQ_ACAudioProcessor::releaseResources()
{
    inputAnalyser.stopThread (1000);
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool ParametricEQ_ACAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ParametricEQ_ACAudioProcessor::parameterChanged (const String& param, float value)
{

        if (param == "Output")
            updatePlot();

        int i = 0;

        if (param == "Freq")
            getBand(i)->Freq = value;

        else if (param == "Q")
            getBand(i)->Q = value;
                
        else if (param == "Gain" )
            getBand(i)->Gain = value;
                
        else if (param == "ActiveState")
            getBand(i)->ActiveState = value;

            updateBand(i);

            return;

}

size_t ParametricEQ_ACAudioProcessor::getNumBands () const
{
    return filterInstances.size();
}

String ParametricEQ_ACAudioProcessor::getBandName   (size_t index) const
{
    if (isPositiveAndBelow (index, filterInstances.size()))
        return filterInstances [size_t (index)].name;
    return TRANS ("unknown");
}


ParametricEQ_ACAudioProcessor::FilterInstance* ParametricEQ_ACAudioProcessor::getBand (size_t index)
{
    if (isPositiveAndBelow (index, filterInstances.size()))
        return &filterInstances [index];
    return nullptr;
}


StringArray ParametricEQ_ACAudioProcessor::getFilterTypeNames()
{
    return {
        TRANS ("Low Pass"),
        TRANS("Peak"),
        TRANS("High Pass")
    };
}

void ParametricEQ_ACAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block (buffer);
    
    dsp::ProcessContextReplacing<float> context (block);
    filterChain1.process(context);
    
    inputAnalyser.addAudioData (buffer, 0, 2);
    
    updatePlot();
}

AudioProcessorValueTreeState& ParametricEQ_ACAudioProcessor::getPluginState()
{
    return treeState;
}

const std::vector<double>& ParametricEQ_ACAudioProcessor::getMagnitudes ()
{
    return magnitudes;
}

bool ParametricEQ_ACAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ParametricEQ_ACAudioProcessor::createEditor()
{
    return new ParametricEQ_ACAudioProcessorEditor (*this);
}


void ParametricEQ_ACAudioProcessor::updateBand (const size_t index)
{
    if (sampleRate > 0) {
        dsp::IIR::Coefficients<float>::Ptr newCoefficients;
        switch (filterInstances [index].type) {
                
            case HighPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, filterInstances [index].Freq, filterInstances [index].Q);
            break;
                
            case Peak:
                 newCoefficients = dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, filterInstances [index].Freq, filterInstances [index].Q, filterInstances [index].Gain);
                 break;
                
            case LowPass:
                newCoefficients = dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, filterInstances [index].Freq, filterInstances [index].Q);
                break;
            
            default:
                break;
        }

        if (newCoefficients)
        {
            {
                // minimise lock scope, get<0>() needs to be a  compile time constant
                ScopedLock processLock (getCallbackLock());
                if (index == 0)
                    *filterChain1.get<0>().state = *newCoefficients;
                if (index == 1)
                    *filterChain1.get<1>().state = *newCoefficients;
                if (index == 2)
                    *filterChain1.get<2>().state = *newCoefficients;
                if (index == 3)
                    *filterChain1.get<3>().state = *newCoefficients;
                if (index == 4)
                    *filterChain1.get<4>().state = *newCoefficients;

            }
            newCoefficients->getMagnitudeForFrequencyArray (frequencies.data(),
                                                            filterInstances [index].magnitudes.data(),
                                                            frequencies.size(), sampleRate);
            
        }
        bypassState();
        updatePlot();
    }
}

void ParametricEQ_ACAudioProcessor::updatePlot()
{
    auto Gain1 = filterChain1.get<5>().getGainLinear();
    std::fill (magnitudes.begin(), magnitudes.end(), Gain1);

    for (size_t i = 0; i < filterInstances.size(); ++i) {
        FloatVectorOperations::multiply (magnitudes.data(), filterInstances [i].magnitudes.data(), static_cast<int> (magnitudes.size()));
    }

    sendChangeMessage();
}

void ParametricEQ_ACAudioProcessor::setClickedBypass (int index)
{
    clicked = index;
    bypassState();
}

void ParametricEQ_ACAudioProcessor::bypassState()
{
    if (isPositiveAndBelow (clicked, filterInstances.size()))
    {
        for(int i = 0; i < 5; ++i)
            filterChain1.setBypassed<sizeof(i)>(!filterInstances[i].ActiveState);
    }
}


void ParametricEQ_ACAudioProcessor::createFrequencyPlot (Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble)
{

    p.startNewSubPath (bounds.getX(), mags [0] > 0 ? float (bounds.getCentreY() - pixelsPerDouble * std::log (mags [0]) / std::log (2)) : bounds.getBottom());
    const double xFactor = static_cast<double> (bounds.getWidth()) / frequencies.size();
    for (size_t i=1; i < frequencies.size(); ++i)
    {
        p.lineTo (float (bounds.getX() + i * xFactor),
                  float (mags [i] > 0 ? bounds.getCentreY() - pixelsPerDouble * std::log (mags [i]) / std::log (2) : bounds.getBottom()));
    }
}

void ParametricEQ_ACAudioProcessor::createAnalyserPlot (Path& p, const Rectangle<int> bounds, float minFreq, bool input)
{
    if (input)
        inputAnalyser.createPath (p, bounds.toFloat(), minFreq);
}

bool ParametricEQ_ACAudioProcessor::checkForNewAnalyserData()
{
    return inputAnalyser.checkForNewData();
}

void ParametricEQ_ACAudioProcessor::getStateInformation (MemoryBlock& destData){}

void ParametricEQ_ACAudioProcessor::setStateInformation (const void* data, int sizeInBytes){}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametricEQ_ACAudioProcessor();
}
