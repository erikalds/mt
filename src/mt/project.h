#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <string>

class Instrument;

/// Document class for one musical project/song.
class Project
{
public:
  explicit Project(std::string title);
  ~Project();

  Instrument* get_instrument(std::size_t idx);

private:
  std::string title;
  std::vector<Instrument> instruments;
};

#endif /* PROJECT_H */
