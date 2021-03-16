#include "file_dialog.h"
#include <chrono>
#include <filesystem>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <cstring>
#include <regex>

namespace digg {

  namespace {
    std::filesystem::path current_directory = std::filesystem::current_path();
    const std::size_t MAX_PATH_LEN{4096};

    std::regex to_regex(const std::string& filter)
    {
      std::string s{"^"};
      for (auto c : filter)
      {
        switch (c) {
        case '*':
          s += ".*?";
          break;
        case '.':
          s += "\\.";
          break;
        default:
          s.push_back(c);
          break;
        }
      }
      s.push_back('$');
      spdlog::debug(R"(filter "{}" gives regex "{}")", filter, s);
      return std::regex{s};
    }

  } // anonymous namespace

  FileDialog::FileDialog(std::string title,
                         std::string filter_,
                         std::function<void(const std::string&)> ok_fun_,
                         std::function<void()> cancel_fun_) :
    SubWindow{std::move(title)},
    filter{std::move(filter_)},
    filter_re{to_regex(filter)},
    selected_filter{0},
    ok_fun{std::move(ok_fun_)},
    cancel_fun{std::move(cancel_fun_)},
    current_item{0},
    current_dir_buf(MAX_PATH_LEN, '\0'),
    curr_selected_buf(MAX_PATH_LEN, '\0')
  {
    change_dir(current_directory);
  }

  void FileDialog::draw_widgets()
  {
    if (ImGui::InputText("Path", &current_dir_buf[0], current_dir_buf.size()))
    {
      change_dir(std::filesystem::path{&current_dir_buf[0]});
    }

    current_item = std::max(0,
                            std::min(current_item,
                                     static_cast<int>(dir_cont_bufs.size())));
    if (ImGui::ListBoxHeader("Files", static_cast<int>(dir_cont_bufs.size()), 10))
    {
      bool updated = false;
      for (std::size_t i = 0; i < dir_cont_bufs.size(); ++i)
      {
        ImGui::Columns(static_cast<int>(dir_cont_bufs[i].size()));
        for (std::size_t j = 0; j < dir_cont_bufs[i].size(); ++j)
        {
          bool selected = (static_cast<int>(i) == current_item);
          if (ImGui::Selectable(dir_cont_bufs[i][j], &selected))
          {
            current_item = static_cast<int>(i);
            updated = true;
          }
          ImGui::NextColumn();
        }
      }

      if (updated)
      {
        set_current_selected(dir_contents.at(static_cast<std::size_t>(current_item))[0]);
      }
      ImGui::ListBoxFooter();
    }

    if (ImGui::InputText("Filename", &curr_selected_buf[0], curr_selected_buf.size()))
    {
      set_current_selected(&curr_selected_buf[0]);
    }

    std::vector<const char*> filters{filter.c_str(), "*.*"};
    if (ImGui::Combo("Filter", &selected_filter, &filters[0], static_cast<int>(filters.size())))
    {
      filter_re = to_regex(filters.at(static_cast<std::size_t>(selected_filter)));
      refresh_dir_list();
    }

    if (ImGui::Button("OK"))
    {
      ok_fun((current_directory / current_selected).string());
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      cancel_fun();
    }
  }

  void FileDialog::change_dir(const std::filesystem::path& p)
  {
    std::strcpy(&current_dir_buf[0], p.string().c_str());

    if (!std::filesystem::is_directory(p))
    {
      return;
    }

    current_directory = std::filesystem::canonical(p);
    std::strcpy(&current_dir_buf[0], current_directory.string().c_str());

    refresh_dir_list();
    set_current_selected("");
  }

  void FileDialog::refresh_dir_list()
  {
    dir_contents.clear();
    dir_cont_bufs.clear();
    std::error_code ec;
    add_to_dir_contents(current_directory / ".");
    add_to_dir_contents(current_directory / "..");
    for (const auto& dirent : std::filesystem::directory_iterator{current_directory, ec})
    {
      if (ec)
      {
        spdlog::error("Error creating directory iterator: {}", ec.message());
        break;
      }
      if (!is_filtered_out(dirent.path()))
      {
        add_to_dir_contents(dirent.path());
      }
    }
  }

  bool FileDialog::is_filtered_out(const std::filesystem::path& p) const
  {
    if (std::filesystem::is_directory(p))
    {
      return false;
    }

    return !std::regex_match(p.filename().string(), filter_re);
  }

  namespace {

    std::string filename(const std::filesystem::path& p)
    {
      std::string s{p.filename()};
      if (std::filesystem::is_directory(p))
      {
        s += std::filesystem::path::preferred_separator;
      }
      return s;
    }

    std::string filesize(const std::filesystem::path& p)
    {
      std::ostringstream ost;
      std::error_code ec;
      auto fs = std::filesystem::file_size(p, ec);
      if (!ec)
      {
        ost << fs;
      }
      return ost.str();
    }

    std::string filetime(const std::filesystem::path& p)
    {
      auto lwt = std::chrono::file_clock::to_sys(std::filesystem::last_write_time(p));
      std::time_t clwt = decltype(lwt)::clock::to_time_t(lwt);
      std::string s{std::asctime(std::localtime(&clwt))};
      return s.substr(0, s.size() - 1);
    }

  } // anonymous namespace

  void FileDialog::add_to_dir_contents(const std::filesystem::path& p)
  {
    dir_contents.push_back(std::vector<std::string>{filename(p), filesize(p), filetime(p)});

    std::vector<const char*> bufs;
    bufs.reserve(dir_contents.back().size());
    for (const auto& s : dir_contents.back())
    {
      bufs.push_back(s.c_str());
    }
    dir_cont_bufs.push_back(bufs);
  }

  void FileDialog::set_current_selected(const std::string& filename)
  {
    if (filename == current_selected
        && (std::chrono::system_clock::now() - prev_selection_time) < std::chrono::milliseconds(500))
    { // double click
      if (filename.back() == std::filesystem::path::preferred_separator)
      {
        change_dir(current_directory / filename);
      }
      else
      {
        ok_fun((current_directory / filename).string());
      }
      return;
    }

    prev_selection_time = std::chrono::system_clock::now();

    current_selected = filename;
    std::strcpy(&curr_selected_buf[0], filename.c_str());
  }

}  // digg
