#include "audiostream.h"

#include "audiomixer.h"
#include "channel.h"
#include <memory>
#include <portaudio.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>



#include <sstream>

namespace mt::snd {
namespace {

int audio_callback(const void* /*input_buffer*/, void* output_buffer,
                   unsigned long frames_per_buffer,
                   const PaStreamCallbackTimeInfo* time_info,
                   PaStreamCallbackFlags /*status_flags*/,
                   void* user_data)
{
  auto* out = static_cast<float*>(output_buffer);
  static_cast<AudioMixer*>(user_data)->write_out(out, frames_per_buffer,
                                                 *time_info);
  return 0;
}

void debug_dump_device_info(const PaDeviceInfo* devinfo)
{
  spdlog::debug("Device name '{}' ({})", devinfo->name, devinfo->hostApi);
  spdlog::debug(" - Max I/O channels: {}/{}.", devinfo->maxInputChannels,
                devinfo->maxOutputChannels);
  spdlog::debug(" - Default latencies: {:0.3f}-{:0.3f}/{:0.3f}-{:0.3f} s.",
                devinfo->defaultLowInputLatency,
                devinfo->defaultHighInputLatency,
                devinfo->defaultLowOutputLatency,
                devinfo->defaultHighOutputLatency);
  spdlog::debug(" - Default sample rate: {} Hz.", devinfo->defaultSampleRate);
}

const auto output_channels{2};  // stereo

} // anonymous namespace

AudioStream::AudioStream(int audio_device_index) :
  mixer{std::make_unique<AudioMixer>(output_channels)}
{
  PaStreamParameters* input_params = nullptr; // no input
  PaStreamParameters output_params{};
  output_params.device = audio_device_index;
  output_params.channelCount = output_channels;
  output_params.sampleFormat = paFloat32;
  const auto* devinfo = Pa_GetDeviceInfo(output_params.device);
  debug_dump_device_info(devinfo);
  output_params.suggestedLatency = devinfo->defaultLowOutputLatency;
  output_params.hostApiSpecificStreamInfo = nullptr;
  const auto sample_rate = devinfo->defaultSampleRate;
  // Let the platform/underlying API decide what's best
  const auto frames_per_buffer = paFramesPerBufferUnspecified;
  const auto stream_flags = paNoFlag;

  auto err = Pa_IsFormatSupported(input_params, &output_params, sample_rate);
  if (err != paNoError)
  {
    throw std::runtime_error{fmt::format("Error opening stream: Format is not supported. {}",
                                         Pa_GetErrorText(err))};
  }

  spdlog::debug("Audio format is supported.");

  err = Pa_OpenStream(&stream, input_params, &output_params, sample_rate,
                      frames_per_buffer, stream_flags, audio_callback,
                      mixer.get());
  if (err != paNoError)
  {
    throw std::runtime_error{fmt::format("Error opening stream: {}", Pa_GetErrorText(err))};
  }
  spdlog::debug("Audio stream opened.");

  err = Pa_StartStream(stream);
  if (err != paNoError)
  {
    throw std::runtime_error{fmt::format("Error starting stream: {}", Pa_GetErrorText(err))};
  }
  spdlog::debug("Audio stream started.");
}

AudioStream::~AudioStream()
{
  auto err = Pa_StopStream(stream);
  if (err != paNoError)
  {
    spdlog::error("Error stopping stream: {}", Pa_GetErrorText(err));
  }
  spdlog::debug("Audio stream stopped.");
  err = Pa_CloseStream(stream);
  if (err != paNoError)
  {
    spdlog::error("Error closing stream: {}", Pa_GetErrorText(err));
  }
  spdlog::debug("Audio stream closed.");
}

std::vector<std::unique_ptr<AudioChannel>> AudioStream::create_channels(std::size_t count)
{
  std::vector<std::unique_ptr<AudioChannel>> channels{};
  channels.resize(count);
  for (auto& c : channels)
  {
    auto channel = std::make_unique<Channel>();
    mixer->add_source(*channel);
    c = std::move(channel);
  }

  return channels;
}

} // namespace mt::snd
