#include "sndmix/sinknode.h"
#include "sndmix/sourcenode.h"
#include <catch2/catch.hpp>

namespace mt::snd::unittest {

class MySink : public SinkNode
{
public:
  explicit MySink(std::size_t max_sources) : SinkNode{max_sources} {}

  void uncovered_visitor(std::function<void(SourceNode&)> fun)
  { visit_sources(std::move(fun)); }
  void uncovered_visitor(std::function<void(const SourceNode&)> fun) const
  { visit_sources(std::move(fun)); }
  [[nodiscard]] std::size_t uncovered_source_count() const { return source_count(); }
};

class MySource : public SourceNode
{
public:
  MySource() = default;

  void put_data(std::span<float> /*output_buffer*/,
                const PaStreamCallbackTimeInfo& /*time_info*/,
                std::size_t /*num_output_channels*/) override
  {
  }
};

TEST_CASE("can_visit_source", "[sndmix]")
{
  MySink sink{2};
  MySource source1;
  MySource source2;

  SECTION("no sources")
  {
    sink.uncovered_visitor([](SourceNode& /*sn*/) { FAIL("Should not be called"); });
    sink.uncovered_visitor([](const SourceNode& /*sn*/) { FAIL("Should not be called"); });
  }
  SECTION("one source")
  {
    sink.add_source(source1);
    int calls = 0;
    sink.uncovered_visitor([&calls, &source1](SourceNode& sn)
                           {
                             CHECK(&source1 == &sn);
                             ++calls;
                           });
    sink.uncovered_visitor([&calls, &source1](const SourceNode& sn)
                           {
                             CHECK(&source1 == &sn);
                             ++calls;
                           });
    CHECK(calls == 2);
  }
  SECTION("two sources")
  {
    sink.add_source(source1);
    sink.add_source(source2);
    std::vector<const SourceNode*> sources_called;
    sink.uncovered_visitor([&sources_called](SourceNode& sn)
                           {
                             sources_called.push_back(&sn);
                           });
    sink.uncovered_visitor([&sources_called](const SourceNode& sn)
                           {
                             sources_called.push_back(&sn);
                           });
    CHECK(std::vector<const SourceNode*>{&source1, &source2, &source1, &source2} == sources_called);
  }
}

TEST_CASE("cannot_add_more_than_max_sources", "[sndmix]")
{
  MySink sink{1};
  MySource source;
  sink.add_source(source);
  CHECK_THROWS_AS(sink.add_source(source), std::runtime_error);
}

TEST_CASE("source_count_is_num_of_sources", "[sndmix]")
{
  const auto N{32};
  MySink sink{N};
  MySource source0;
  CHECK(0 == sink.uncovered_source_count());
  sink.add_source(source0);
  CHECK(1 == sink.uncovered_source_count());
  MySource source1;
  sink.add_source(source1);
  CHECK(2 == sink.uncovered_source_count());
}

}  // mt::snd::unittest
