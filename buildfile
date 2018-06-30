
project := "IDKProject".

project_version := "1.0".

buildfile_version := "1.0".

url_to_src := "https://github.com/JHG777000/IDK/archive/master.zip".

build IDKBuild.

 options.

  on test_enable("-t", "--test", "Enable IDK test.").

  on toolchain_select("-s", "--select_toolchain=tool", "Select toolchain.").

 end options.

 get test_enable.

 get toolchain_select.

 if ( toolchain_select == nil && is_mac ).

  var toolchain_select := "clang".

 end if.

 if ( toolchain_select == nil && !is_mac ).

  var toolchain_select := "gcc".

 end if.

 message("Building GLFW...").

 make filepath glfw_buildfile_path from "resources" to "GLFW/GLFWbuildfile".

 files GLFWbuildfile(glfw_buildfile_path).

 if ( is_mac ).

  subproject GLFWProject("local",GLFWbuildfile,"-i __c -d").

 end if.

 if ( is_win ).

  subproject GLFWProject("local",GLFWbuildfile,"-i __w -d").

 end if.

 return_output GLFWProject.

 make filepath include_path from "resources" to "include".

 make filepath glfw_include_path from "resources" to "include" from GLFWProject.

 url URLForRKLib("https://raw.githubusercontent.com/JHG777000/RKLib/master/buildfile").

 subproject RKLibProject("local",URLForRKLib,nil).

 return_output RKLibProject.

 make filepath rklib_include_path from "resources" to "include" from RKLibProject.

 files IDKFiles("src.directory").

 sources IDKSource(IDKFiles).

 compiler IDKCompilerFlags("-Wall", "-I " + include_path, "-I " + rklib_include_path,
          "-I " + glfw_include_path).

 toolchain IDKToolChain(toolchain_select,IDKCompilerFlags).

 output IDK("library",IDKSource,IDKToolChain).

 if ( test_enable ).

  message("Running IDKTest...").

  grab RKLib from RKLibProject.

  grab libglfw3 from GLFWProject.

  files IDKTestFiles("main.c").

  sources IDKTestSource(IDKTestFiles,IDK,libglfw3,RKLib).

  output IDKTest("application",IDKTestSource,IDKToolChain).

  launch(IDKTest).

  message("Ran IDKTest.").

 end if.

end build.

build clean_build.

 url URLForRKLib("https://raw.githubusercontent.com/JHG777000/RKLib/master/buildfile").

 subproject RKLibProject("local",URLForRKLib,"-b clean_build").

 make filepath glfw_buildfile_path from "resources" to "GLFW/GLFWbuildfile".

 files GLFWbuildfile(glfw_buildfile_path).

 subproject GLFWProject("local",GLFWbuildfile,"-b clean_build").

 message("Cleaning IDK...").

 clean("build").

end build.

default IDKBuild.