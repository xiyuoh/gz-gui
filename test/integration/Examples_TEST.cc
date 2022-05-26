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

#include <ignition/common/Console.hh>
#include <ignition/common/Filesystem.hh>
#include <ignition/utils/ExtraTestMacros.hh>

#include "test_config.h"  // NOLINT(build/include)
#include "ignition/gui/Application.hh"

int g_argc = 1;
char* g_argv[] =
{
  reinterpret_cast<char*>(const_cast<char*>("./Examples_TEST")),
};

using namespace ignition;
using namespace gui;

auto kExampleConfigPath = common::joinPaths(PROJECT_SOURCE_PATH,
    "examples", "config");

//////////////////////////////////////////////////
/// Generate a list of examples to be built.
std::vector<std::string> GetExamples()
{
  std::vector<std::string> examples;

  // Iterate over directory
  ignition::common::DirIter endIter;
  for (ignition::common::DirIter dirIter(kExampleConfigPath);
      dirIter != endIter; ++dirIter)
  {
    auto base = ignition::common::basename(*dirIter);
    examples.push_back(base.erase(base.find(".config"), 7));
  }
  return examples;
}

//////////////////////////////////////////////////
class PluginsLoad: public ::testing::TestWithParam<std::string>
{
  /// \brief Build code in a temporary build folder.
  /// \param[in] _entry Example source code to build
  public: void Load(const std::string &_config);
};

// See https://github.com/ignitionrobotics/ign-gui/issues/75
/////////////////////////////////////////////////
TEST_P(PluginsLoad, IGN_UTILS_TEST_ENABLED_ONLY_ON_LINUX(Configs))
{
  common::Console::SetVerbosity(4);

  auto file = common::joinPaths(kExampleConfigPath, GetParam()) + ".config";
  Application app(g_argc, g_argv);
  app.AddPluginPath(common::joinPaths(PROJECT_BINARY_PATH, "lib"));
  EXPECT_TRUE(app.LoadConfig(file));
}

//////////////////////////////////////////////////
INSTANTIATE_TEST_SUITE_P(Example, PluginsLoad,
    ::testing::ValuesIn(GetExamples()),
    [](const ::testing::TestParamInfo<PluginsLoad::ParamType>& param) {
      return param.param;
    });
