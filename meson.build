project(
  'carrier',
  'c',
  version: '1.0.0',
  default_options: ['warning_level=3', 'c_std=c99'],
)

glfw_dep = dependency('glfw3')
glew_dep = dependency('glew')
cglm_dep = dependency('cglm')

src_files = files(
  'src/ball.c',
  'src/enemy.c',
  'src/main.c',
  'src/player.c',
)

carrier = executable(
  'carrier',
  src_files,
  dependencies: [glfw_dep, glew_dep, cglm_dep],
  link_args: ['-lm'],
  install: true,
)

test('test', carrier)
