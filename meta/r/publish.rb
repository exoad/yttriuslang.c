#!/usr/bin/env ruby

visited = {}

cont = []

SOURCE = ARGV[0]
DIST = ARGV[1] || "#{SOURCE}.pub.hpp"

cont << SOURCE

def visit(file, visited, cont)
    return if (visited.include? file)
    visited[file] = [];
    File.open(file) do |f|
        f.readlines.each do |s|
            if s.strip =~ /#include\s*"(.*)"/
                depends = $1;
                if  !(visited.include? depends)
                    depends = File.dirname(f) + "/" + depends
                    cont << depends
                    visited[file] << depends
                end
            end
        end
    end
end

while not cont.empty?
    visit(cont.shift, visited, cont)
end

def reorder(file, dependency)
    return [file] if dependency[file].empty?
    return dependency[file].map {|dep| reorder(dep, dependency) }
                    .reduce(:concat)
                    .concat([file])
                    .uniq
end

File.open(DIST, 'w').write reorder(SOURCE, visited).map {|file|
    puts file
	"// File #{file}\n" + File.open(file).read
}.map { |s|
    s.gsub(/#include\s*"(.*)"[\r\n]*/, "")
}.reduce(:concat)
