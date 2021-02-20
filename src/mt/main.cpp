#include "muzaktracker.h"
#include "digg/sentry.h"

#include <docopt/docopt.h>
#include <imgui.h>
#include <SFML/Config.hpp>
#include <spdlog/spdlog.h>

static constexpr auto USAGE =
  R"(mt - Muzak Tracker

Usage:
    mt [--debug]
    mt (-h | --help)
    mt --version

Options:
    -h --help  Show this screen.
    --version  Show version.
    --debug    Set debug log level.
)";

static constexpr auto VERSION = "Muzak Tracker 0.1";

int main(int argc, const char* argv[])
{
  auto args = docopt::docopt(USAGE,
                             { std::next(argv), std::next(argv, argc) },
                             true,  // show help if requested
                             VERSION);  // version string

  spdlog::info("{} started", VERSION);
  digg::Sentry sentry{[](){ spdlog::info("{} stopped.", VERSION); }};

  if (args["--debug"].asBool())
  {
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Log level debug selected.");
  }

  spdlog::debug("SFML version {}.{}.{}", SFML_VERSION_MAJOR, SFML_VERSION_MINOR,
                SFML_VERSION_PATCH);
  spdlog::debug("ImGui version {}", IMGUI_VERSION);

  MuzakTracker mt;
  return mt.run();
}
