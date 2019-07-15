{
  'targets': [{
    'target_name': 'socket',
    'sources': [
      'src/socket.cc',
    ],
    'include_dirs': [
      'src',
      '<!(node -e "require(\'nan\')")'
    ]
  }]
}