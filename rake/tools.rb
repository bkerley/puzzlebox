def cc(dest, *args)
  sh "#{CC} #{CFLAGS} -o #{ dest } #{args.join ' '}"
end

def ld(dest, objs, libs=[])
  lib_args = libs.map{ |l| "-l#{l}" }.join(' ')
  sh "#{LD} #{LDFLAGS} #{lib_args} -o #{ dest } #{objs.join ' '}"
end

def final(target_name, obj_names=nil, libs=[])
  target = "bin/#{target_name}"
  obj_names = [target_name] if obj_names.nil?
  objs = obj_names.map{ |n| "build/#{n}.o" }
  file target => ['bin', *objs] do |t|
    ld t.name, objs, libs
  end
end
