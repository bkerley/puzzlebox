require 'rake'
require 'rake/loaders/makefile'

EXES = %w{test http_notice sqli}

SRC_LIST = FileList.new('src/*.c')

CC = 'clang'
CFLAGS = '-Wall -Werror -g -fblocks -c'

LD = 'clang'
LDFLAGS = '-Wall -Werror'

def cc(dest, *args)
  sh "#{CC} #{CFLAGS} -o #{ dest } #{args.join ' '}"
end

def ld(dest, objs, libs=[])
  lib_args = libs.map{ |l| "-L#{l}" }.join(' ')
  sh "#{LD} #{LDFLAGS} -o #{ dest } #{objs.join ' '}"
end

task :default => EXES.map{ |x| "bin/#{x}"}

directory 'build'
directory 'bin'
directory 'tmp'

file 'bin/test' => %w{bin build/test.o} do |t|
  objs = t.prerequisites.grep(/\.o$/)
  ld t.name, objs
end

file 'bin/sqli' => %w{bin build/sqli.o}
file 'bin/http_notice' => %w{bin build/http_notice.o}

rule(%r{build/.+\.o} => [
       'build',
       proc do |t|
         src = File.basename(t, '.o')
         "src/#{src}.c"
       end
     ]) do |t|
  p t
  src = t.prerequisites.grep(%r{\.c$})
  cc t.name, src
end

file 'tmp/depends.mf' => ['tmp', *SRC_LIST] do |t|
  sh "echo -n > #{t.name}"
  SRC_LIST.each do |src|
    obj = File.basename(src, '.c')
    sh "#{CC} -I include -MM #{src} -MT build/#{obj}.o | sed 's/$/ build/' >> #{t.name}"
  end
end

import 'tmp/depends.mf'

task :clean do
  sh "rm -rf bin build tmp"
end
