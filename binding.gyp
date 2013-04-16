{
  'targets': [
    {
      'target_name': 'jpegoptim',
      'sources': [
        'src/jpegoptim.cc',
        'src/optimize.cc',
        'src/jpeginfo.cc'
      ],
      'libraries': [
            '-lm -ljpeg'
      ]
    }
  ]
}
