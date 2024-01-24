project('aioinput', 'cpp', version: '0.1.0')

sources = ['src/Main.cpp']

if target_machine.system() == 'linux'
  sources += 'src/InputBackendLinux.cpp'
endif

executable(
  'aioinput',
  sources,
  include_directories: [
    'asio2/include',
    'asio2/3rd',
  ],
)
