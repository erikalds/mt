#include "audiosystem.h"

#include "audiostream.h"
#include "audiochannel.h"
#include "mixer.h"
#include <portaudio.h>
#include <spdlog/spdlog.h>

namespace mt::snd {

AudioSystem::AudioSystem(std::optional<int> device_index)
{
  auto err = Pa_Initialize();
  if (err != paNoError)
  {
    spdlog::error("Failed to initialize PortAudio library: {}", Pa_GetErrorText(err));
    throw std::runtime_error(fmt::format("Failed to initialize PortAudio library: {}",
                                         Pa_GetErrorText(err)));
  }
  const auto* version = Pa_GetVersionInfo();
  spdlog::debug("Successfully initialized {}", version->versionText);

  list_devices();
  open_output(device_index);
}

AudioSystem::~AudioSystem()
{
  stream.reset();

  spdlog::debug("Terminating PortAudio...");
  auto err = Pa_Terminate();
  if (err != paNoError)
  {
    spdlog::error("Failed to terminate PortAudio: {}", Pa_GetErrorText(err));
  }
  else
  {
    spdlog::debug("PortAudio terminated successfully");
  }
}

std::unique_ptr<Mixer> AudioSystem::create_mixer()
{
  const auto default_channel_cnt{8};
  return std::make_unique<Mixer>(stream->create_channels(default_channel_cnt));
}

void AudioSystem::list_devices() const
{
  auto num_devices = Pa_GetDeviceCount();
  if (num_devices < 0)
  {
    spdlog::error("Pa_GetDeviceCount() returned: {}", num_devices);
  }
  const auto* hostApiInfo = Pa_GetHostApiInfo(Pa_GetDefaultHostApi());
  spdlog::info("Available devices ({} [{}]):", hostApiInfo->name, hostApiInfo->type);
  for (decltype(num_devices) i = 0; i < num_devices; ++i)
  {
    const auto* devinfo = Pa_GetDeviceInfo(i);
    std::string defdev{};
    if (hostApiInfo->defaultInputDevice == i)
    {
      defdev += " (";
      defdev += "default input device";
    }
    if (hostApiInfo->defaultOutputDevice == i)
    {
      if (defdev.empty())
      {
        defdev += " (";
      }
      else
      {
        defdev += ", ";
      }
      defdev += "default output device";
    }
    if (!defdev.empty())
    {
      defdev += ")";
    }

    spdlog::info(" - {} [{}/{}]{}", devinfo->name, i, devinfo->hostApi, defdev);
    spdlog::debug("  - I/O channels: {}/{}, latency: [{:0.3f}, {:0.3f}]/[{:0.3f}, {:0.3f}] s., sample rate: {} Hz.",
                  devinfo->maxInputChannels, devinfo->maxOutputChannels,
                  devinfo->defaultLowInputLatency, devinfo->defaultHighInputLatency,
                  devinfo->defaultLowOutputLatency, devinfo->defaultHighOutputLatency,
                  devinfo->defaultSampleRate);
  }
}

void AudioSystem::open_output(std::optional<int> device_index)
{
  auto idx = Pa_GetDefaultOutputDevice();
  if (device_index)
  {
    idx = *device_index;
  }

  spdlog::info("opening output device: {}", idx);
  stream = std::make_unique<AudioStream>(idx);
}

} // namespace mt::snd
