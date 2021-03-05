#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include "digg/subwindow.h"
#include <chrono>
#include <filesystem>
#include <functional>
#include <regex>
#include <string>

namespace digg {

class FileDialog : public SubWindow
{
public:
  FileDialog(std::string title,
             std::string filter,
             std::function<void(const std::string&)> ok_fun,
             std::function<void()> cancel_fun);

  void draw_widgets() override;

private:
  void change_dir(const std::filesystem::path& p);
  void refresh_dir_list();
  [[nodiscard]] bool is_filtered_out(const std::filesystem::path& p) const;
  void add_to_dir_contents(const std::filesystem::path& p);
  void set_current_selected(const std::string& filename);

  std::string filter;
  std::regex filter_re;
  int selected_filter;

  std::function<void(const std::string&)> ok_fun;
  std::function<void()> cancel_fun;

  int current_item;

  std::vector<std::vector<std::string>> dir_contents;
  std::vector<std::vector<const char*>> dir_cont_bufs;

  std::vector<char> current_dir_buf;

  std::string current_selected;
  std::chrono::system_clock::time_point prev_selection_time;
  std::vector<char> curr_selected_buf;
};

}  // digg

#endif /* FILE_DIALOG_H */
