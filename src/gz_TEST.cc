/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <string>

#include <gz/common/Filesystem.hh>
#include <gz/common/Util.hh>
#include <gz/utilities/ExtraTestMacros.hh>

#include "test_config.h"  // NOLINT(build/include)

#ifdef _MSC_VER
#    define popen _popen
#    define pclose _pclose
#endif

static const std::string kIgnCommand(
    std::string(BREW_RUBY) + std::string(IGN_PATH));

/////////////////////////////////////////////////
std::string custom_exec_str(std::string _cmd)
{
  _cmd += " 2>&1";
  FILE *pipe = popen(_cmd.c_str(), "r");

  if (!pipe)
    return "ERROR";

  char buffer[128];
  std::string result = "";

  while (!feof(pipe))
  {
    if (fgets(buffer, 128, pipe) != NULL)
      result += buffer;
  }

  pclose(pipe);
  return result;
}

using namespace gz;

class CmdLine : public ::testing::Test
{
  // Documentation inherited
  protected: void SetUp() override
  {
    // Change environment variable so that test files aren't written to $HOME
    common::env(IGN_HOMEDIR, this->realHome);
    EXPECT_TRUE(common::setenv(IGN_HOMEDIR, this->kFakeHome.c_str()));
  }

  // Documentation inherited
  protected: void TearDown() override
  {
    // Restore $HOME
    EXPECT_TRUE(common::setenv(IGN_HOMEDIR, this->realHome.c_str()));
  }

  /// \brief Directory to act as $HOME for tests
  public: const std::string kFakeHome = common::joinPaths(PROJECT_BINARY_PATH,
      "test", "fake_home");

  /// \brief Store user's real $HOME to set it back at the end of tests.
  public: std::string realHome;
};

// See https://github.com/gazebosim/gz-gui/issues/75
TEST_F(CmdLine, IGN_UTILS_TEST_ENABLED_ONLY_ON_LINUX(list))
{
  // Clear home if it exists
  common::removeAll(this->kFakeHome);

  // This line is flaky, see https://github.com/gazebosim/gz-gui/issues/415
  // EXPECT_FALSE(common::exists(this->kFakeHome));

  std::string output = custom_exec_str("ign gui -l");
  EXPECT_NE(output.find("TopicEcho"), std::string::npos) << output;
  EXPECT_NE(output.find("Publisher"), std::string::npos) << output;

  EXPECT_TRUE(common::exists(common::joinPaths(this->kFakeHome, ".ignition",
      "gui")));
}

//////////////////////////////////////////////////
/// \brief Check --help message and bash completion script for consistent flags
// See https://github.com/gazebo-tooling/release-tools/issues/398
TEST(ignTest, IGN_UTILS_TEST_DISABLED_ON_WIN32(GuiHelpVsCompletionFlags))
{
  // Flags in help message
  std::string helpOutput = custom_exec_str(kIgnCommand + " gui --help");

  // Call the output function in the bash completion script
  std::string scriptPath = common::joinPaths(std::string(PROJECT_SOURCE_DIR),
    "src", "cmd", "gui.bash_completion.sh");

  // Equivalent to:
  // sh -c "bash -c \". /path/to/gui.bash_completion.sh; _gz_gui_flags\""
  std::string cmd = "bash -c \". " + scriptPath + "; _gz_gui_flags\"";
  std::string scriptOutput = custom_exec_str(cmd);

  // Tokenize script output
  std::istringstream iss(scriptOutput);
  std::vector<std::string> flags((std::istream_iterator<std::string>(iss)),
    std::istream_iterator<std::string>());

  EXPECT_GT(flags.size(), 0u);

  // Match each flag in script output with help message
  for (const auto &flag : flags)
  {
    EXPECT_NE(std::string::npos, helpOutput.find(flag)) << helpOutput;
  }
}
