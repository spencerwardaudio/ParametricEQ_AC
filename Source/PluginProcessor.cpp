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
    
    defaults.push_back(ParametricEQ_ACAudioProcessor::FilterInstance ("Freq1", ParametricEQ_ACAudioProcessor::HighPass, 20.0f, 0.707f));
    
    defaults.push_back(ParametricEQ_ACAudioProcessor::FilterInstance ("Freq2", ParametricEQ_ACAudioProcessor::Peak, 500.0f, 0.707f));
    
    defaults.push_back(ParametricEQ_ACAudioProcessor::FilterInstance ("Freq3", ParametricEQ_ACAudioProcessor::Peak, 1200.0f, 0.707f));
    
    defaults.push_back(ParametricEQ_ACAudioProcessor::FilterInstance ("Freq4", ParametricEQ_ACAudioProcessor::Peak, 5000.0f, 0.707f));
    
    defaults.push_back(ParametricEQ_ACAudioProcessor::FilterInstance ("Freq5", ParametricEQ_ACAudioProcessor::LowPass, 18000.0f, 0.707f));
    
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


             auto freqParameter1 = std::make_unique<AudioParameterFloat> ("Freq1",
                                                                         "Freq1",
                                                                         NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                         defaults [0].Freq,
                                                                         String(),
    
                                                                            AudioProcessorParameter::genericParameter,
                                                                            [](float value, int) { return (value < 1000) ?
                                                                                String (value, 0) + " Hz" :
                                                                                String (value / 1000.0, 2) + " kHz"; },
                                                                            [](String text) { return text.endsWith(" kHz") ?
                                                                                text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                                text.dropLastCharacters (3).getFloatValue(); });


             auto qltyParameter1 = std::make_unique<AudioParameterFloat> ("Q1",
                                                                         "Q1",
                                                                         NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                         defaults [0].Q,
                                                                         String(),
                                                                         AudioProcessorParameter::genericParameter,
                                                                         [](float value, int) { return String (value, 1) + " Q"; },
                                                                         [](const String& text) { return text.getFloatValue(); });

             
             auto gainParameter1 = std::make_unique<AudioParameterFloat> ("Gain1",
                                                                           "Gain1",
                                                                           NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                               std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                         defaults [0].Gain, String(),
                                                                          AudioProcessorParameter::genericParameter,
                                                                         [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                         [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());} );
    
            auto activeParameter1 = std::make_unique<AudioParameterBool>("ActiveState1", "ActiveState1", defaults[0].ActiveState, String(), [](float value, int){ return value > 0.5 ? TRANS("F1 Active") : TRANS("F1 Bypassed"); });

            
            auto freqParameter2 = std::make_unique<AudioParameterFloat> ("Freq2",
                                                                        "Freq2",
                                                                        NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                        defaults [1].Freq,
                                                                        String(),
                                                                         AudioProcessorParameter::genericParameter,
                                                                         [](float value, int) { return (value < 1000) ?
                                                                             String (value, 0) + " Hz" :
                                                                             String (value / 1000.0, 2) + " kHz"; },
                                                                         [](String text) { return text.endsWith(" kHz") ?
                                                                             text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                             text.dropLastCharacters (3).getFloatValue(); });

            auto qltyParameter2 = std::make_unique<AudioParameterFloat> ("Q2",
                                                                        "Q2",
                                                                        NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                        defaults [1].Q,
                                                                        String(),
                                                                        AudioProcessorParameter::genericParameter,
                                                                        [](float value, int) { return String (value, 1) + " Q"; },
                                                                        [](const String& text) { return text.getFloatValue(); });

            
            auto gainParameter2 = std::make_unique<AudioParameterFloat> ("Gain2",
                                                                          "Gain2",
                                                                          NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                              std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                        defaults [1].Gain, String(),
                                                                         AudioProcessorParameter::genericParameter,
                                                                        [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                        [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());} );
    
            auto activeParameter2 = std::make_unique<AudioParameterBool>("ActiveState2", "ActiveState2", defaults[1].ActiveState, String(), [](float value, int){ return value > 0.5 ? TRANS("F2 Active") : TRANS("F2 Bypassed"); });
    

            auto freqParameter3 = std::make_unique<AudioParameterFloat> ("Freq3",
                                                                        "Freq3",
                                                                        NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                        defaults [2].Freq,
                                                                        String(),
                                                                         AudioProcessorParameter::genericParameter,
                                                                         [](float value, int) { return (value < 1000) ?
                                                                             String (value, 0) + " Hz" :
                                                                             String (value / 1000.0, 2) + " kHz"; },
                                                                         [](String text) { return text.endsWith(" kHz") ?
                                                                             text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                             text.dropLastCharacters (3).getFloatValue(); });


            auto qltyParameter3 = std::make_unique<AudioParameterFloat> ("Q3",
                                                                        "Q3",
                                                                        NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                        defaults [2].Q,
                                                                        String(),
                                                                        AudioProcessorParameter::genericParameter,
                                                                        [](float value, int) { return String (value, 1) + " Q"; },
                                                                        [](const String& text) { return text.getFloatValue(); });

            
            auto gainParameter3 = std::make_unique<AudioParameterFloat> ("Gain3",
                                                                          "Gain3",
                                                                          NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                              std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                        defaults [2].Gain, String(),
                                                                         AudioProcessorParameter::genericParameter,
                                                                        [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                        [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());} );
    
            auto activeParameter3 = std::make_unique<AudioParameterBool>("ActiveState3", "ActiveState3", defaults[2].ActiveState, String(), [](float value, int){ return value > 0.5 ? TRANS("F3 Active") : TRANS("F3 Bypassed"); });
    
    
    auto freqParameter4 = std::make_unique<AudioParameterFloat> ("Freq4",
                                                                "Freq4",
                                                                NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                defaults [3].Freq,
                                                                String(),
                                                                 AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) { return (value < 1000) ?
                                                                     String (value, 0) + " Hz" :
                                                                     String (value / 1000.0, 2) + " kHz"; },
                                                                 [](String text) { return text.endsWith(" kHz") ?
                                                                     text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                     text.dropLastCharacters (3).getFloatValue(); });


    auto qltyParameter4 = std::make_unique<AudioParameterFloat> ("Q4",
                                                                "Q4",
                                                                NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                defaults [3].Q,
                                                                String(),
                                                                AudioProcessorParameter::genericParameter,
                                                                [](float value, int) { return String (value, 1) + " Q"; },
                                                                [](const String& text) { return text.getFloatValue(); });

    
    auto gainParameter4 = std::make_unique<AudioParameterFloat> ("Gain4",
                                                                  "Gain4",
                                                                  NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                      std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                defaults [3].Gain, String(),
                                                                 AudioProcessorParameter::genericParameter,
                                                                [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());} );
    
    auto activeParameter4 = std::make_unique<AudioParameterBool>("ActiveState4", "ActiveState4", defaults[3].ActiveState, String(), [](float value, int){ return value > 0.5 ? TRANS("F4 Active") : TRANS("F4 Bypassed"); });
    
    
    
     auto freqParameter5 = std::make_unique<AudioParameterFloat> ("Freq5",
                                                                 "Freq5",
                                                                 NormalisableRange<float> {20.0f, 20000.0f, 1.0f, std::log (0.5f) / std::log (980.0f / 19980.0f)},
                                                                 defaults [4].Freq,
                                                                 String(),
                                                                  AudioProcessorParameter::genericParameter,
                                                                  [](float value, int) { return (value < 1000) ?
                                                                      String (value, 0) + " Hz" :
                                                                      String (value / 1000.0, 2) + " kHz"; },
                                                                  [](String text) { return text.endsWith(" kHz") ?
                                                                      text.dropLastCharacters (4).getFloatValue() * 1000.0 :
                                                                      text.dropLastCharacters (3).getFloatValue(); });


     auto qltyParameter5 = std::make_unique<AudioParameterFloat> ("Q5",
                                                                 "Q5",
                                                                 NormalisableRange<float> {0.1f, 10.0f, 1.0f, std::log (0.5f) / std::log (0.9f / 9.9f)},
                                                                 defaults [4].Q,
                                                                 String(),
                                                                 AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) { return String (value, 1) + " Q"; },
                                                                 [](const String& text) { return text.getFloatValue(); });

     
     auto gainParameter5 = std::make_unique<AudioParameterFloat> ("Gain5",
                                                                   "Gain5",
                                                                   NormalisableRange<float> {1.0f / maxGain, maxGain, 0.001f,
                                                                       std::log (0.5f) / std::log ((1.0f - (1.0f / maxGain)) / (maxGain - (1.0f / maxGain)))},
                                                                 defaults [4].Gain, String(),
                                                                  AudioProcessorParameter::genericParameter,
                                                                 [](float value, int) {return String (Decibels::gainToDecibels(value), 1) + " dB";},
                                                                 [](String text) {return Decibels::decibelsToGain (text.dropLastCharacters (3).getFloatValue());} );
    
    auto activeParameter5 = std::make_unique<AudioParameterBool>("ActiveState5", "ActiveState5", defaults[4].ActiveState, String(), [](float value, int){ return value > 0.5 ? TRANS("F5 Active") : TRANS("F5 Bypassed"); });
    
    
    
    

      auto group0 = std::make_unique<AudioProcessorParameterGroup> ("band" + String (0), defaults [0].name, "|",
                                                                    
                                                                    std::move (freqParameter1),
                                                                    std::move (qltyParameter1),
                                                                    std::move (gainParameter1),
                                                                    std::move (activeParameter1));
                                                
       parameters.push_back (std::move (group0));
    
    
        auto group1 = std::make_unique<AudioProcessorParameterGroup> ("band" + String (1), defaults [1].name, "|",
                                                                      
                                                                      std::move (freqParameter2),
                                                                    std::move (qltyParameter2),
                                                                      std::move (gainParameter2),
                                                                      std::move (activeParameter2));
    
    parameters.push_back (std::move (group1));
    
    
     auto group2 = std::make_unique<AudioProcessorParameterGroup> ("band" + String (2), defaults [2].name, "|",
                                                                   
                                                                    std::move (freqParameter3),
                                                                     std::move (qltyParameter3),
                                                                    std::move (gainParameter3),
                                                                   std::move (activeParameter3));
    
    parameters.push_back (std::move (group2));
    
     auto group3 = std::make_unique<AudioProcessorParameterGroup> ("band" + String (3), defaults [3].name, "|",
                                                                   
                                                                    std::move (freqParameter4),
                                                                     std::move (qltyParameter4),
                                                                    std::move (gainParameter4),
                                                                   std::move (activeParameter4));
    
    parameters.push_back (std::move (group3));
    
     auto group4 = std::make_unique<AudioProcessorParameterGroup> ("band" + String (4), defaults [4].name, "|",
                                                                   
                                                                    std::move (freqParameter5),
                                                                     std::move (qltyParameter5),
                                                                    std::move (gainParameter5),
                                                                   std::move (activeParameter5));
    
    parameters.push_back (std::move (group4));

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
        
        treeState.addParameterListener ("Freq1", this);
        treeState.addParameterListener ("Q1", this);
        treeState.addParameterListener ("Gain1", this);
        treeState.addParameterListener ("ActiveState1", this);
    
        treeState.addParameterListener ("Freq2", this);
        treeState.addParameterListener ("Q2", this);
        treeState.addParameterListener ("Gain2", this);
        treeState.addParameterListener ("ActiveState2", this);
    
        treeState.addParameterListener ("Freq3", this);
        treeState.addParameterListener ("Q3", this);
        treeState.addParameterListener ("Gain3", this);
        treeState.addParameterListener ("ActiveState3", this);
    
        treeState.addParameterListener ("Freq4", this);
        treeState.addParameterListener ("Q4", this);
        treeState.addParameterListener ("Gain4", this);
        treeState.addParameterListener ("ActiveState4", this);
    
        treeState.addParameterListener ("Freq5", this);
        treeState.addParameterListener ("Q5", this);
        treeState.addParameterListener ("Gain5", this);
        treeState.addParameterListener ("ActiveState5", this);  

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

        if (param == "Output") {
            updatePlot();
        }

            auto* filterBand0 = getBand (0);
            if (param == "Freq1") {
                filterBand0->Freq = value;
            }
            else if (param == "Q1" ) {
                filterBand0->Q = value;
            }
            else if (param == "Gain1") {
                filterBand0->Gain = value;
            }
            else if (param == "ActiveState1") {
                filterBand0->ActiveState = value >= 0.5f;
            }
    
            updateBand (0);
    

            auto* filterBand1 = getBand (1);
            if (param == "Freq2") {
                filterBand1->Freq = value;
            }
            else if (param == "Q2" ) {
                filterBand1->Q = value;
            }
            else if (param == "Gain2") {
                filterBand1->Gain = value;
            }
            else if (param == "ActiveState2") {
                filterBand0->ActiveState = value >= 0.5f;
            }
    
            updateBand (1);

            auto* filterBand2 = getBand (2);
            if (param == "Freq3") {
                filterBand2->Freq = value;
            }
            else if (param == "Q3" ) {
                filterBand2->Q = value;
            }
            else if (param == "Gain3") {
                filterBand2->Gain = value;
            }
            else if (param == "ActiveState3") {
                filterBand0->ActiveState = value >= 0.5f;
            }

            updateBand (2);


            auto* filterBand3 = getBand (3);
            if (param == "Freq4") {
                filterBand3->Freq = value;
            }
            else if (param == "Q4" ) {
                filterBand3->Q = value;
            }
            else if (param == "Gain4") {
                filterBand3->Gain = value;
            }
            else if (param == "ActiveState4") {
                filterBand0->ActiveState = value >= 0.5f;
            }

            updateBand (3);
    
            auto* filterBand4 = getBand (4);
            if (param == "Freq5") {
                filterBand4->Freq = value;
            }
            else if (param == "Q5" ) {
                filterBand4->Q = value;
            }
            else if (param == "Gain5") {
                filterBand4->Gain = value;
            }
            else if (param == "ActiveState5") {
                filterBand0->ActiveState = value >= 0.5f;
            }
    
            updateBand (4);

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
            
            //       for (auto i = magnitudes.begin(); i != magnitudes.end(); ++i)
            //       {
            //           std::cout << *i << ' ';
            //       }
            
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
    if (isPositiveAndBelow (clicked, filterInstances.size())) {
        filterChain1.setBypassed<0>(!filterInstances[0].ActiveState);
        filterChain1.setBypassed<1>(!filterInstances[1].ActiveState);
        filterChain1.setBypassed<2>(!filterInstances[2].ActiveState);
        filterChain1.setBypassed<3>(!filterInstances[3].ActiveState);
        filterChain1.setBypassed<4>(!filterInstances[4].ActiveState);
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

void ParametricEQ_ACAudioProcessor::getStateInformation (MemoryBlock& destData)
{

}

void ParametricEQ_ACAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametricEQ_ACAudioProcessor();
}
