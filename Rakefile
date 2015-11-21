require 'rake'
require 'rake/loaders/makefile'

EXES = %w{test http_notice sqli}

SRC_LIST = FileList.new('src/*.c')

CC = 'clang'
CFLAGS = '-Wall -Werror -g -fblocks -I include -c'

LD = 'clang'
LDFLAGS = '-Wall -Werror'

def cc(dest, *args)
  sh "#{CC} #{CFLAGS} -o #{ dest } #{args.join ' '}"
end

def ld(dest, objs, libs=[])
  lib_args = libs.map{ |l| "-l#{l}" }.join(' ')
  sh "#{LD} #{LDFLAGS} #{lib_args} -o #{ dest } #{objs.join ' '}"
end

def objs(task)
  task.prerequisites.grep(/\.o$/)
end

task :default => EXES.map{ |x| "bin/#{x}"}

directory 'build'
directory 'bin'
directory 'tmp'

file 'bin/test' => %w{bin build/test.o} do |t|
  ld t.name, objs(t)
end

file 'bin/sqli' => %w{bin build/sqli.o} do |t|
  ld t.name, objs(t), %w{scrypt sqlite3 BlocksRuntime}
end

file 'bin/http_notice' => %w{bin build/http_notice.o build/llist.o} do |t|
  ld t.name, objs(t)
end

rule(%r{build/.+\.o} => [
       'build',
       proc do |t|
         src = File.basename(t, '.o')
         "src/#{src}.c"
       end
     ]) do |t|
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
