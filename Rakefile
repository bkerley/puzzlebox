require 'rake'
require 'rake/loaders/makefile'
require_relative 'rake/tools.rb'

EXES = %w{test http_notice sqli}

SRC_LIST = FileList.new('src/*.c')

CC = 'clang'
CFLAGS = '-Wall -Werror -g -fblocks -I include -c'

LD = 'clang'
LDFLAGS = '-Wall -Werror'

task :default => EXES.map{ |x| "bin/#{x}"}

task :clean do
  sh "rm -rf bin build tmp"
end

directory 'build'
directory 'bin'
directory 'tmp'

final 'test'
final 'sqli', nil, %w{scrypt sqlite3 BlocksRuntime}
final 'http_notice', %w{http_notice llist}

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
