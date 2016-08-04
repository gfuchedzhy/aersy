from os import path
from waflib import Task, TaskGen
class strip(Task.Task):
	def keyword(self):
		return "Stripping"
	run_str = '${STRIP} -o ${TGT} ${SRC}'
	color   = 'CYAN'

@TaskGen.feature('strip')
@TaskGen.after('apply_link')
def add_strip_task(self):
	try:
		link_task = self.link_task
	except:
		return
	self.create_task('strip', self.link_task.outputs[0], self.link_task.outputs[0].change_ext(''))

class glsl(Task.Task):
    color = 'PINK'
    def keyword(self):
        return "Processing"
    def run(self):
        with open(self.outputs[0].abspath(), 'w') as out, open(self.inputs[0].abspath()) as inp:
           print('#include <glcxx/src/utils.hpp>', file=out)
           print('namespace glcxx {', file=out)
           print('template<typename TName>', file=out)
           print('const char* get_program_src();\n', file=out)
           print('template<>', file=out)
           print('const char* get_program_src<cts("' + path.splitext(path.basename(self.inputs[0].abspath()))[0] + '")>()', file=out)
           print('{\n\tstatic char retval[] = R"(', file=out)
           for line in inp:
               print(line, file=out, end='')
           print(')";\n\treturn retval;\n}\n}', file=out)

@TaskGen.extension('.glsl')
def process(self, node):
    cpp_node = node.change_ext('.cpp')
    self.create_task('glsl', node, cpp_node)
    self.source.append(cpp_node)

def options(opt):
    opt.add_option('--suppress-gl-error-checks', action='store_true', default=False, help='do not check opengl errors')
    opt.recurse('glcxx')
    opt.recurse('res')
    opt.load('compiler_cxx')

appname='aersy'
def configure(cnf):
    cnf.env.append_value('DEFINES', 'APPNAME="' + appname + '"')
    if cnf.options.suppress_gl_error_checks:
        cnf.env.append_value('DEFINES', 'GLCXX_SUPPRESS_GL_ERROR_CHECKS')
    cnf.recurse('glcxx')
    cnf.recurse('res')
    cnf.load('compiler_cxx')
    cnf.find_program('strip')
    cnf.check(features='cxx cxxprogram', lib=['sfml-window', 'sfml-system'], uselib_store='sfml')
    cnf.check(features='cxx cxxprogram', lib='assimp', uselib_store='assimp')
    cnf.check(features='cxx cxxprogram', defines='GL_GLEXT_PROTOTYPES', lib='GL', uselib_store='opengl')
    cnf.check_cxx(cxxflags='-std=c++14', uselib_store='cxxflags')
    cnf.check_cxx(header_name='glm/glm.hpp')
    cnf.check_cxx(cxxflags='-std=c++11', header_name='gli/load.hpp')

def build(bld):
    bld.recurse('glcxx')
    bld.recurse('res')

    bld(features = 'cxx cxxstlib',
        source   = bld.path.ant_glob(['src/engine/*.cpp', 'src/engine/*.glsl']),
        target   = 'engine',
        includes = ['src/engine', '.'],
        use      = ['sfml', 'cxxflags', 'opengl'])

    bld(features = 'cxx cxxprogram strip',
        source   = bld.path.ant_glob('src/app/*.cpp'),
        target   = appname + '.out',
        includes = ['src/engine', '.'],
        use      = ['engine', 'cxxflags', 'sfml', 'assimp', 'glcxx'])
