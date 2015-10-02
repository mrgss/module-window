MRuby::Gem::Specification.new('mrgss_window') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mrgss developers'
  spec.add_dependency('mrgss_core')
  spec.add_dependency('mrgss_types')
  #----------------------------------------------------------------------------
  # Micro environment for compiling
  #----------------------------------------------------------------------------
  require 'open3'
  glfw_src = "#{build_dir}/glfw-3.1.1"
  
  def run_command(env, command)
    STDOUT.sync = true
    puts "mrgss: [exec] #{command}"
    Open3.popen2e(env, command) do |stdin, stdout, thread|
      print stdout.read
      fail "#{command} failed" if thread.value != 0
    end
  end
  FileUtils.mkdir_p(build_dir)
  #----------------------------------------------------------------------------
  # Download GLFW 
  #----------------------------------------------------------------------------
  if (!File.exists?("#{glfw_src}.zip"))
    Dir.chdir(build_dir) do
      e = {}
      run_command e, 'wget https://github.com/glfw/glfw/releases/download/3.1.1/glfw-3.1.1.zip'
    end
  end
  #----------------------------------------------------------------------------
  # Unzip GLFW 
  #----------------------------------------------------------------------------
  if (!File.exists?(glfw_src))
    Dir.chdir(build_dir) do
      e = {}
      run_command e, 'unzip glfw-3.1.1.zip'
    end
  end
  #----------------------------------------------------------------------------
  # Build GLFW 
  #----------------------------------------------------------------------------
  glfw_build = "#{build_dir}/glfw_build"
  if (!File.exists?(glfw_build))
    FileUtils.mkdir(glfw_build)
	if ENV['OS'] == 'Windows_NT'
		e = {}
	    run_command e, "cmake -G \"MSYS Makefiles\" -B#{glfw_build} -H#{glfw_src}"
    else
		e = {}
    	run_command e, "cmake -B#{glfw_build} -H#{glfw_src}"
    end
    e = {}
    run_command e, "make -C #{glfw_build}"
  end
  #----------------------------------------------------------------------------
  # Add GLFW to linker
  #----------------------------------------------------------------------------
  if(File.exists?("#{glfw_build}/src/libglfw3.a"))
    spec.linker.library_paths.push("#{glfw_build}/src")
    spec.linker.libraries << 'glfw3'
    spec.cc.include_paths << "#{glfw_src}/include"
  end
  #----------------------------------------------------------------------------
  # Add GLFW dependencies to linker
  #----------------------------------------------------------------------------
  if ENV['OS'] == 'Windows_NT'
  	spec.linker.libraries << 'opengl32'
	spec.linker.libraries << 'gdi32'
	spec.linker.libraries << 'user32'
	spec.linker.libraries << 'kernel32'
  else
	spec.linker.libraries << 'GL'
  	spec.linker.libraries << 'X11'
  	spec.linker.libraries << 'Xrandr'
  	spec.linker.libraries << 'Xinerama'
  	spec.linker.libraries << 'Xcursor'
  	spec.linker.libraries << 'Xi'
  	spec.linker.libraries << 'Xxf86vm'
  	spec.linker.libraries << 'pthread'
  end
end
